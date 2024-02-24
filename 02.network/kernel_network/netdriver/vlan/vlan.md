[TOC]
主目录 = ./net/8021q/

## vlan 关键入口
### 收包逻辑: __netif_receive_skb_core > vlan_do_receive > __netif_receive_skb_core
```c++
if (vlan_tx_tag_present(skb)) {
    if (pt_prev) {
        ret = deliver_skb(skb, pt_prev, orig_dev);
        pt_prev = NULL;
    }
    if (vlan_do_receive(&skb))
        goto another_round; // 这里地方 skb->dev 已经重载了
    else if (unlikely(!skb))
        goto out;
}
```
### 发包逻辑: vlan_dev_hard_start_xmit
```c++
    // 标记要加 tag skb->vlan_tci
    // 实际加上 tag 是在 dev_queue_xmit 里面
    struct vlan_dev_priv *vlan = vlan_dev_priv(dev);
	if (veth->h_vlan_proto != vlan->vlan_proto ||
	    vlan->flags & VLAN_FLAG_REORDER_HDR) {
		u16 vlan_tci;
		vlan_tci = vlan->vlan_id;
		vlan_tci |= vlan_dev_get_egress_qos_mask(dev, skb->priority);
		__vlan_hwaccel_put_tag(skb, vlan->vlan_proto, vlan_tci);
        // __vlan_hwaccel_put_tag() {
        //      skb->vlan_proto = vlan_proto;
        //      skb->vlan_tci = VLAN_TAG_PRESENT | vlan_tci;
        // }
	}

    // 一个 vlan只能有一个子设备; 通常就是桥(内核最初的设计)
    skb->dev = vlan->real_dev; // 重定向到vlan子设备
    len = skb->len;
    if (unlikely(netpoll_tx_running(dev)))
        return vlan_netpoll_send_skb(vlan, skb);

    ret = dev_queue_xmit(skb);
```
### vlan注册: vlan_ioctl_handler > register_vlan_device
### 设备方法: struct net_device_ops vlan_netdev_ops

### 加tag逻辑: dev_queue_xmit > dev_hard_start_xmit
```c++
    if (vlan_tx_tag_present(skb) &&
        !vlan_hw_offload_capable(features, skb->vlan_proto)) {
        skb = __vlan_put_tag(skb, skb->vlan_proto,
                        vlan_tx_tag_get(skb));
        if (unlikely(!skb))
            goto out;

        skb->vlan_tci = 0;  // 加完 tag 后, vlan_tci 不再携带 vlan 信息了
        // 也就是说; 到了 vlan 子接口, vlan_tci 是0, 拿不到了
    }
```
### 剥tag逻辑: __netif_receive_skb_core
```c++
	if (skb->protocol == cpu_to_be16(ETH_P_8021Q) ||
	    skb->protocol == cpu_to_be16(ETH_P_8021AD)) {
		skb = skb_vlan_untag(skb);
		if (unlikely(!skb))
			goto out;
	}
```

# 报文头格式 vlan_ethhdr
```c++
/**
 *	struct vlan_ethhdr - vlan ethernet header (ethhdr + vlan_hdr)
 *	@h_dest: destination ethernet address
 *	@h_source: source ethernet address
 *	@h_vlan_proto: ethernet protocol
 *	@h_vlan_TCI: priority and VLAN ID
 *	@h_vlan_encapsulated_proto: packet type ID or len
 */
struct vlan_ethhdr {
	unsigned char	h_dest[ETH_ALEN];
	unsigned char	h_source[ETH_ALEN];
	__be16		h_vlan_proto;
	__be16		h_vlan_TCI;
	__be16		h_vlan_encapsulated_proto;
};
```
# 术语映射
## db  层
vlan_mode vlan_tag vlan_trunks vlan_trunks_ranges vlan_tagged_ranges vlan_untagged_ranges
ovsdb-client dump Port name `ovsdb-client list-columns Port | grep vlan | sed 's| *$||' | awk '{print $1}' | sed ':T N; s|\n| |g; bT'`
vlan_tag: 指的是 pvlan
vlan_tagged_ranges: 由 vlan_trunks_ranges, vlan_trunks, 构造
vlan_untagged_ranges:
## 芯片层 pvlan, tag, untag
1. pvlan: 收到没有tag的报文, 芯片加上 tag
2. tag  : 报文转发出去时, 带上tag
3. untag: 报文转发出去时, 不带tag
## 服务层 access, native_tag, native_untag
1.

## 配置层 access, hybric,
