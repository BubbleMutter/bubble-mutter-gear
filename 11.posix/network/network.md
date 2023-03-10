[TOC]
# api
socket bind listen accept connect 
send recv
sendmsg recvmsg
readv writev
select pselect poll epoll
setsockopt getsockopt

#### multiplex (select/poll/epoll 同步IO)
+ select 有fd限制
+ poll 无fd限制, 但内核层依然需要 轮询fd 去mask readfds/writefds
    + 使得用户层也要 轮询地 FD_ISSET(fd, readfds)
+ epoll 去除轮训的弊端
+ select/poll 必须每次调用前合理计算准确的 nfds (最大的fd加1)
+ epoll_create 不同于nfds 传入一个要监听多少个fd

#### 常规流程
+ server 前摇
    1. socket()    注册fd
    2. struct sockaddr_in srvaddr, cliaddr 监听地址 和 接收连接的地址
    3. bind(sockfd, srvaddr)       绑定 fd和监听地址
    4. listen()                    注册监听事件
    5. accept(listenfd, cliaddr)   阻塞等待事件响应 成功时返回通信的socket
    6. fork/threads 处理
+ TCP-SRV select (fd_set/FD_CLR/FD_SET/FD_ZERO/FD_ISSET)
    1. socket sockaddr_in bind listen
    2. 把socket返回的fd 放到 readfds中 `FD_SET(listenfd, readfds);`
    3. select
    4. select返回 检查 listenfd `FS_ISSET(listenfd, readfds)` 则accept
    5. 把accept正确返回的fd放到 readfds/writefds 中
    6. select返回 非listenfd则作客户端处理
+ TCP-SRV poll (struct pollfd  )
    1. socket sockaddr_in bind listen
    2. 构造 struct pollfd 数组, 把listenfd放进去,  注册 POLLIN事件
    3. poll
    4. poll返回 检测 listenfd的(revents & POLLIN) 则accept
    5. 把accept正确返回的fd放到 struct pollfd数组, 注册 POLLIN事件
    6. poll返回 非listenfd则作为客户端处理
+ TCP-SRV epoll (struct epoll_event)
    1. socket sockaddr_in bind listen
    2. epoll_create, epoll_ctl添加listenfd EPOLLIN事件
    3. epoll_wait
    4. epoll_wait返回 遍历队列 struct epoll_event
    5. `epoll_event[i].data.fd == listenfd 且 events & EPOLLIN` accept
    6. 把accept正确返回的fd epoll_ctl添加到 epfd中
    7. `epoll_event[i].data.fd` 非listenfd则作为客户端处理
+ TCP-CLI
    1. sokect()  注册fd
    2. struct sockaddr_in srvaddr 需要连接的远程地址
    3. connect(fd, srvaddr)       连接

#### 多路服用的场合
+ 如果一个TCP服务器既要处理监听套接口，又要处理已连接套接口
+ 如果一个服务器即要处理TCP，又要处理UDP
+ 如果一个服务器要处理多个服务或多个协议
+ 当客户处理多个描述字时（一般是交互式输入和网络套接口）
+ 当一个客户同时处理多个套接口时，而这种情况是可能的，但很少出现

#### poll 事件
+ POLLIN/POLLOUT
+ POLLPRI urgent-data事件
+ POLLRDBAND Priority band data 事件
+ POLLERR fd发生错误
+ POLLHUP fd挂断
+ POLLNVAL fd已经关闭

## epoll
+ epoll_create/epoll_ctl/epoll_wait
#### struct epoll_event 的 events值
+ EPOLLIN/EPOLLOUT
+ EPOLLPRI urget-data 数据
+ EPOLLERR fd发生错误
+ EPOLLHUP fd挂断
+ EPOLLET  (epoll特有)设置为边缘触发
+ EPOLLONESHOT (epoll特有)只监听一次 监听后退出队列
#### epoll_ctl 的 op 增/删/改
+ EPOLL_CTL_ADD
+ EPOLL_CTL_MOD
+ EPOLL_CTL_DEL
#### 监听结果在 epoll_events[]中
+ 获取事件fd `m_epoll_events[i].data.fd`
#### helper 封装CTL_ADD/CTL_DEL/CTL_MOD 以及 struct epoll_event
+ epoll_add_events(int epfd, int fd, int events)
+ epoll_del_events(int epfd, int fd, int events)
+ epoll_mod_events(int epfd, int fd, int events)
#### EPOLLET 边缘触发 超级重点
+ LT（level trigger）和ET（edge trigger）。LT模式是默认模式，LT模式与ET模式的区别如下：
+ LT模式：当epoll_wait检测到描述符事件发生并将此事件通知应用程序，应用程序可以不立即处理该事件。下次调用epoll_wait时，会再次响应应用程序并通知此事件。
+ ET模式：当epoll_wait检测到描述符事件发生并将此事件通知应用程序，应用程序必须立即处理该事件。如果不处理，下次调用epoll_wait时，不会再次响应应用程序并通知此事件。
+ ET模式在很大程度上减少了epoll事件被重复触发的次数，因此效率要比LT模式高。epoll工作在ET模式的时候，必须使用非阻塞套接口，以避免由于一个文件句柄的阻塞读/阻塞写操作把处理多个文件描述符的任务饿死。

#### epoll/select/poll 总结
+ epoll针对select/poll的痛点进行的修改，也就是高效之处总结
+ select/poll把fd的监听列表放在用户空间，由用户空间管理，导致在用户空间和内核空间之间频繁重复拷贝大量fd；epoll在内核建立fd监听列表（实际是红黑树），每次通过epoll_ctl增删改即可。
+ select/poll每当有fd内核事件时，都唤醒当前进程，然后遍历监听列表全部fd，检查所有就绪fd并返回；epoll在有fd内核事件时，通过回调把该fd放到就绪队列中，只需返回该就绪队列即可，不需要每次遍历全部监听fd