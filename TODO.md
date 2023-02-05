[TOC]
# kernel network
1. 协议栈的 scope 和 net_namespace 分别是什么意思 ??
2. https://zhuanlan.zhihu.com/p/22813775?refer=kls-software-arch-world  Linux性能优化12：网络IO的调度模型
3. https://www.cnblogs.com/listenerln/p/6846328.html
   稍微清晰一点的 ip_route_input_slow 函数分析(其实也很鸡肋)
4. 协议栈 qdisc gso nic tun tap
5. 参考下面两篇文章画函数调用关系图的方式
   https://blog.csdn.net/flynetcn/article/details/115740471
   http://blog.nsfocus.net/linux-tun-tap/

# netdev
- [kernel netdev](https://netdevconf.info/0x16/submit-proposal.html)
- if_tun.h
- if_tunnel.h
- if_vlan.h
- if_tap.h
- if_link.h
- if_ether.h
- if_arp.h
- inetdevice.h

# tcp/ip
https://wiki.aalto.fi/download/attachments/70789052/linux-tcp-review.pdf

# arp
arp_process 中改代码; 实现学习免费 arp
```c++
if (inet_confirm_dst_addr(in_dev, sip)) {
    n = __neigh_lookup(&arp_tbl, &sip, dev, 1);
}
```

# memory
https://www.cnblogs.com/xiaolincoding/p/13886559.html 内存一致性讨论

# tcp/ip
1. gro/gso
2. virtio
3. tcp rtt rto algorithm
