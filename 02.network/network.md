# network
TCP IP 超简总结
https://www.jianshu.com/p/9f3e879a4c9c
https://www.sans.org/security-resources/tcpip.pdf

man 3 byteorder bswap endian (字端控制)

+ OSI Model = physical / datalink / network / transport / session / presentation / application
+ TCP / IP  = physical / datalink / network / transport / application
+ `/etc/services`   = inet layer  = ftp/http/ssh/...
+ `/etc/protocols`  = ip layer    = tcp/udp/icmp/gre/ospf/ipip/...
+ `/etc/ethertypes` = ether layer = arp/rarp/ip/ipv6/lldp/MPLS/RoCE
+ `/etc/networks`   = subnet = loopback, link-local

# TODO:
Q: tcp option scale and timestamps 复用 mechanism
Q: How does a tcp-keepalive-probe segment look like? What is the header and payload?
Q: `connect()` on udp, will the performance be enhanced?
Q: TCP RTT and RTO algorithm is a quite difficult topic for me currently, hack it.

### Self Conclusion
一个通讯流程的抽象至少以下部分组成:
角色描述, 通讯时序, 通讯内容, 数据流向
#### 角色描述
至少包括角色集合, 升华版可以是关系描述
例子:
TCP(C/S)
SNMP(MasterAgent, Manager, Agent)
802.1x(supplicant, Authenticator, Authentication server)
到了更复杂的场景,  就需要考虑拓扑关系
#### 通讯时序-状态机
可以采用至少两种状态机模型
+ 每个角色的状态机, "变换函数"是可以"事件" 或[收/发 通讯内容](#通讯内容)
+ 全局上下文抽象状态机, 每个状态下是 不同角色的状态tuple
#### 通讯内容
协议头描述
数据内容的组成结构
TCP/IP 的话要指明端口
#### 数据流向
物理流向  抽象流向

接入层确实会多开一些业务，比如防环，认证。
然后核心层的 硬件转发性能要好一点，MAC地址表大一点，甚至有的交换机灾备的需求。