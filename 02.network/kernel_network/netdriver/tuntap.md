[TOC]
# tuntap
1. 实现 kernelspace 与 userspace 的转发
    1. netif_rx, 则 userspace `read()`
    2. userspace `write()`, 则 netif_tx
2. tap 只提供 L2 packet 的转发
3. tun 可提供 L3 packet 的转发

# tun (ip link iptap)

# tap (ip link mactap)
