[TOC]
# multilevel tx queueing mechanism
1. tcp limit
    1. /proc/sys/net/ipv4/tcp_limit_output_bytes
2. qdisc
    1. tc qdisc
    2. `ip link set txqueuelen 1000 dev eth0`
3. bql
    1. `ls /sys/class/net/eth0/queues/tx-*/byte_queue_limits/`
4. driver queue
    1. `ethtool -g eth0` tx parameter
