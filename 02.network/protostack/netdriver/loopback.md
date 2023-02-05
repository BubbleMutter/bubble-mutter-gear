# 回环口驱动
```c++
static netdev_tx_t loopback_xmit(struct sk_buff *skb, struct net_device *dev) {
    // ...
    skb_orphan(skb); // 调用拥有者的回调
    // ...
    skb->protocol = eth_type_trans(skb, dev);
    // ...
    if (likely(netif_rx(skb) /* 重新回到协议栈 */ == NET_RX_SUCCESS)) {
        // 发包统计
    }
	return NETDEV_TX_OK;
}
```
1. 发包回调中, 再次收包到协议栈

# 回环口地址发包流程1
```c++
inet_sendmsg() > ping_v4_sendmsg() > ip_route_output_flow() > __ip_route_output_key() {
    fib_lookup(res) {
        // 查路由表; 传出到 res
        // 回环口的路由表项中 res.type 为 RTN_LOCAL
    }
    if (res.type == RTN_LOCAL) {
        dev_out = net->loopback_dev; // 本机路由, 出接口设置为 回环口
        goto make_route;
    }
make_route:
    __mkroute_output() {
        // 如果出接口设备是 回环口设备 or 目的ip是 回环地址;
        // 则拉起标记 flags |= RTCF_LOCAL
        rt_dst_alloc(flags) {
            // 如果标记了 RTCF_LOCAL
            // 然后 skb_dst(skb)->output = ip_local_deliver
            // 否则 skb_dst(skb)->output = ip_output
        }
    }
}
```
1. 最终, 在发包过程中, 走到 ip_local_deliver 重新回到协议栈收包

# 回环口地址发包流程2 (走完整 netfilter)
1. ping 127.0.0.1 (用户态走 raw-socket 发包)
```txt
loopback_xmit
dev_hard_start_xmit
__dev_queue_xmit
dev_queue_xmit
ip_finish_output2
ip_finish_output
ip_output
ip_local_out
ip_send_skb
ip_push_pending_frames
raw_sendmsg
inet_sendmsg
sock_sendmsg
SyS_sendto
```

2. tcp syn 包 (connect)
```txt
loopback_xmit
dev_hard_start_xmit
__dev_queue_xmit
dev_queue_xmit
ip_finish_output2
ip_finish_output
ip_output
ip_local_out
ip_queue_xmit
__tcp_transmit_skb
tcp_connect
tcp_v4_connect
__inet_stream_connect
inet_stream_connect
SyS_connect
```

# 小结
1. 回环口发包, 是走完整的 netfilter; 经过 local_out 到 postrouting
2. 再经过 netif_rx 回来, 到 local_in
3. 因此在 ip_finish_output2 中, 需要查路由, 查arp表项, 需要创建回环地址的 arp 表项的
4. 而 ip_finish_output2 是经过了 NF_INET_POST_ROUTING 后由 ip_finish_output 调用的
```c++
static int ip_finish_output2(struct net *net, struct sock *sk, struct sk_buff *skb) {
    // ...
	nexthop = (__force u32) rt_nexthop(rt, ip_hdr(skb)->daddr); // <-- 回环口地址
	neigh = __ipv4_neigh_lookup_noref(dev, nexthop);
	if (unlikely(!neigh))
		neigh = __neigh_create(&arp_tbl, &nexthop, dev, false); // 创建 回环地址 的 arp 表项
    // ...
}
```

# Q: 为什么 cat /proc/net/arp 看不到回环地址的 arp 表项 ?
1. 初始化 `arp_tbl->constructor` 为 `arp_constructor`
2. `arp_constructor()` 中把回环地址的 arp 表项, 打上 NUD_NOARP 表项
```c++
if (dev->flags & (IFF_NOARP | IFF_LOOPBACK)) {
	neigh->nud_state = NUD_NOARP;
}
```
3. `__neigh_create()` 创建 arp 表项时, 调用 `arp_tbl->constructor`
4. `arp_seq_ops->start` > `arp_seq_start()` >
   `neigh_seq_start()` > `neigh_get_idx_any()`
   `neigh_get_idx()` > { `neigh_get_first()`, `neigh_get_next()` }
5. `neigh_get_first()`, `neigh_get_next()`
   分别过滤掉带 NUD_NOARP 标记的 arp 表项