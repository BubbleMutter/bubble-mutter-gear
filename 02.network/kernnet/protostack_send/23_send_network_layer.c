ip_finish_output2(struct net *net, struct sock *sk, struct sk_buff *skb) {
    struct dst_entry *dst = skb_dst(skb);     // 这个 dst 哪里来的 ??
	struct rtable *rt = (struct rtable *)dst; // 通过 dst 索引到路由表 (一般情况下, 内核就一个)
	struct net_device *dev = dst->dev;
    u32 nexthop; // 下一跳 ip 地址, 要么是 dip; 要么是 网关 ip

    // ...
	rcu_read_lock_bh();
	nexthop = (__force u32) rt_nexthop(rt, ip_hdr(skb)->daddr); // 通过 dip 找下一跳 ip
	neigh = __ipv4_neigh_lookup_noref(dev, nexthop);            // 通过  ip 找邻居表
	if (unlikely(!neigh))
		neigh = __neigh_create(&arp_tbl, &nexthop, dev, false); // 这个时候是 incomplete 状态
	if (!IS_ERR(neigh)) {
		int res;

		sock_confirm_neigh(skb, neigh); // 广播发 arp 请求
		res = neigh_output(neigh, skb); // 走二层发包逻辑

		rcu_read_unlock_bh();
		return res;
	}
	rcu_read_unlock_bh();
    // ...
}