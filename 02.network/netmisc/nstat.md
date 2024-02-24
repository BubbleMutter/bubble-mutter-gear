[TOC]
# nstat -az
## Tcp Basic
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

## Tcp SYNCookies
TcpExtSyncookiesSent   = SYN cookies sent 二次握手时发送
TcpExtSyncookiesRecv   = SYN cookies recv 三次握手时接收
TcpExtSyncookiesFailed = SYN cookies failed (not update TcpExtSyncookiesRecv)

## Tcp TIME_WAIT
TcpExtTW                     = 进入 TIME_WAIT 状态的次数
TcpExtTWRecycled             = net.ipv4.tcp_tw_reuse 的 socket 数
TcpExtTWKilled               = net.ipv4.tcp_tw_recycle 的 socket 数 (deprecated)
TcpExtTCPTimeWaitOverflow

## Tcp Timeouts
TcpExtTCPTimeouts     = congestion control 超时重传次数 (RTO timer)
TcpExtTCPSpuriousRTOs = F-RTO机制发现的虚假超时个数
TcpExtTCPLossProbes = (PTO timer) (Tail Loss Probe) 重传次数
TcpExtTCPLossProbeRecovery = (PTO timer) 修复丢包个数 (不小于 TcpExtTCPLossProbes)
TcpExtTCPRenoRecovery = congestion control 使用 reno 算法, 进入快速恢复的次数 (没使用 SACK)
TcpExtTCPRenoRecoveryFail
TcpExtTCPSackRecovery  = 拥塞控制 进入 恢复状态; 使用 Sack 的次数
TcpExtTCPSACKReneging  = SACK 确认数据包后, receiver drop 数据包, sender 重传的次数

## Tcp Retransmit
TcpExtTCPFastRetrans      = congestion control 快速重传
TcpExtTCPForwardRetrans   =
TcpExtTCPSlowStartRetrans = congestion control 慢启动重传
TcpExtTCPRetransFail      = FastRetrans/ForwardRetrans/SlowStartRetrans 重传失败数加和
TcpExtTCPLostRetransmit   = 重传包丢失数

## Tcp Reorder
TcpExtTCPFACKReorder = FACK 检测到 重排数据包计数
TcpExtTCPSACKReorder = SACK 检测到 重排数据包计数; 如已发 1~5, 收到 2,5 DSACK, 少了中间的3,4
TcpExtTCPRenoReorder = Reno 快速重传机制 检测到 排数据包计数 (仅当禁用SACK时才使用)
TcpExtTCPTSReorder   = TCP Timestamp 检测到 排数据包计数

## TCP Abort
TcpExtTCPAbortOnData    =
TcpExtTCPAbortOnClose   = 用户态 close/shutdown 次数
TcpExtTCPAbortOnMemory  = 内存不足导致 abort
TcpExtTCPAbortOnTimeout = RTO,PTO,Keepalive 重传超出限制后 abort 次数
TcpExtTCPAbortOnLinger
TcpExtTCPAbortFailed    = abort 会走发送 RST 报文流程, 记录RST报文发送失败失败的次数

## TCP Memory Issues
TcpExtTCPMemoryPressures = 从 memory 非压力水位进入到 压力水位 的次数
TcpExtPruneCalled        = memory 不足而进行 prune 的次数 (prune 耗CPU, 用算力换内存)
TcpExtRcvPruned          = rcv_buf 不足导致 tcp 内存回收, 回收失败次数
TcpExtOfoPruned          = 清理结构 tcp_sock 的乱序队列的次数

## Tcp Listen
TcpExtListenOverflows = `listen()` 队列中, 完成三次握手的sock超过上限 (一般是进程来不及 `accept()`)
TcpExtListenDrops     = listen() 丢包; (包括 TcpExtListenOverflows /  创建 二三次握手 sk 失败)

## Tcp PAWS (Protect Against Wrapping Sequence)
TcpExtPAWSActive  = active端, 接收握手的SYN+ACK, PAWS 检查失败
TcpExtPAWSEstab   = Established 状态下, PAWS 检查失败
TcpExtPAWSPassive = passive端, 收到握手的ACK, PAWS 检查失败

## Tcp HP (High Path)
TcpExtTCPHPHits       = High Path 中处理了带数据的报文次数
TcpExtTCPHPAcks       = High Path 中处理了纯 ack 的报文次数
TcpExtTCPPureAcks     = Slow Path 中处理了纯 ack 的报文次数
TcpExtTCPHPHitsToUser =

## Tcp Other
TcpExtEmbryonicRsts    = 在 SYN_RECV 状态收到带 RST/SYN 标记的包个数
TcpExtOutOfWindowIcmps = 处理 ICMP_DEST_UNREACH / ICMP_TIME_EXCEED / ICMP_QUENCH 时, 超出 tcp 窗口的次数
TcpExtLockDroppedIcmps = 处理 ICMP_DEST_UNREACH / ICMP_TIME_EXCEED / ICMP_QUENCH 时, sock 被 lock 的次数
TcpExtArpFilter        = arp 报文 match 网卡设备上的 arp_filter 的次数

## Tcp DelayACK
TcpExtDelayedACKs      = 发送 DelayedACK 的次数，包括未成功发送的次数
TcpExtDelayedACKLocked = 发送 DelayedACK 时, sock 被 lock 导致丢包的次数
TcpExtDelayedACKLost   = 发送 DelayedACK 丢包的次数
TcpExtTCPSchedulerFailed

## Tcp Fast Open
TcpExtTCPFastOpenActive          = sent fast open syn
TcpExtTCPFastOpenActiveFail      =
TcpExtTCPFastOpenPassive         = recv fast open syn
TcpExtTCPFastOpenPassiveFail     =
TcpExtTCPFastOpenListenOverflow  = recv fast open syn, but listen overflow
TcpExtTCPFastOpenCookieReqd
TcpExtTCPFastOpenBlackhole
TcpExtTCPFastOpenPassiveAltKey

## Tcp DSACK
TcpExtTCPDSACKUndo
TcpExtTCPDSACKOldSent
TcpExtTCPDSACKOfoSent
TcpExtTCPDSACKRecv
TcpExtTCPDSACKOfoRecv
TcpExtTCPDSACKIgnoredOld
TcpExtTCPDSACKIgnoredNoUndo