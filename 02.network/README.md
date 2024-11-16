# network
TCP IP 超简总结
http://www.embeddedlinux.org.cn/linux_net/
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