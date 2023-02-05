1. 扩容 skb.cb[], 扩容 `struct neighbor { struct net_device *real };`
2. 驱动层收包, 把 二层网口 信息插入到 skb.cb[] 中
3. arp.c::{ arp_rcv, arp_process } 收包时; 传递 skb.cb[] 中的二层口信息 到 neighbor
4. 驱动层 vlan 发包时, 提取 dip, 如果是单播
   1. 用 vlan_dev 和 dip, 再arp_tbl, 找neigh, 取得 real 成员
   2. 找不到 (处理路由场景) 再走路由得到路由项, 重新找一次
      Note: skb_dst(skb) 跑到二层驱动发包已经是 NULL 了, 只能重新找
6. neighbour.c::neigh_flush_dev 除了匹配 neighbour->dev, 还要匹配 neighbour->real 进行销毁
7. neighbour.c::neigh_destroy   销毁表项时, 销毁 neighbour->real 的引用
8. `neigh->real` 变更时通知用户态
   1. neighbour.c::neigh_update 识别标记 NEIGH_UPDATE_F_NEW_REAL; 执行 notify 逻辑
   2. arp.c::{ arp_rcv, arp_process } 收包时; 
      如果 real 更新, 传递 NEIGH_UPDATE_F_NEW_REAL 标记到 neigh_update
9. 通知用户态 neigh 表项中的 real
   1. 扩展 rta `enum { NDA_REAL_INFO, NDA_MAX, }`
   2. 自定义结构体
   3. 在 neighbour.c::neigh_fill_info 中使用 nla_put 把 rta 传递出去
10. /proc/net/arp 显示
   1. arp.c::arp_seq_show 打印首行时, 多加一列 Real
   2. arp.c::arp_format_neigh_entry 打印每行时, 多加一列 real->name

# 查找接口
```c++
/**
 * @brief 目的 ip 查 neigh 表中的 dev
 * @note  外层保证调用 rcu_read_lock(); rcu_read_unlock(); 包住
 * @return NULL 找不到; 否则是 bonding, vlanif, ge, mgmt 口之一 
 */
struct net_device *fdb_search(struct sk_buff *skb, const unsigned char *ip, int vid) {
    struct net_device *dev = NULL, *vlan_dev = NULL;
    struct neighbour *neigh = NULL;
    struct rtable *rt;
    u32 daddr, nexthop;

    vlan_dev = get_vlan_dev_via_vid(vid); // 这个 dev 已经 hold 了
    if (!vlan_dev) {
        goto out;
    }
    /* 先用 ip 和 vlan_dev 去查找 */
    neigh = neigh_lookup(&arp_tbl, ip, vlan_dev);
    /* 找不到, 再用 ip 重新找出口路由 */
    if (!neigh) {
        daddr = *(u32 *)ip;
        rt = ip_route_output(dev_net(skb->dev), daddr, 0/* saddr */, 0/* tos */, 0);
        if (rt) {
            nexthop = (__force u32) rt_nexthop(rt, daddr);  // 通过 dip 找下一跳
            /* note: 因为 dip 能跑到这里
             *       说明之前就是用 vlan_dev 和 nexthop 找出来的 neigh
             *       所以这里重新用 vlan_dev 和 nexthop 再找一遍
             */
            neigh = neigh_lookup(&arp_tbl, nexthop, vlan_dev); // 通过  ip 找邻居表
        }
    }
	if (!neigh) {
        goto out;
    }

    dev = rcu_dereference(neigh->real);
out:
    if (neigh) {
        neigh_release(neigh);
    }
    return dev;
}
```

# mac 地址漂移

# 路由双包问题
```txt
框堆叠跨框的二层聚合口A  (至少每个框的有一样线卡连着1个线)
                           ||
                   交换机聚合口B (二三层都可以) (与 聚合口A相连）
                   交换机其他口C (不与 框相连)
1.  配置 聚合口A，聚合口B 为直连路由且能互相 ping 通
2.  配置 其他口C与聚合口B 不在相同 vlan，不在相同网段
3.  框上 配置静态路由，目的IP 是 其他口C 的网段，下一跳是 聚合口B 的 IP地址。
4.  框上 ping 交换机其他口C, 需要保证不会出现双包问题
```