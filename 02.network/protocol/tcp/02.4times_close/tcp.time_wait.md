[TOC]
# TIME_WAIT 状态的目的/意义
1. 防止具有相同"四元组"的"旧"数据包被收到
    1. 经过 2MSL 这个时间，让两个方向上的数据包都被丢弃
    2. 原来连接的数据包在网络中会消失(TTL)
    3. "被动关闭连接方" LAST_ACK 状态也超时转为 CLOSED
2. 保证"被动关闭连接方"能被正确的关闭
    1. 保证最后的 ACK 能让被动关闭方接收 (才能从 LAST_ACK 状态退出)
    2. 如果"被动关闭连接方"处于 LAST_ACK 没有收到 ACK, 会定时重发 FIN
       "主动关闭连接方" 处于 TIME_WATI 收到 FIN, 会重发 ACK

# server 有大量 TIME_WAIT 怎么触发的? 该如何处理?
1. 系统主动 关闭大量连接; 等待2MSL(最大的分段生存时间) 产生
2. 有大量peer端异常or超时, 导致系统关闭连接, 后续等待2MSL
3. 网络异常, 没有收到 passive-close 方的ACK, passive-close 此时处于 LAST_ACK 反复重发 FIN
```sh
echo 1  > /proc/sys/net/ipv4/tcp_syncookies     # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_reuse       # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_recycle     # 默认 0
echo 16384 > /proc/sys/net/ipv4/tcp_max_tw_buckets # 默认 65536
echo 30 > /proc/sys/net/ipv4/tcp_fin_timeout    # 默认60
echo 5  > /proc/sys/net/ipv4/tcp_orphan_retries # ??
```
1. tcp_tw_reuse    TIME_WAIT 的 sockets 可重新用于新的 连接
2. tcp_tw_recycle  TIME_WAIT 的 sockets 启用快速回收(默认是2MSL)
3. tcp_max_tw_buckets 下调 TIME_WAIT 的 sockets
    1. 一般不建议下调这个数; 而且可能下调了用处不大; 该进入的还是会进入
4. tcp_fin_timeout    加快 FIN_WAIT_2 进入 TIME_WAIT  的等待时间
    + 超过这个时间; FIN_WAIT_2 强制进入 TIME_WAIT
5. tcp_orphan_retries 加快 FIN_WAIT_1 进入 FIN_WAIT_2 的等待时间
    + 超过这个时间; FIN_WAIT_1 强制进入 FIN_WAIT_2
6. 参数 listen的fd前设置 SO_REUSEADDR
    + `setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt , sizeof(int);`