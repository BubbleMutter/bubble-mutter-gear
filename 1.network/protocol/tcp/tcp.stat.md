[TOC]
# tcp stat
```sh
cat /proc/net/snmp | grep Tcp:  | rs -T | column -t
cat /proc/net/snmp | grep Ip:   | rs -T | column -t
cat /proc/net/snmp | grep Icmp: | rs -T | column -t
cat /proc/net/snmp | grep Udp:  | rs -T | column -t
cat /proc/net/snmp6
cat /proc/net/netstat | grep TcpExt: | rs -T | column -t

netstat -st
# 静态显示所有字段
nstat -az
# 动态观察
watch -n 1 -d 'nstat -a | grep Tcp | grep -E xxx'
```

# nstat -az 字段解析
TcpActiveOpens = CLOSED > SYN_SENT
TcpPassiveOpens = LISTEN > SYN_RCVD
TcpAttemptFails = SYN_SENT/SYN_RCVD > CLOSED + SYN_RCVD > LISTEN
TcpEstabResets  = ESTABLISHED/CLOSE_WAIT  > CLOSED

TcpInSegs = Input Segments (include TcpInErrs)
TcpOutSegs = Output Segments (exclude TcpRetransSegs)
TcpRetransSegs = Retrans Segments 重传报文数 (包括 超时重传/快速重传)
TcpInErrs = Input Error Segments
TcpInCsumErrors = Input Checksum Error
TcpOutRsts = Output Reset (tcpflag::RST)

TcpExtSyncookiesSent   = SYN cookies sent 二次握手时发送
TcpExtSyncookiesRecv   = SYN cookies recv 三次握手时接收
TcpExtSyncookiesFailed = SYN cookies failed (not update TcpExtSyncookiesRecv)

TcpExtEmbryonicRsts    = 在 SYN_RECV 状态收到带 RST/SYN 标记的包个数
TcpExtOutOfWindowIcmps = 处理 ICMP_DEST_UNREACH / ICMP_TIME_EXCEED / ICMP_QUENCH 时, 超出 tcp 窗口的次数
TcpExtLockDroppedIcmps = 处理 ICMP_DEST_UNREACH / ICMP_TIME_EXCEED / ICMP_QUENCH 时, sock 被 lock 的次数
TcpExtArpFilter        = arp 报文 match 网卡设备上的 arp_filter 的次数

TcpExtTW               = 进入 TIME_WAIT 状态的次数
TcpExtTWRecycled       = TIME_WAIT 回收的次数  (/proc/sys/net/ipv4/tcp_tw_reuse)
TcpExtTWKilled         = 进入 TIME_WAIT 状态后, 触发 TWKilled 的次数

TcpExtPAWSActive       = active open 端接收 SYN+ACK包中 timestamp 错误, 而发送 RST 的次数
TcpExtPAWSEstab        = Established 状态下接收 timestamp 错误的次数

TcpExtDelayedACKs      = 发送 DelayedACK 的次数，包括未成功发送的次数
TcpExtDelayedACKLocked = 发送 DelayedACK 时, sock 被 lock 导致丢包的次数
TcpExtDelayedACKLost   = 发送 DelayedACK 丢包的次数

TcpExtRcvPruned        = rcv_buf 不足导致 tcp 内存回收, 回收失败次数

TcpExtListenOverflows  = `listen()` 队列中, 完成三次握手的sock超过上限 (一般是进程来不及 `accept()`)
TcpExtListenDrops      = listen() 丢包; (包括 TcpExtListenOverflows /  创建 二三次握手 sk 失败)

TcpExtTCPHPHits        = fast_path 中处理了带数据的报文次数
TcpExtTCPHPAcks        = fast_path 中处理了纯 ack 的报文次数
TcpExtTCPPureAcks      = slow_path 中处理了纯 ack 的报文次数

TcpExtTCPSackRecovery  = 拥塞控制 进入 恢复状态; 使用 Sack 的次数
TcpExtTCPRenoRecovery  = 拥塞控制 进入 恢复状态; 没有 Sack 的次数

TcpExtTCPSACKReneging = SACK 确认数据包后, receiver drop 数据包, sender 重传的次数
TcpExtTCPSACKReorder  = SACK 检测到重排数据包计数; 如已发 1~5, 收到 2,5 DSACK, 少了中间的3,4
TcpExtTCPRenoReorder  = 通过快速恢复检测到重新排序数据包。仅当禁用SACK时才使用

TcpExtTCPFastRetrans      = congestion control 快速重传次数
TcpExtTCPSlowStartRetrans = congestion control 慢启动次数
TcpExtTCPTimeouts         = congestion control 超时重传次数

TcpExtTCPMemoryPressures       = tcp_mem 内存压力??
TcpExtTCPMemoryPressuresChrono = tcp_mem 内存压力??

TcpExtPruneCalled     = rcv_buf 不足导致 tcp 内存回收的次数 ()
    tcp_data_queue()
    tcp_try_rmem_schedule() slow 接收路径时,
    如果不能拷贝到用户 socket, 则先缓存到 sk_receive_queue;
    如果 tcp_mem 内存不足则触发 prune

TcpExtOfoPruned      = 清理结构 tcp_sock 的乱序队列的次数
