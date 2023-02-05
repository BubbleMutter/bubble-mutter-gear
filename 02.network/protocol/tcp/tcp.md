[TOC]
tcp 的内容相当多; 内容间深度耦合; 先列举几个较大的切入点

# 1. 三次握手

# 2. 四次挥手

# 3. 滑动窗口

# 4. 重传机制

# 5. 拥塞控制
https://blog.csdn.net/qq_33436509/article/details/81979024
https://www.cnblogs.com/alifpga/p/7675850.html (后半部分)

# 6. 状态迁移

# 7. systcl 调参

# 99. references
1. 小林 coding (总结非常到位)
    1. https://www.cnblogs.com/xiaolincoding/p/12732052.html tcp的可靠连接
2. TCP/IP详解
    1. 卷1
    2. 卷2 实现
3. 图解 TCP/IP 竹下隆史
4. The TCP/IP Guide.Charles M. Kozierok.
5. tcp 那些事
    1. https://coolshell.cn/articles/11564.html
    2. https://coolshell.cn/articles/11609.html

https://segmentfault.com/a/1190000019292140 <  这个链接的底部有一系列的关联链接; 可以看看

# tcp pacing (send)
发送端结合 流控制 和 窗口控制 的发送控制机制
tcp pacing uses the tcp window to determine how much to send
but uses rates instead of acknowledgments to determine when to send.