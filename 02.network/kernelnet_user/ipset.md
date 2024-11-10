[TOC]
# ipset
1. 内核机制: 存储一个 ip/mac/port 的集合, 该集合用于 iptables
2. 当地址变更时, 无须重新变更 iptables, 只增删 集合 中的成员即可
3. ipset 用户态工具, 用来通告内核 crud 集合表
