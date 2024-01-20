[TOC]
# TCP
# 1. 三次握手

# 2. 四次挥手

# 3. 滑动窗口

# 4. 重传机制

# 5. 拥塞控制
https://blog.csdn.net/qq_33436509/article/details/81979024
https://www.cnblogs.com/alifpga/p/7675850.html (后半部分)

# 6. 状态迁移

# 7. systcl 调参

# 8. implement framework
`tcp.c` = things between userspace and kernelspace
`tcp_input.c` =
`tcp_output.c` =
`tcp_timer.c`  = timer handle
`tcp_ipv4.c`   = Congestion control handler (includes tcp reno)
`tcp_[veno|vegas|...].c` = Congestion control algorithms (veno/vegas/...)

# 99. references
+ 小林 coding (总结非常到位)
    1. https://www.cnblogs.com/xiaolincoding/p/12732052.html tcp的可靠连接
+ TCP/IP详解
    1. 卷1
    2. 卷2 实现
+ 图解 TCP/IP 竹下隆史
+ The TCP/IP Guide.Charles M. Kozierok.
+ tcp 那些事
    1. https://coolshell.cn/articles/11564.html
    2. https://coolshell.cn/articles/11609.html
+ https://segmentfault.com/a/1190000019292140
   底部有一系列的关联链接; 可以看看
+ https://wiki.aalto.fi/download/attachments/70789052/linux-tcp-review.pdf
+ TCP/IP Volume 2 = implement on BSD

# tcp pacing (send)
发送端结合 流控制 和 窗口控制 的发送控制机制
tcp pacing uses the tcp window to determine how much to send
but uses rates instead of acknowledgments to determine when to send.