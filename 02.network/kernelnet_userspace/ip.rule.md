[TOC]
# ip rule
```log
#> ip rule show
0:	from all lookup local
32766:	from all lookup main
32767:	from all lookup default
```
1. 策略路由的策略形式: `<prio> <condition> lookup <route-table-id>`

# usage
```sh
ip rule show

ip rule add from 192.168.1.0/24 table 1 # add a rule without prio
ip rule add from 192.168.2.0/24 table 2 prio 10 # add a rule with prio

ip rule del from 192.168.1.0/24 table 1 # del a rule without prio
ip rule del from 192.168.2.0/24 table 2 prio 10 # del a rule with prio
ip rule del prio 10
ip rule del from 192.168.2.0/24
ip rule del table 1

ip route show table main
ip route show table 1
ip route add 192.168.5.0/24 via 10.242.234.129 table main
ip route add 192.168.2.0/24 via 10.242.234.123 table 2
ip route add default via 192.168.1.254 table 1
```
+ ip rule add 没有 prio 参数, 默认是尽可能低的优先级, 即 prio 值尽可能大,
    + 比方说已经有 `32766:	from all lookup main`, 则新增 `<prio>` 为 `32765`
+ ip rule del 可以指定 `<prio>`, `<route-table-id>`, `<condition>` 其中一个或组合
+ ip route add 新增路由项指定 `<route-table-id>` 否则默认是 `main`
    + 每个 table 中只有一个 default 路由项
+ lookup 254 同于 lookup main
+ lookup 255 同于 lookup default
+ lookup local 用于与本地 netcard 地址建立连接
  如果有优先级高于 local 的 table 吸走了路由, 会导致 ping 本地 netcard 地址也不通