# VRRP
Virtual Router Redundancy Protocol
provides for automatic assignment of available Internet Protocol (IP) routers to **participating hosts**

virtual routers: **abstract representation** of **multiple routers**
i.e. master and backup routers  --> acting as a group

The default gateway of a **participating host** is assigned to the **virtual router** instead of a **physical router**

If the **physical router** that is routing packets on behalf of the **virtual router** fails, **another physical router** is selected to automatically replace it. The **physical router** that is forwarding packets at any given time is called the **master router**.


一个二层网LAN (比如说是傻瓜交换机)
两个WAC, 或者两个路由,实现VRRP协议
三个IP  vip, vrrp-ip-masker, vrrp-ip-backup,
两个WAC  抢占vip, 即记录在傻瓜交换机中的"mac地址-ip"项

场景1:
如果backup机子 收到VIP网段的IP报文, 通过vrrp-ip-*通讯转给master机子, master机子作路由
场景2:
如果master机子挂掉了, 则, 无人跟backup抢占vip, backup机子成为vip对应的唯一机子,
此时backup机子作路由


实现:
两个机子的 三层口 / vlanif 
1. 分别有自己的 ip 网段相同 地址不同
2. 主角色上的 vlanif / 三层口 有虚拟 ip
   备角色上没有
3. 定时器往外发送免费 arp, 通告虚拟 ip 地址

虚拟 mac:
为了防止主备倒换时, mac 不同导致的网络震荡问题, 现在使用一个虚拟mac
1. 两个机子的 三层口 / vlanif 分别有 upper 网口, vrrpX
2. 主角色上的 vrrpX mac 是 虚拟mac, ip 是虚拟 ip
   备角色上的 vrrpX mac 是 虚拟mac, ip 不配置, 主备倒换时才配置 虚拟ip
3. 定时器往外发送免费 arp, 通告虚拟 ip 地址
4. 此时发送的免费 arp 以及 arp_reply mac 是共同的 mac, ip 是 虚拟ip

如何创建 三层口/vlanif 的 upper 网口 ? macvlan 口
`sudo ip link add link enp0s3 name dummy_upper type macvlan`
其中 `link enp0s3` 字段指示新创建内核口  共享二层口, 即 lower 口
然后 type 有很多中类型, 其他类型还不懂, 这里先用了 macvlan
然后新创建的 dummy_upper 可以设置自己的 ip 和 mac
另外, 并非所有的网口类型都可以设置子接口
比如桥是没用的, 如
`ip link add link enp0s3 name br0 type bridge`
创建出来的 iflink 是新的, 且不能直接指定子接口