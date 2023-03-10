[TOC]
# 1. 三次握手问题
## Q: TCP 两次握手行不行? 为什么?
1. 两次握手无法验证源ip的真实性,
   被半连接攻击时大量消耗服务器资源
   而处于二次握手的服务端, 只会用少量资源记录未完成的连接,
   只有三次握手后服务端, 才会把完成的连接返回用户态的 accept 调用

## Q: 什么是 TCP 四次握手? 优缺点分别是什么?

## Q: 三层握手 server 和 client 的状态切换
server: LISTEN -> SYN_RECV -> ESTABLISHED
client: CLOSED -> SYN_SENT -> ESTABLISHED

## Q: 四次挥手 server 和 client 的状态切换
1. 主动发起断开
    1. 被动段: CLOSE_WAIT -> LAST_ACK -> CLOSED
    2. 发起端: FIN_WAIT_1 -> FIN_WAIT_2 -> TIME_WAIT -> CLOSED
    3. 发起端2: FIN_WAIT_1 -> TIME_WAIT -> CLOSED
2. 服务端和客户端同时断开
    2. FIN_WAIT_1 -> CLOSING -> TIME_WAIT -> CLOSED

## Q: 半连接DOS攻击应对 or 半连接队列满了处理?
+ 问题本身是 服务端处理三层握手不过来
```sh
echo 2048 > /proc/sys/net/ipv4/tcp_max_syn_backlog # 默认是 256;   半连接队列
echo 2048 > /proc/sys/net/core/somaxconn           # 默认是 128;   全连接队列
echo 65536 > /proc/sys/net/core/netdev_max_backlog # 默认是 1000;  网卡队列
echo 1 > /proc/sys/net/ipv4/tcp_abort_on_overflow  # 默认不开
echo 1 > /proc/sys/net/ipv4/tcp_syncookies         # 默认不开
echo 3 > /proc/sys/net/ipv4/tcp_synack_retries     # 默认是 5
```
1. 上调半连接队列
2. 上调全连接队列 和 `listen()`系统调用的第二参数 backlog
3. 上调网卡驱动上协议栈前的队列
4. 在 连接队列溢出时; 往客户端发 RST. (使得客户端感知)
5. 开启 tcp_syncookies 功能; 从而不启用 半连接队列, 节省资源
6. 减少 SYN+ACK 重传次数 (缩短 销毁二次握手的超时等待)

## Q: 如何调整 客户端 SYN 之后的等待时间
```sh
echo 3 > /proc/sys/net/ipv4/tcp_syn_retries
```

# 2. 四次挥手问题
## Q: server 有大量 CLOSE_WAIT 怎么触发的? 该如何处理?
1. 客户端主动关闭连接; 服务端进程还没来得及关闭socket;
2. 只能等待服务端进程关闭 socket

## Q: server 有大量 TIME_WAIT 怎么触发的? 该如何处理?
1. 系统主动 关闭大量连接; 等待2MSL(最大的分段生存时间) 产生
2. 有大量peer端异常or超时, 导致系统关闭连接, 后续等待2MSL
3. 网络异常, 主动连接方的ACK 没有被peer端收到, peer 处于 LAST_ACK 反复重发 FIN
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

## Q: 为什么要有四次挥手? 而握手只需要三次?

## Q: TIME_WAIT 状态的目的/意义

# 3. 滑动窗口问题
## Q: 零窗口攻击
1. tcp的流量控制: 对端的接收窗口故意设置为零  本段不能发送数据
2. 攻击者下载大文件; 且把接收窗口设置为零; 那么服务器的 FIN 报文则无法发送
3. 服务器程序尝试关闭连接；但 FIN 报文无法发送; socket 一直处于 FIN_WAIT_1
4. 解决方案: 下调 tcp_max_orphans

## Q: 空连接 tcp_keepalive*
1. 下调 3个 tcp_keepalive_* 参数
```sh
echo 1800 > /proc/sys/net/ipv4/tcp_keepalive_time # 默认 7200 (两小时)
echo 3 > /proc/sys/net/ipv4/tcp_keepalive_probes  # 默认 9
echo 30 > /proc/sys/net/ipv4/tcp_keepalive_invtl  # 默认 75 （秒)
```
2. tcp_keepalive 的机制:
    1. 当超过 tcp_keepalive_time 时间没有收到数据
    2. 尝试发送 tcp_keepalive_probes 次 keepalive
    3. 发送后等待间隔 tcp_keepalive_invtl
    4. 如果等待间隔中 接收到对端 keepalive 连接维持
    5. 否则超过发送次数; 内核判定连接失效 (errno ??)
3. 实际上 应用层协议也应该设计心跳逻辑
    1. 在应用层以更短的时间销毁 "无效" 连接
4. 重压服务器需要下调的场景
    1. 避免空连接攻击
    2. 加快对端物理宕机后; 本段socket的回收

## 什么是糊涂窗口综合症问题? 如何避免?
1. 问题描述
    1. 如果接收方一直没有读取数据; 导致发送窗口越来越小
    2. 到最后，如果接收方腾出几个字节并告诉发送方现在有几个字节的窗口
        而发送方会义无反顾地发送这几个字节
    3. 最终, 发送方发了 tcp+ip头 却携带了少量字节; 浪费网络性能
2. 解决方法
    1. 接收方不通告小窗口, `窗口 < min(mss, 缓冲区大小/2)` 即提前关闭
    2. 发送方不发送小数据, `buffer-size > mss or window-size > mss` 才发送 (Nagle 算法)
    3. 发送方收到之前 ack, 才发数据

## Q: 滑动窗口大小如何确定 ?
1. 接收方的接收缓存 - 减去已接收未读取 = 窗口大小
2. 接收方在 ack 报文中通过 tcp-header-window 字段告知发送方 窗口大小
3. tcp-header-window 只有 2 bytes, 窗口大小最大为 65535, 即 64K
4. tcp-option-window_scaling, 开启后, 窗口大小最大可以设置为 1G
    + 表示 1G 大小, 要 30 bits, 即 4 bytes.

## Q: 如何调整滑动窗口使得传输速率最大?
1. 使得 发送缓冲区 略小于 带宽时延积

## Q: 发送双方系统性能足够高; 能否无上限地提升 窗口 大小?
+ 不能; 窗口大小除了受 系统性能限制; 还受到 tcp 协议和网络性能限制
1. tcp协议: 不开启 tcp_window_scaling 窗口最大为 64K, 开启后, 窗口最大为 1G
2. 网络性能: 发送超过网络中间设备处理能力的报文, 网络设备把包丢掉

## Q: 发送窗口 和 接收窗口是否相等 ?
1. 不完全相等
2. 接收方 用户进程处理较快; 接收窗口很快变空
3. 接收方 通过 tcp-header-window 字段告知发送方更新窗口大小
4. 这个告知报文在网络中存在时延

# 4. 重传问题
## Q: 重传分为哪几种 ?
超时重传 和 快速重传
其中超时重传又分为 数据报文丢失 和 ack报文丢失
## Q: 什么是快速重传 ?
当连续收到3次重复的ack, 即判定存在丢失
## Q: 优点 和 缺点分别是什么?
+ 优点: 避免超时重传 的等待时间过长的问题
+ 缺点: 无法确认重传窗口内全部数据还是重传丢失的数据
    + 因为有可能 丢失数据 的后面数据是正常收到的
## Q: 如何避免快速重传的缺点?
开启 sack, echo 1 > /proc/net/core/ipv4/tcp_sack
使得 接收方把接收窗口的 bitmap 通过ack返回给发送端
发送从而准确地得知丢失的部分数据, 然后只重传丢失的部分数据
## Q: RTO 过大 or 过小分别会有什么问题?
1. RTO 太大, 导致超时重传需要等待过长时间, 降低传输效率
2. RTO 太小, 导致频繁重传, 加重网络负载, 甚至负循环产生过多的重传; 且消耗内核性能收发数据
## Q: RTO 如何计算 ?
动态采样 RTT, 并加权平均

# 5. 拥塞控制问题
## Q: 拥塞控制有哪几个部分组成
1. 慢启动
2. 拥塞避免
3. 拥塞发生
4. 快速回复

# 6. 混杂综合问题
## Q: 如何进行tcp优化 3个角度
1. 三次握手
    + 第一次 tcp_syn_retries
    + 第二次 半连接队列扩容 tcp_max_syn_backlog somaxconn backlog
    + 第二次 半连接队列停用 tcp_syncookies 一般来说只需要设置为 1
    + 第二次 半连接队列时效 tcp_synack_retries
    + 第三次 全连接队列扩容 somaxconn backlog(listen第二参数)
    + 绕过三次握手 tcp_fastopen
    + 第二次 半连接队列重置 tcp_abort_on_overflow 是的客户端感知到服务端队列已满, 客户端自行实现等待逻辑
2. 四次挥手
    + tcp_orphan_retries  (FIN报文重传次数) (主被动方)
    + tcp_max_orphans     (LAST_ACK/FIN_WAIT_1/FIN_WAIT_2 状态下的孤儿连接数) (主被动方)
    + tcp_fin_timeout     (FIN_WAIT_2 的状态维持时间) (仅主动方)
    + tcp_max_tw_buckets  (TIME_WAIT 状态的连接上限)  (仅主动方)
    + tcp_tw_reuse        (复用 TIME_WAIT 状态的连接) (仅主动方)
    + tcp_tw_recycle      (加快 TIME_WAIT 状态的回收) (仅主动方)
    + tcp_timestamps      (复用 TIME_WAIT 状态的连接) (仅主动方)
3. 数据传输
    + 扩大窗口大小 tcp_window_scaling
    + 发送缓冲区大小 tcp_wmem
    + 接收缓冲区大小 tcp_rmem
    + 开启接收缓冲区调节 tcp_moderate_rcvbuf
    + 调整内存范围和设置触发阈值 tcp_mem
在高并发服务器中，为了兼顾网速与大量的并发连接，我们应当保证缓冲区的动态调整的最大值达到带宽时延积，而最小值保持默认的 4K 不变即可。而对于内存紧张的服务而言，调低默认值是提高并发的有效手段。

同时，如果这是网络 IO 型服务器，那么，调大 tcp_mem 的上限可以让 TCP 连接使用更多的系统内存，这有利于提升并发能力。需要注意的是，tcp_wmem 和 tcp_rmem 的单位是字节，而 tcp_mem 的单位是页面大小。

## Q: 为什么会存在 tcp 分包和粘包?
### 分包
1. a) 发送端 待要发送的数据, 大于 接收端 即tcp缓冲区剩余空间
2. b) 发送端 待发送数据大于MSS（最大报文长度）
3. 然后 发送端将把待发送数据, 分开发送

### 粘包
1. tcp 有 ack通讯机制，为了提高通讯效率诞生的 滑动窗口机制。
2. 但在滑动窗口机制中，当接收方一直不读取数据，将导致糊涂窗口综合症，消耗网络资源。
3. 为了解决糊涂窗口综合症的, 发送端将等待足够多的数据包才发送, 已提升网络效率。

### 方案
1. 应用层 定义通信协议的两种方式:
    1. 定义数据包包头
    2. 在数据包之间设置边界(即定义包头 包尾)
2. 发送端关闭 Nagle 算法, 即可关闭发送粘包功能

## Q: tcp 的状态 列举一下? (11个)
1. CLOSED
2. LISTEN
3. SYN_SENT
4. SYN_RCVD
5. ESTABLISHED
6. FIN_WAIT_1
7. FIN_WAIT_2
8. TIME_WAIT
9. CLOSING
10. CLOSE_WAIT
11. LAST_ACK

## Q: 为什么说 tcp 是可靠连接 ? tcp 的可靠连接体现在哪几点?
三次握手: 保证双方正常建立连接
四次挥手: 保证双方正常断开连接
滑动窗口: 保证发送方发送的报文 在 接收方负载范围内
重传机制: 保证系统超载, 网络超载时报文能被正确接收
拥塞控制: 保证网络超载时, 发送方及时减少发送的报文, 避免进一步的超载

## Q: tcp 的 seq 号是如何确定
rfc   建议: 一个随着时间叠加的基准值 + 4元祖相关的随机值
linux 实现: 基准值每 64 纳秒加1 + md5sum(4元组, 随机值)

## Q: 一个连接有多少个参数唯一确定? (5个 = 协议簇 + 4元组)
1. 协议号 tcp/ udp
2. 本端 ip
3. 本端 端口
4. 对段 ip
5. 对段 端口
Note: tcp 和 udp 的4元组是分离的