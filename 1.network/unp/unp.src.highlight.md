[TOC]
# unix network programming code
+ `https://github.com/unpbook/unpv13e`

# unp source code highlight example (just use them)
+ `setsockopt()` = `unpv13e/sock/*.c`
+ `SIGPIPE` = `unpv13e/tcpcliserv/tsigpipe.c`
+ `SIGCHLD` = `unpv13e/server/sig_chld_waitpid.c`
+ `connect()` udp = `unpv13e/udpcliserv/dgcliconnect.c`

+ un-`connect()` udp client query all the ip address of a domain trickly
    + `recvfrom()` then `getnameinfo()` then `getaddrinfo()`
+ `gethostbyname()` = `unpv13e/names/hostent.c` (@deprecated)
    + `gethostbyname()`/`gethostbyaddr()`/`h_errno` are deprecated
    + use `getnameinfo()`/`getaddrinfo()` instead
+ `getaddrinfo()`
    + `unpv13e/lib/tcp_connect.c, unpv13e/lib/tcp_listen.c`
    + `unpv13e/lib/udp_connect.c, unpv13e/lib/udp_server.c`
    + `unpv13e/lib/host_serv.c`

+ `poll()` = `unpv13e/tcpcliserv/tcpservpoll01.c`
+ `select()` = `unpv13e/tcpcliserv/tcpservselect01.c` / `unpv13e/udpcliserv/udpservselect01.c`
+ easier approach of non-blocking io = `unpv13e/nonblock/strclifork.c`
    + really easier than `select()`
    + but with slightly performance drawback
    + `select()` 代码复杂度太高, non-blocking 程序需要在 代码复杂度 和 性能间取舍
+ non-blocked simultaneous `connect()` = `unpv13e/nonblock/web.c`
+ `pselect()` = `unpv13e/bcast/dgclibcast4.c`
    + 用以解决 SIGALARM的丢失 问题
    + 即 `unpv13e/bcast/dgclibcast.3.c` 中 `SIG_UNBLOCK > wait > SIG_BLOCK` 3个steps 并非原子化的问题
    + 然而该例子中, 使用 SIGALARM 目的是实现 `recvmsg()` 等待超时的机制
    + 实际上只使用 `select(指定超时时间), recvmsg()` 即可; 不需要 `SIGALARM, pselect()`

+ unix-domain = `unpv13e/unixdomain/unixstrserv01.c` / `unpv13e/unixdomain/unixstrcli01.c`
+ send fd = `unpv13e/lib/read_fd.c`/`unpv13e/lib/write_fd.c`
    + nginx core process transmits sockfd, returned from `accept()`, to worker processes
+ send user credential = `unpv13e/unixdomain/readcred.c`

+ multicast setsockopt `setsockopt(SOL_IP, MCAST_*)` = `unpv13e/lib/mcast_join.c`, `unpv13e/lib/mcast_leave.c`
+ multicast send recv = `unpv13e/mcast/main.c, unpv13e/mcast/recv.c, unpv13e/mcast/send.c`
+ out-of-band
    + `SIGURG`            = `unpv13e/oob/tcprecv01.c`
    + `select(exceptfds)` = `unpv13e/oob/tcprecv03.c` (notice the tricky exceptfds in `select()`)
+ `IPOPT_LSRR, IPOPT_SSRR` = `unpv13e/ipopts/sourceroute.c`
    + `IPOPT_LSRR` = loose source route = packets can pass through listed node or else
    + `IPOPT_SSRR` = strict source route = packets must pass through each listed node only

+ timeout io
    + `alarm()` and caught `SIGALRM`                      = `unpv13e/lib/connect_timeo.c`
    + `select()`/`poll()` to set timeout parameter        = `unpv13e/lib/readable_timeo.c`
    + `setsockopt(SO_RCVTIMEO)`/`setsockopt(SO_SNDTIMEO)` = `unpv13e/advio/dgclitimeo2.c`
+ timeout on multi iterative blocked io
    + use `SIGALARM` (has atomic issue) (incorrectly `unpv13e/bcast/dgclibcast3.c`)
    + use `SIGALARM` and `pselect()` (`unpv13e/bcast/dgclibcast4.c`)
    + use `SIGALARM`, `sigsetjmp()` and `siglongjmp()`; (`unpv13e/bcast/dgclibcast5.c`)
    + use `SIGALARM`, `select()`, and `pipe()`;
      `pipefd[]` for timeout notification (`unpv13e/bcast/dgclibcas6.c`)

+ signal driven io = `unpv13e/sigio/dgecho01.c`
    + note: there is race condition between `main()` and `SIGIO handler`
      So, in `main()`, modification of shared vars should be after blocked `SIGIO`,
      which means that `sigprocmask(SIG_BLOCK)` and `sigprocmask(SIG_SETMASK)`
    + note: As the non-queue signal mechanism, in `SIGIO handler`, `recv()` should execute util `EWOULDBLOCK`
+ threads io = `unpv13e/threads/web03.c`
    + `pthread_cond_signal(), pthread_cond_wait(), pthread_mutex_lock()`
+ per thread data = `unpv13e/threads/readline.c`
    + `pthread_once(), pthread_key_create(), pthread_getspecific(), pthread_setspecific()`
+ ping   = `socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)` = `unpv13e/ping/send_v4.c` / `unpv13e/ping/proc_v4.c`
+ libpcap = `unpv13e/udpcksum/pcap.c`
    + `pcap_open_live(), pcap_lookupnet(), pcap_compile(), pcap_setfilter(), pcap_datalink()`
    + `pcap_datalink(), pcap_next()`
+ libnet = `unpv13e/udpcksum/senddnsquery-libnet.c`