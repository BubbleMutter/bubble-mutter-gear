`vlan_proto_idx()` 方法异常报错了
非 ETH_P_8021Q ETH_P_8021AD 协议就报错的
这里分析一下这个报错可能产生的逻辑

# 理想流程
1. 驱动层
    1. `skb->protocol = eth_trans_type(skb);` 求得 ETH_P_8021Q 或者 ETH_P_8021AD
    2. 且 sbk->vlan_tci 没有设置 VLAN_TAG_PRESENT
2. `__netif_receive_skb_core`
    1. 如果 skb->protocol 是  ETH_P_8021Q 或者 ETH_P_8021AD 则 `skb_vlan_untag`
    2. 如果拉起 VLAN_TAG_PRESENT 则 `vlan_do_receive`
3. `skb_vlan_untag` 在没有设置 VLAN_TAG_PRESENT 时 __vlan_hwaccel_put_tag
    1. `__vlan_hwaccel_put_tag` 中赋值 `skb->vlan_proto` 和 `sbk->vlan_tci`
    2. 此时 `skb->vlan_proto = skb->protocol` 和 `skb->vlan_tci` 拉起 VLAN_TAG_PRESENT
4. `vlan_do_receive` 
    1. `vlan_do_receive` 中先找到 vlan口 `vlan_find_dev(skb->dev, skb->vlan_proto, vlan_id);`
    2. 也就是说这里的 skb->vlan_proto 参数, 是 ETH_P_8021Q 或者 ETH_P_8021AD

# 异常流程1
1. 驱动层
    1. `skb->protocol = eth_trans_type(skb);` 求得 ETH_P_8021Q 或者 ETH_P_8021AD
    2. 且 sbk->vlan_tci 故意设置 VLAN_TAG_PRESENT
2. `__netif_receive_skb_core`
    1. 如果 skb->protocol 是  ETH_P_8021Q 或者 ETH_P_8021AD 则 `skb_vlan_untag`
    2. 如果拉起 VLAN_TAG_PRESENT 则 `vlan_do_receive`
3. `skb_vlan_untag`
    1. 因为已经设置了 VLAN_TAG_PRESENT, 所以没有走 `__vlan_hwaccel_put_tag`
    2. 所以这个时候 skb->vlan_proto 可能是0没有设置, 或者是其他值
4. `vlan_do_receive` 
    1. `vlan_do_receive` 中先找到 vlan口 `vlan_find_dev(skb->dev, skb->vlan_proto, vlan_id);`
    2. 但是这里的 skb->vlan_proto 不一定是 ETH_P_8021Q 或者 ETH_P_8021AD
    3. 然后 vlan_find_dev > vlan_group_get_device > vlan_proto_idx
5. `vlan_proto_idx`
    1. `switch (vlan_proto)` 只处理 ETH_P_8021Q ETH_P_8021AD
    2. 其他 vlan_proto 会打印异常日志 `BUG();`

# 异常流程2
1. 驱动层
    1. `skb->protocol = eth_trans_type(skb);` 求得不是 ETH_P_8021Q 或者 ETH_P_8021AD
    2. 且 sbk->vlan_tci 故意设置 VLAN_TAG_PRESENT
2. `__netif_receive_skb_core`
    1. 由于 skb->protocol 不是 ETH_P_8021Q 或者 ETH_P_8021AD; 不走 `skb_vlan_untag`
    2. 如果拉起 VLAN_TAG_PRESENT 则 `vlan_do_receive`
3. `skb_vlan_untag` 不执行
4. `vlan_do_receive`
    1. `vlan_do_receive` 中先找到 vlan口 `vlan_find_dev(skb->dev, skb->vlan_proto, vlan_id);`
    2. 但是这里的 skb->vlan_proto 不一定是 ETH_P_8021Q 或者 ETH_P_8021AD
    3. 然后 vlan_find_dev > vlan_group_get_device > vlan_proto_idx
5. `vlan_proto_idx`
    1. `switch (vlan_proto)` 只处理 ETH_P_8021Q ETH_P_8021AD
    2. 其他 vlan_proto 会打印异常日志 `BUG();`

# 最终结论
1. 我们自己的驱动提取了 skb->vlan_tci 但漏了设置 skb->vlan_proto
2. 而报文中 vlan_tci 的 VLAN_TAG_PRESENT 位是有实际含义的, 即 CFI
3. 如果故意构造报文, 设置这个 cfi 位置,
4. 我们自己的驱动提取后, 就会触发这个 bug
5. 实际上这个 cfi 标记是在令牌环的网络设备中使用的,
   也就是如果我们这个驱动对接令牌环, 就会出问题