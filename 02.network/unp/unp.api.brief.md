[TOC]
# unp api type
```c++
int shutdown(int sockfd, int how /* SHUT_WR, SHUT_RD, SHUT_RDWR */);
```

# misc = signal / block
```c++
// Set of signals to be blocked during execution of the signal handling function.
sigaction.sa_mask;
retval = sigemptyset(&sigaction.sa_mask);
retval = sigemptyset(&sigaction.sa_mask, signum /* wanted to be blocked */);
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

ioctl(sockfd, FIONBIO, ...);  // set sockfd to nonblocking or not
ioctl(sockfd, FIOASYNC, ...); // set sockfd to asynchronous or not
ioctl(sockfd, FIONREAD, ...); // get unread byte count in sk-receive-buffer
// example
val = fcntl(sockfd, F_GETFL, 0);
fcntl(sockfd, F_SETFL, val | O_NONBLOCK);
```

# getsockopt / setsockopt
```c++
/**
 * @param level SOL_SOCKET/IPPROTO_IP/IPPROTO_TCP/IPPROTO_UDP/IPPROTO_IPV6/...
 */
int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```

# inet_pton/inet_ntop/getsockname/getpeername
```c++
// listen on any address
sockaddr_in6.sin6_addr = IN6ADDR_ANY_INIT;
sockaddr_in.sin_addr   = INADDR_ANY;
#define INADDR_BROADCAST /* 255.255.255.255 */
#define INADDR_LOOPBACK  /* 127.0.0.1 */

// check ipv6 address is mapped-ipv4 or not
// for a no IPV6_ONLY server, check resulted-addr returned by getpeername()
#define IN6_IS_ADDR_V4MAPPED(a)

// ip from string to integer, set into sockaddr_in.sin_addr / sockaddr_in6.in6_addr
int inet_pton(int af, const char *src, void *dst);
#define INET_ADDRSTRLEN
#define INET6_ADDRSTRLEN
// ip from integer to string, dst[INET_ADDRSTRLEN] or dst[INET6_ADDRSTRLEN]
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);

struct sockaddr_storage; // 可作为统一传入参数
// 传出根据 sockaddr_storage.ss_family cast sockaddr_in/sockaddr_in6/sockaddr_un/...
// client retrieves local address:port after connect()
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// server retrieves peer address:port after accept()
int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

# socket/bind/listen/accept/connect
```c++
int socket(int domain /* AF_INET, AF_INET6, AF_UNIX */,
    int type /* SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET */,
    int protocol /* 0, IPPROTO_SCTP */);

// client
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// server
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t * addrlen);
```

# select/poll
```c++
#define FD_SETSIZE        1024
struct fd_set fd_set;
void FD_CLR(int fd, fd_set *set);   // del fd from fd_set
int  FD_ISSET(int fd, fd_set *set); // test fd in fd_set
void FD_SET(int fd, fd_set *set);   // add fd into fd_set
void FD_ZERO(fd_set *set);          // zero all in fd_set
int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);

struct pollfd {
    int   fd;         /* file descriptor */
    short events;     /* requested events */
    short revents;    /* returned events */
};
int poll(struct pollfd *fds, nfds_t nfds, int timeout /* millisecond */);
```

# /etc/hosts (dns) = getaddrinfo
```c++
struct addrinfo {
    int              ai_flags;     // 0, AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST, AI_V4MAPPED, AI_ALL
    int              ai_family;    // 0, AF_INET, AF_INET6
    int              ai_socktype;  // 0, SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;  // 0, IPPROTO_TCP, IPPROTO_UDP
    socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};
int getaddrinfo(const char *domain, const char *service /* /etc/services or digit */,
                const struct addrinfo *hints, struct addrinfo **res);
sockfd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
void freeaddrinfo(struct addrinfo *res /* getaddrinfo() res parameter */);
const char *gai_strerror(int errcode /* getaddrinfo() return */);

// reverse dns query
#define NI_NAMEREQD // return error if the hostname cannot be determined.
#define NI_DGRAM  // UDP service only
#define NI_NOFQDN //  return only the hostname part of the fully qualified domain
#define NI_NUMERICHOST // dotted-decimal ip address
#define NI_NUMERICSERV // numeric service, sport
int getnameinfo(const struct sockaddr *addr, socklen_t addrlen,
                char *host /* domain */, socklen_t hostlen,
                char *serv /* /etc/services */, socklen_t servlen,
                int flags /* NI_* */);
```

# /etc/networks (subnet) = getnetbyname
```c++
struct netent *getnetent(void);
struct netent *getnetbyname(const char *name);
struct netent *getnetbyaddr(uint32_t net, int type);
void setnetent(int stayopen);
void endnetent(void);
```

# /etc/protocols (l3layer) = getprotobyname
```c++
struct protoent *getprotoent(void); // traverse /etc/protocols
struct protoent *getprotobyname(const char *name);
struct protoent *getprotobynumber(int proto);
void setprotoent(int stayopen);
void endprotoent(void);
```

# /etc/services (l4layer) = getservbyname
```c++
struct servent *getservent(void); // traverse /etc/services
struct servent *getservbyname(const char *name /* ftp/telnet/... */,
                              const char *proto /* tcp/udp/NULL */);
struct servent *getservbyport(int port /* shoould be htons() */,
                              const char *proto /* tcp/udp/NULL */);
void setservent(int stayopen /* fd to file like /etc/services */);
void endservent(void);
```

# syslog
```c++
/**
 * @param ident indicates the log file, define in /etc/*syslog.conf
 *        kern.* -/var/log/kern.log
 *        user.* -/var/log/user.log
 * @param option LOG_CONS/LOG_NDELAY/LOG_PERROR/LOG_PID
 */
void openlog(const char *ident,
             int option,
             int facility /* LOG_AUTH/LOG_KERN/LOG_USER/LOG_SYSLOG/... */);
/**
 * @brief fprintf message to /var/log/xxx.log
 * @param format is equivalent the `format' parameter in printf()
 */
void syslog(int priority /* LOG_DEBUG/LOG_INFO/LOG_WARNING/LOG_ERR */,
            const char *format, ...);
void closelog(void);
int setlogmask(int mask /* LOG_MASK(LOG_DEBUG) | LOG_MASK(LOG_INFO) | ... */);
```

# send/recv/sendto/recvfrom/sendmsg/recvmsg
```c++
#define MSG_DONTROUTE // bypass routing table lookup
#define MSG_DONTWAIT  // nonblocking
#define MSG_PEEK      // peek at incoming data and leaving them into sk-receive-data
#define MSG_OOB       // process out-of-band data
#define MSG_WAITALL   // wait for a full request

ssize_t send(int sockfd, const void buf[], size_t len, int flags /* MSG_* */);
ssize_t recv(int sockfd, void buf[], size_t len, int flags /* MSG_* */);

ssize_t sendto(int sockfd, const void buf[], size_t len, int flags /* MSG_* */,
               const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recvfrom(int sockfd, void buf[], size_t len, int flags /* MSG_* */,
                 struct sockaddr *src_addr, socklen_t *addrlen);
```

# readv/writev/sendmsg/recvmsg
```c++
struct iovec {
    void *iov_base;
    size_t iov_len;
};
ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
ssize_t writev(int fd, const struct iovec *iov, int iovcnt);

struct msghdr{
    void *msg_name;         /* Address (struct sockaddr_in/sockaddr_in6/sockaddr_un/... *)  */
    socklen_t msg_namelen;  /* Length of address data.  */
    struct iovec *msg_iov;  /* Vector of iovec */
    size_t msg_iovlen;      /* Number of iovec */
    void *msg_control;      /* Ancillary data (struct cmsghdr *). */
    size_t msg_controllen;  /* Ancillary data length */
    int msg_flags;    /* Flags MSG_DONTROUTE/MSG_DONTWAIT/... */
};
ssize_t sendmsg(int sockfd, const struct msghdr *msghdr, int flags);
ssize_t recvmsg(int sockfd, struct msghdr *msghdr, int flags);

struct cmsghdr {
    size_t cmsg_len;    // cmsghdr byte count, including header
    int    cmsg_level;  // SOL_SOCKET/IPPROTO_IP/IPPROTO_IPV6
    int    cmsg_type;   /* protocol-dependent type */
    unsigned char cmsg_data[];
};
struct cmsghdr *CMSG_FIRSTHDR(struct msghdr *msghdr);
struct cmsghdr *CMSG_NXTHDR(struct msghdr *msghdr, struct cmsghdr *cmsg);
size_t CMSG_SPACE(size_t length); // msghdr.msg_controllen
size_t CMSG_LEN(size_t length);   // cmsghdr.cmsg_len
unsigned char *CMSG_DATA(struct cmsghdr *cmsg);
```
