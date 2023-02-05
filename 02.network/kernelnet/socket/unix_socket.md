[TOC]
man 7 unix

```c++
// type is one of 
// SOCK_DGRAM     ==> unix_dgram_ops
// SOCK_STREAM    ==> unix_stream_ops
// SOCK_SEQPACKET ==> unix_seqpacket_ops
int unix_socket = socket(AF_UNIX, type, 0);
int error = socketpair(AF_UNIX, type, 0, int *sv);

struct sockaddr_un {
    sa_family_t sun_family;               /* AF_UNIX */
    char        sun_path[108];            /* pathname */
};
```

# 创建 socket(AF_UNIX, type, 0)
unix_create > unix_create1
    `sk->sk_max_ack_backlog	= net->unx.sysctl_max_dgram_qlen`

unix-socket 的收包缓存大小, 不能通过 setsockopt 设置 (4.18.76)
必须通过 `/proc/sys/net/core/rmem_*`, `/proc/sys/net/core/wmem_*` 设置