[TOC]
# unix network programming (Awesome Book)

# TCP Server Model
1. iterative server (baseline implementation)
2. multi-process = `fork()` after each `accept()`; one process per connection
3. multi-thread  = `pthread_create()` after each `accept()`; one thread per connection
4. multiplex nonblocking io = `select()` + `O_NONBLOCK` + `shutdown()`
5. prealloc process-pool and one-accept = after `accept()`, main-process send sockfd to avaiable sub-process
    + `sendmsg()`/`recvmsg()` with `SCM_RIGHTS` to transfer fd via `socketpair()`
6. prealloc process-pool and all-accept = all process `flock(LOCK_EX); accept(listenfd); flock(LOCK_UN);`
7. prealloc thread-pool and one-accept = after `accept()`, main-thread send sockfd sub-thread avaiable sub-thread
8. prealloc thread-pool and all-accept = all thread `pthread_mutex_lock(); accept(); pthread_mutex_unlock();`

# TCP Client Model
1. iterative client (baseline implementation)
2. multiplex nonblocking io = `select()` + `O_NONBLOCK` + `shutdown()`
3. one connection per process
4. one connection per thread

# reading note
+ `read()` enough bytes on sockfd
    + `ioctl(sockfd, FIONREAD, ...);` = check byte count in sk-receive-buffer before actually read
    + `recv(sockfd, buf[size], size, MSG_WAITALL, ...)` = blocked util enough byte
    + `recv(sockfd, buf[size], size, MSG_PEEK|MSG_DONTWAIT, ...)` = slow way to check byte count in sk-receive-buffer
+ multiplexing = the capability for kernel notify userspace which fd is available inside a fdset
+ polling = a program keeps `read*()` on non-blocking fd in a loop
+ I/O Models
    + Blocking I/O Model      =
    + Nonblocking I/O Model   =
    + I/O Multiplexing Model  = `select()`/`pselect()`/`poll()`/`epoll()`
    + Signal-Driven I/O Model = SIGIO
    + Asynchronous I/O Model  =
+ blocking 4 categories
    + `read(), readv(), recv(), recvfrom(), recvmsg()` with no available data in sk-receive-buffer
    + `write(), writev(), send(), sendto(), sendmsg()` with no enough space in sk-send-buffer
    + `accept()` waiting incoming connection
    + `connect()` initiating outgoing connection
+ non-blocking `connect()`
    2. non-blocking `connect()` without `select()` 移植性相对较差 (`unpv13e/lib/connect_nonb.c`)
    3. So, it's highly recommended that `fcntl(O_NONBLOCK)` then `select()`
+ drawback of non-blocked simultaneous `connect()` to single destination = `unpv13e/nonblock/web.c`
    + a. It increases the load at any given time on the server as 3-hand-shaking consumes more resources.
    + b. There are pitfalls here if any congestion occurs.
    +    When multiple sockets are established from a client to a server,
    +    there is no communication between different sockets.
    +    If one sockets encounters a packet loss, the other sockets to the same server are unknown.
    +    But it is highly probable that the other sockets will encounter packet loss soon.
    +    Then all these sockets are sending more packets into an already congested network.
+ signal driven io
    + 4 steps: `signal(SIGIO, ...); fcntl(F_SETOWN); fcntl(O_ASYNC); fcntl(O_NONBLOCK);`
    + note: signal driven io is useless for TCP mostly, as TCP would generate `SIGIO` very frequently
+ broadcast packets will send one duplicates to sender itself via loopback
+ ping `SOCK_RAW`
    + `socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);`
    + `socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);`
    + `read()` buf include `struct iphdr`, and need to parse
    + `write()` doesn't need `struct iphdr` if `IP_HDRINCL` is not set (by default)
      Or, `setsockopt(IP_HDRINCL)` and `write()` custom `struct iphdr`
+ traceroute implementation
    + `socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);`
    + send icmp with incremental `iphdr->ttl`
    + recv icmp `ICMP_TIME_EXCEEDED` from each node on the path to destination, and extract sip
+ tcpdump implementation
    + `socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));`
    + `setsockopt(SO_ATTACH_FILTER)`
    + However, we use libpcap for more convinience

# TCP
+ tcp piggybacking = ack with psh in a tcp packet, happens when sent packet less than 200ms after received
+ tcp TIME_WAIT's necessity:
    1. reliability of connection termination
    2. ensuring expiration of old duplicated segments in network
+ tcp retransmission around termination
    + `FIN_WAIT_1` = resend FIN in timer then goto FIN_WAIT_2     (/proc/sys/net/ipv4/tcp_orphan_retries)
    + `FIN_WAIT_2` = goto TIME_WAIT                               (/proc/sys/net/ipv4/tcp_fin_timeout)
    + `CLOSING`    = resend FIN in timer then goto TIME_WAIT      (/proc/sys/net/ipv4/tcp_orphan_retries)
    + `TIME_WAIT`  = resend ACK when receive FIN then goto CLOSED
    + `LAST_ACK`   = resend FIN in timer then goto CLOSED         (/proc/sys/net/ipv4/tcp_orphan_retries)
+ `CLOSE_WAIT`
    + sk-receive-buffer is not readable and `read()` returns EOF
    + if caused by FIN but not RST, `write()` is still allowed before `close()`/`shutdown(SHUT_WR)`
+ tcp Nagle algorithm
+ tcp Delayed ACK algorithm

# UDP
+ udp server triangle-netflow when `bind(INADDR_ANY)`
    + 即 udp 服务器有多个网口, 可能出现 `sip of outgoing-server-response != dip of incoming-client-request`
+ UDP reliability
    1. add timeout and retransmission
    2. add sequence number for datagram
+ UDP concurrency
    1. server `fork()` and child process handle subsequent processing
    2. server `fork()`, `bind()` an ephemeral port, and notify client;
       client then communicate with server via the ephemeral port (TFTP use this strategy)

# UDP v.s. TCP
+ no 3-way-handshaking
+ no 4-way-hand?
+ no sliding-window
+ no congestion-control
+ no retransmission
+ no datagram merging
+ datagram fragments are implement on IP layer
    + the first part of a big datagram, encap by IP, has UDP header
    + the continuing part of a big datagram, encap by IP, has no UDP header
+ once `send*()`, the datagram is guarantee to be sent out without `TCP_DELAY`
    + However, there is no guarantee whether it can be received
+ once `recv*()`, only one datagram can be retrieved for `SOCK_DGRAM`
    + While `SOCK_STREAM`, once `recv*()`, multi-segments can be retrieved
+ size once `send*()` for SOCK_DGRAM is `min(SO_SNDBUF, iphdr.tot_len - sizeof(udphdr))`
    + While `SOCK_STREAM`, size once `send*()` is only limited by `SO_SNDBUF`

# IPv6 Header Control with cmsg
+ set sent without fragment `sendmsg(cmsg_type=IPV6_DONTFRAG)`
+ set sent srcip and ifindex `sendmsg(cmsg_type=IPV6_PKTINFO)`
    cmsg_data `struct in6_pktinfo { ipi6_addr; ipi6_ifindex; };`
+ set sent hop limit     `sendmsg(cmsg_type=IPV6_HOPLIMIT)`
+ set sent nexthop       `sendmsg()`
    cmsg_data `struct sockaddr_in6`
+ set sent traffic class `sendmsg(cmsg_type=IPV6_TCLASS)`
+ get recv dstip and ifindex `recvmsg(cmsg_type=IPV6_PKTINFO)` and `setsockopt(IPV6_RECVPKTINFO)`
    cmsg_data `struct in6_pktinfo { ipi6_addr; ipi6_ifindex; };`
+ get recv hop limit     `recvmsg(cmsg_type=IPV6_HOPLIMIT)` and `setsockopt(IPV6_RECVHOPLIMIT)`
    cmsg_data `struct sockaddr_in6`
+ get recv traffic class `recvmsg(cmsg_type=IPV6_HOPLIMIT)` and `setsockopt(IPV6_RECVTCLASS)`

# multicast
+ ipv4 multicast
    + `224.0.0.0 - 239.255.255.255`; group ID = lower 28bits
    + Mapped Ethernet `01:00:5e:00:00:00 - 01:00:5e:7f:ff:ff`; lower 23bits are equal, ignore 5bits
    + all-hosts group = `224.0.0.1`
    + all-routers group = `224.0.0.2`
+ ipv6 multicast
    + `ff00::0:0:0:0 - ff00::ffff:ffff:ffff:ffff`; group ID = lower 32bits
    + Mapped Ethernet `33:33:00:00:00:00 - 33:33:ff:ff:ff:ff`; lower 32bits are equal
    + all-hosts group = `ff01::1` at interface-local, `ff02::1` at link-local
    + all-routers group = `ff01::2` interface-local, `ff02::2` at link-local,  `ff05::2` at site-local
+ ipv4 multicast ethernet is not one to one mapped to ip (仅match group ID 的 23bits, 忽略剩下5bits)
    + 会出现 ethernet match, 收包到 ip layer doesn't match, 此时需要在协议栈 discard