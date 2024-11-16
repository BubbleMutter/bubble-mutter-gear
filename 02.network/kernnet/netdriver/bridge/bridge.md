# bridge
# 功能
子接口收到包 = 会往桥转发一次
向子接口发包 = 不会往bridge转发, 跟bridge没有任何关系了
往bridge发包 = bridge 把报文转发到所有子接口

用nft 限制 桥内端口间的 广播泛洪是有性能问题的;
```log
$> nft list ruleset
table bridge filter {
    chain forward {
        type filter hook forward priority 0; policy drop;
        iifname "ge*" oifname "ext1" accept
        iifname "ext1" oifname "ge*" accept
        iifname "lag*" oifname "ext1" accept
        iifname "ext1" oifname "lag*" accept
    }
}
```
因为: br_flood 函数中; list_for_each_entry_rcu遍历桥的每个子接口
    调用 maybe_deliver > deliver_clone > __br_forward > NF_HOOK(走到 bridge forward 链)
    如果 桥有大量端口, 将会使得每个端口都要走一遍, 非常耗费性能
改进: br_flood 函数中; 调用 maybe_deliver 前, 还会判断
    br_port->flags 的 BR_FLOOD BR_MCAST_FLOOD BR_BCAST_FLOOD 等标记位, 禁止 泛洪
    对应于 `/sys/class/net/$ifname/brport/{broadcast_flood, multicast_flood unicast_flood}`
    在内核口 加桥成功后, 设置
    系统调用的方法比较绕, 用的是 netlink
    `socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);`
    `man 7 rtnetlink, man 3 netlink`
    `rtnl_register(PF_BRIDGE, RTM_SETLINK, rtnl_bridge_setlink, NULL, 0);`
    `rtnl_register(PF_BRIDGE, RTM_GETLINK, NULL, rtnl_bridge_getlink, 0);`
    `br_setlink > br_setport`
    `br_getlink > br_getport`
    先获取; 再设置
实际上, 如果是针对端口集合A 到 端口集合B 的禁止转发, 但允许 端口集合C 到 端口集合B 的转发
    使用 nft 是合理的, 但是如果禁止所有 桥内端口 的转发, 使用 br_port->flags 标记更合适