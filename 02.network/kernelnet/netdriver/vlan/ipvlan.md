[TOC]
# ipvlan
```txt
ipvlan0 -\
ipvlan1 -*- eth0
ipvlan2 -/
```
1. 在 macvlan 基础上迭代, 优化跨 namespace 通讯
2. `ipvlan*` 子接口直连 eth0, 无须 bridge, 或者本身说本身就是 bridge
3. eth0 的 rx_handler 替换为 ipvlan 的 rx_handler
    1. ipvlan 的 rx_handler 中转发 `ipvlan*` (ipvlan  的 bridge 模式)
    2. `ipvlan*` 可以跨 namespace 转发
    3. `ipvlan*` 直接配置到不同的 namespace 中 (无须 veth 的成对出现)
4. ipvlan L2 mode: 同于 macvlan 的 bridge 模式
    1. `ipvlan*` 间通过 arp 泛洪学习
    2. `ipvlan*` 间通过 mac forward table 转发
5. ipvlan L3 mode: `ipvlan*` 与 eth0 使用相同 mac, 但分别有各自 ip
    1. `ipvlan*` 打上了 NOARP 的标记
    2. `ipvlan*` 间通过 ip forward table 转发
