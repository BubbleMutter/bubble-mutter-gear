内核 bonding
bond口列表      = /sys/class/net/bonding_masters
bond口平衡模式  = /sys/class/net/$interface/bonding/mode []
bond口的配置    = /proc/net/bonding/$interface
bond口子接口    = /sys/class/net/$interface/bonding/slaves
    echo "+subinterface" > slaves
    echo "-subinterface" > slaves
bond口子接口    = /sys/class/net/$interface/bonding/active_slave
bond口子接口    = /sys/class/net/$interface/slave_$slave
bond口子接口    = /sys/class/net/$interface/lower_$slave

# 注册 子接口
bond_enslave

# 聚合口收包 (形同 bridge)
1. 注册子接口时 `bond_enslave` 注册 net_device 的 rx_handler
    + `netdev_rx_handler_register(bond_handle_frame)`
2. bond_handle_frame 转发入口, 找到上层聚合口, 设置 skb->dev
    1. `slave = bond_slave_get_rcu(skb->dev);`
	2. `bond = slave->bond;`
    3. `skb->dev = bond->dev;`
3. bond_handle_frame 修改完 skb->dev 后; 回到 __netif_receive_skb_core
