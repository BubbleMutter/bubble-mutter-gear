[TOC]
# ip
# ip header
+ Version = constant 4
+ Header Length (IHL) = length of header
+ Type of Service (TOS)
+ Total Length = total length of header and payload
+ Identification = work with fragments
+ DF (Don’t Fragment) = flag indicates no fragment
+ MF (More Fragments) = flag indicates more fragment
+ Fragment Offset = indicates fragment position
+ Time To Live (TTL) = default 64, calculate number of hops
+ Protocol = L4 Protocol, define in `/etc/protocols`
+ Header Checksum
+ Source Address = ip address
+ Destination Address = ip address
+ Options

# ip implementation (kernel main task)
1. Sanity checks
2. Firewall
3. Options Handle
4. Fragmentation
5. Receive, Transmit, and Forward

# ip address 5 class
+ class A = 0~127.x.x.x 首位为0 bin(0 0000 000, 0 1111 111)
    + 127.x = loopback
    + 10.x  = LAN
    + 政府
+ class B = 128~191.x.x.x 首位为10 bin(10 000 000, 10 111 111)
    + 172.16.0.0 ~ 172.31.255.255 = LAN
    + 169.254.x.x = reserve
+ class C = 192~223.x.x.x 首位为110 bin(110 00 000, 110 11 111)
    + 192.0.0.1 ~ 223.255.255.254 = WAN
    + 192.168.X.X = LAN
    + 个人/企业
+ class D = 224~239.x.x.x 首位为1110 bin(1110 0000, 1110 1111)
    + 224.0.0.0 ~ 239.255.255.255 = multicast / broadcast
+ class E = 240~255.x.x.x 首位为1111 (1111 0000, 1111 1111)
    + 试验阶段
    + 掩码是为了 使得IP网段不受制于 A-E类型
+ 掩码出现前 A-E分别固定 前n-bits表示网段

# ip fragment = (Identification + DF + MF + Fragment Offset)
+ id + offset + flags
id:     16 bits,
offset: 13 bits,  值为已装载的 bytes / 8
flags:  (3 bits), 保留|是否分片|是否末尾 (分包标志 至少需要 2-bits)

# ip unicast/broadcast/multicast
+ broadcast mac-dest-addr: FF-FF-FF-FF-FF-FF
    + A类 IP: xxx.255.255.255
    + B类 IP: xxx.xxx.255.255 所有子网
    + 所有子网 IP: xxx.xxx.yyy.255
    + 必须在同一子网 不能跨路由 (路由器不转发 跨网段的广播包)
+ multicast mac-dest-addr: 最高位为01 (01-xx..)
    + IP 规定是D类IP: 224.0.0.0 - 239.255.255.255
    + 既可以穿透路由器，又可以实现只给那些必要的组发送数据包
+ unicast
    + 直连 mac & ip 均为目标host
    + 经过路由 mac 为路由器 ip为目标host
一个网卡的broadcast 有什么用? 何时发生作用?
    when本机send dst-ip == broadcast, 不查询arp表, 构造链路层广播包, dst-Mac: ff-ff-...
    when本机recv dst-ip == broadcast, 即使不同于 网卡的ip, 不discard, 而是 进行后续response

# ip tos field (8bit)
1. 高 3bit: precedence, internetwork control
    1. 一般用于 ospf rip 协议的控制
2. 中 4bit:
    1. 1000: 最小延迟   minimize delay
    2. 0100: 最大吞吐量 maximize throughput
    3. 0010: 最高可靠性 maximize reliability
    4. 0001: 最小费用   minimize monetary cost
3. 低 1bit: reserve

# ip options
```c++
#include <linux/ip.h>

#define IPOPT_END
#define IPOPT_NOOP
#define IPOPT_SEC
#define IPOPT_LSRR
#define IPOPT_TIMESTAMP
#define IPOPT_CIPSO
#define IPOPT_RR
#define IPOPT_SID
#define IPOPT_SSRR
#define IPOPT_RA
```
IPOPT_END       = End of Option List = indicates end of options
IPOPT_NOOP      = No Operation = pad between options for byte align
IPOPT_TIMESTAMP = Timestamp =
IPOPT_RR        = Record Route = record routers in the path (9 routers at most)
IPOPT_SEC       = Security =
IPOPT_LSRR      = Loose Source and Record Route = follow all routers in the path
IPOPT_SSRR      = Strict Source and Record Route = one of the intermediated routers can use another router
IPOPT_CIPSO     =
IPOPT_SID       = Stream ID =
IPOPT_RA        = Router Alert = indicates packets need better routers