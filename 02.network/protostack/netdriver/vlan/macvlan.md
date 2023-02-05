[TOC]
# macvlan
```txt
macvlan0 -\
macvlan1 -*- eth0
macvlan2 -/
```
1. 比原始的 veth-in-vm <-> veth-in-host <-> bridge <-> phynic 性能更优
2. 即 macvlan-in-vm <-> phynic
    1. 没有 bridge 转发, bridge 学习/泛洪 等损耗
    2. 省去了 host 和 vm 两个不同 namespace 间 veth 的转发, 直接转发到物理网卡
3. eth0 的 rx_handler 替换为 macvlan 的 rx_handler
    1. macvlan 的 rx_handler 中转发 `macvlan*` (macvlan  的 bridge 模式)
    2. `macvlan*` 可以跨 namespace 转发
    3. `macvlan*` 直接配置到不同的 namespace 中 (无须 veth 的成对出现)
4. `macvlan*` 有独立的 mac 以及 ip
    1. 通过 mac forward table 实现转发

# restriction:
1. 接入层交换机一般会限制一个物理端口上的 mac 个数
2. 物理网卡是 wireless 的场景无法使用
