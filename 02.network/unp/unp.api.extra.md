[TOC]
# unp api extra
+ `CLOSE_WAIT` state
    + sk-receive-buffer is not readable and `read()` returns EOF
    + if caused by FIN but not RST, `write()` is still allowed before `close()`/`shutdown(SHUT_WR)`
+ `SIGPIPE`
    + occurs when `write()` to a sockfd which has received RST without `close()`
    + occurs when `write()` to a closed fd, default action is crashing
    + redefine the sa_action for mostly experience
+ `sigaction.sa_flags |= SA_RESTART;`
    + Most syscall can restart transparently after being interrupted by signal with `SA_RESTART`
    + Making EINTR no longer return
    + But not all the syscall support this, handling EINTR is more practical
+ `send*(), write()`
    + errno `EMSGSIZE` = buffer size greater than `SO_SNDBUF`
    + errno `EPIPE`    = `SIGPIPE` is caught by sigaction or `SIG_IGN`
+ `recv*(), read()` on `SOCK_DGRAM`, never return `EOF`, never understand peer `close()`
    + `socket(AF_UNIX, SOCK_STREAM, 0)` to notify each other that connection is closed
    + `socketpair(AF_UNIX, SOCK_STREAM, 0)` to notify each other that peer is closed
+ `recvfrom()`
    + errno `EWOULDBLOCK` = with flag `NON_BLOCKED`, but no readable buffer in kernel
    + once `recv*()`, only one datagram can be retrieved from the SOCK_DGRAM (different from SOCK_STREAM)

# connect
```c++
/**
 * @e ETIMEOUT     = not any response
 * @e ECONNREFUSED = receive RST
 * @e EHOSTUNREACH = receive icmp-host-unreachable
 * @e EADDRINUSE   = re-`connect()` immediately after `EINTR` (fixme by `SO_REUSEADDR`)
 * @e EINPROGRESS  = `fnctl(O_NONBLOCK)` and connection is not ready. It's OK.
 */
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
+ udp after `connect()`
    + sockfd can `send()`/`recv()` like tcp
    + sockfd `recv()` datagrams from certain peer, igoring from others, which means received sip is restricted

# accept
```c++
/**
 * @e EWOULDBLOCK  accept() on O_NONBLOCK but 3-way-hand-shaking is not finished
 * @e ECONNABORTED receive RST before accept() but after finish 3-way-hand-shaking
 */
int accept(int sockfd, struct sockaddr *addr, socklen_t *ddrlen);
```

# close shutdown
```c++
/**
 * @details 1. decrements refcnt of sockfd
 *          2. terminates both r/w directions of sock when refcnt reach 0
 * @e EWOULDBLOCK when `SO_LINGER` `l_linger > 0`, pending data in sk-send-buffer is timeout to be ACKed
 */
int close(int fd);

/**
 * @param how SHUT_RD: do not send FIN, then read() will return error.
 *            SHUT_WR: send data in sk-send-buffer and FIN,
 *                     write() will return error, read() is still allowed.
 * @details usage 1. finishing write(), then shutdown(SHUT_WR), and read() wait response
 *          usage 2. read() return EOF, shutdown(SHUT_RD), and write() pending data
 */
int shutdown(int sockfd, int how);
```

# send/recv v.s. sendto/recvfrom
+ `send()` `recv()`
    + only available for `connect()` sock
    + errno `EISCONN` for un-`connect()` sock
+ `sendto()`/`recvfrom()`
    + available for un-`connect()` udp
    + for `connect()` udp use `read()`/`write()`/`send()`/`recv()` just like tcp
    + for tcp, they are available, however tcp doesn't use them usually
+ `sendmsg()/recvmsg()`
    + available for udp and tcp

# sendmsg recvmsg example
```c++
// recv example
int recvmsg_payload() {
    struct payload *payload; // custom type
    for (struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msghdr); cmsg != NULL;
         cmsg = CMSG_NXTHDR(&msghdr, cmsg)) {
        payload = CMSG_DATA(cmsg);
        // parse payload
    }
}

// send example
int sendmsg_payload() {
    struct payload *payload; // custom type
    struct msghdr msghdr = { /* ... */ };
    struct iovec iov[1] = {{
        .iov_base = "", /* for reader detecting EOF or empty */
        .iov_len = 1,
    }};
	union {
	  struct cmsghdr cmsghdr;
	  unsigned char msg_control[CMSG_SPACE(sizeof(payload))];
	} _msg;
	msghdr.msg_control = _msg.msg_control;
	msghdr.msg_controllen = sizeof(_msg);
	struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msghdr);
	cmsg->cmsg_len = CMSG_LEN(sizeof(payload));
	cmsg->cmsg_level = ...;
	cmsg->cmsg_type = ...;
    payload = CMSG_DATA(cmsg);
    // setup payload ...
    sendmsg(..., &msghdr, ...);
}
```
1. iov array should not be empty, use one-element to distinguish `EOF` or no data

# select
+ `select()` fd_set is value-result parameters and needed to be rebuilt before next recall
+ `select()` readfds condition
    + data in sk-receive-buffer is greater than or equal to `SO_RCVLOWAT` (default is 1)
    + active `shutdown(SHUT_RD)` then `read()` returns EOF
    + passive receives FIN(CLOSE_WAIT); then `read()` returns EOF
    + new connection of LISTEN sock is completed; then `accept()` returns sockfd
    + error occurs in sock; then `read()` returns -1, and set `errno`
+ `select()` writefds condition
    + space in sk-send-buffer is greater than or equal to `SO_SNDLOWAT` (default is 2048)
    + active `shutdown(SHUT_WR)` then `write()` returns EPIPE or makes `SIGPIPE`
    + non-blocking `connect()` is finished, succeed or fail is possible
    + error occurs in sock; then `write()` returns -1, and set `errno`
+ `select()` exceptfds condition
    + TCP OOB (out-of-band)
    + error occurs in sock; then `read()` or `write()` return -1, and set `errno`

# alarm()
```c++
unsigned int alarm(unsigned int seconds);
```
+ usual usage steps
    1. save `SIGALARM` signal handler
    2. reset `SIGALARM` signal handler
    3. `alarm(timeout)`
    4. call blocked syscall
    5. handle `ETIMEDOUT, EINTR`
    6. reset `alarm(0)`
    7. restore `SIGALARM` signal handler and return

# setsockopt SOL_SOCKET
+ `SO_DONTROUTE` = packet sending will bypass route table, and ifindex is recommended.
+ `SO_ERROR`     = get `struct sock { int sk_err; }`; once get then `sk_err` will be reset to 0;
+ `SO_KEEPALIVE` = send tcp-keepalive-probe after no data transition for a long time
    1. receive ACK = then everything is fine and keepalive-timer is re-started
    2. receive RST = sock will be closed. `ECONNRESET`
    3. receive icmp-host-unreachable. sock will be closed. `EHOSTUNREACH`
    4. receive nothing after timeout. sock will be closed. `ETIMEOUT`
       `/proc/sys/net/ipv4/tcp_keepalive_time`   = first waiting time (default 2 hour)
       `/proc/sys/net/ipv4/tcp_keepalive_probes` = the count of probing
       `/proc/sys/net/ipv4/tcp_keepalive_intvl`  = the interval of probing (default 75 second)
+ `SO_LINGER`    = more accurately `close()` with `struct linger { int l_onoff; int l_linger; }`
    1. `l_onoff=0` = `close()` return immediately and data in sk-send-buffer is to be sent without waiting ACKs
    2. `l_onoff=1` = `close()` return after data in sk-send-buffer is sent and ACKed; or `l_linger` timeout
+ `SO_OOBINLINE`  = out-of-band data will be placed in normal data
+ `SO_RCVBUF` / `SO_SNDBUF`
    1. `net/core/rmalarmitable for `select()` when space in sk-send-buffer is larger than `SO_SNDLOWAT` (tcp only)
    3. sockfd is always writable for `select()` if it is udp
+ `SO_REUSEADDR` = allow `bind()` the same ip:port, but only one that is not `SOCK_DEAD`, for quick restart
    + consider the other process's sock is dead but not clean, like in TIME_WAIT/LAST_ACK state
+ `SO_REUSEPORT` = allow `bind()` the same ip:port for load balance or advanced performance
+ `SO_BROADCAST` = enable broadcast sending and receiving
+ `SO_OOBINLINE`
    + for server, `setsockopt(listenfd, SO_OOBINLINE, ...)` instead of `acceptfd`
      otherwise, some OOB data, arriving between `3-way-hand-shaking-finish` and `accept()`, will be ignored

# setsockopt IPPROTO_IP
+ `IP_HDRINCL` = userspace build partial `struct iphdr` of packet, except
    1. `iphdr->check`; kernel or hardware will recalculate
    2. `iphdr->id`;    if zero, kernel will re-assign
    3. `iphdr->saddr`; if `INADDR_ANY`, kernel will re-assign
+ `IP_RECVIF`  = ifindex of the received packet
+ `IP_TOS`     = sending packet's tos in `struct iphdr`, defined via `#define IPTOS_xxxx`
+ `IP_TTL`     = sending packet's ttl in `struct iphdr`
+ `IP_HDRINCL` = indicates packet's data including `struct iphdr`
    + kernel still modify `struct iphdr { __be16 id; }` and `struct iphdr { __be16 check; }`
+ `IP_MULTICAST_IF` = specify netwowrk interface for outgoing multicasts
+ `IP_MULTICAST_LOOP` = enable loopback of outgoing multicasts
+ multicast
    + `MCAST_JOIN_GROUP`         = join ifindex to a multicast group
    + `MCAST_BLOCK_SOURCE`       = leave ifindex from a multicast group
    + `MCAST_UNBLOCK_SOURCE`     = block a source from a joined group (`(ifindex, ip)`)
    + `MCAST_LEAVE_GROUP`        = unblock a source in a joined group (`(ifindex, ip)`)
    + `MCAST_JOIN_SOURCE_GROUP`  = join a source-specific group
    + `MCAST_LEAVE_SOURCE_GROUP` = leave a source-specific group

# setsockopt IPRPOTO_IPV6
+ `IPv6_V6ONLY`         = disable sockfd ipv6 mapping
+ `IPV6_MULTICAST_IF`   = specify netwowrk interface for outgoing multicasts
+ `IPV6_MULTICAST_LOOP` = enable loopback of outgoing multicasts
+ `ICMP6_FILTER`        = filter particular icmp6 type via `struct icmp6_filter;`

# setsockopt IPPROTO_TCP
+ `TCP_MAXSEG`  = tcp mss (readonly, as it's announced by peer)
+ `TCP_NODELAY` = nonzero to disable tcp Nagle (default zero, means enable)
    + to send data immediately (may do harm to network) without Nagle