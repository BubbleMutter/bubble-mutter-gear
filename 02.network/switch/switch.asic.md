[TOC]
# ifp (acl)
1. acl 的资源分布
2. acl 的优先级
3. acl 关键业务所需 的条目
4. acl 统计命中次数 如何统计具体acl
5. acl 配置下发流程 (配置文件? 进程通信? 配置后场景触发(如拉黑)?)

# l2table
1. mac-forward-table is shared by multi asic on a single switch
2. on stack-mode, multi-switch's mac-forward-table is synchronize
3. using modid to indicate which asic is

# l3table
# vlan (access / trunk / hybric / pvlan)
1. pvlan = 每个物理口都有且只有一个 pvlan; 当收到没有 vlan 报文加上
    + 即 进入转发芯片 内部的报文一定是有 vlan 的
2. access = 入方向允许带 access vlan 以及 无 vlan 的报文进来
            出方向上剥掉报文 access vlan
3. trunk  = 入方向允许带 member vlan, native vlan 以及 无 vlan 的报文进来
            出方向上剥掉报文 native vlan, 保留 member vlan
4. hybric = 入方向运行带 tag-vlan, untag-vlan, native 以及无vlan 报文进来
            出方向上剥掉报文 native vlan, untag-vlan, 保留 tag-vlan

# lacp (trunk)

# cos

# port (7 types)
## 内核口 对应 硬件类型
1. 二层口 = 有硬件口, 内核口无 ip
2. 三层口 = 有硬件口, 内核口有 ip
    1. 需要设置 静态mac地址 的 cpu口
3. 二层聚合口 = 无硬件口, 占用芯片 trunk, 内核口无 ip
4. 二层聚合口的成员口 = 有硬件口, 内核口无 ip
    1. 内核口作为聚合口成员
5. 三层聚合口 = 无硬件口, 占用芯片 trunk, 内核口有 ip
    1. 需要设置 静态mac地址 的 cpu口
6. 三层聚合口的成员口 = 有硬件口, 内核口无 ip
    1. 内核口作为聚合口成员
7. VLANIF = 无硬件口

## learning state
1. Learn:      allow to learn into L2 table automatically
               if set; can be learned by software manually.
2. ToCPU:      allow copy to CPU automatically
3. Forwarded:  allow to forward automatically
               (is  这个口入方向报文, 可以自动转发)
               (not 其他口的报文, 可以转发到这个口)
4. Discard:    packet will be discarded. But can be recv by acl.
               if set; can be get by acl only.
+ 1 2 3 是并行的
+ 4会使得1 2 3失效; 只要1/2/3一个开启, 4必须为0;

## 2. stp state:
### Block  (一般用于 避免环路)
1. 特性
  1. 其他口进来的报文: 不许自动从这个口 自动转发 
  2. 这个口进来的报文: 不允许自动转发
  3. 但是, 可以通过acl匹配, 然后copytocpu
  4. 不学习 mac 地址
2. 已知场景:
  1. MAD, STP, 故意环路的拓扑, 不允许 SDK层环路(耗尽CPU)
  2. 单端口防环, 
### Forward: 
1. 特性
  1. 其他口进来的报文; 允许自动从这个口 自动转发

# frame
Fullmesh Framework
Crossbar Framework

# stp

# packet
收发包转发场景: 一般考虑收发 广播/单播 报文
发包模式区别: bypass 发包; vlan 发包