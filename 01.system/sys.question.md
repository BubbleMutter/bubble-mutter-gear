## Q: 进程和线程有什么不同 (执行 /  资源(地址空间) / 通讯 / 健壮性)
1. 进程是资源分配的最小单元; 线程是程序执行的最小单元
2. 进程有自己的独立地址空间; 而线程是共享进程中的地址空间
    1. 一系列的页表(task_struct->mm_struct 链表) 维护代码段 堆栈段
    2. 分配页表的操作是昂贵的(建立 MMU的映射关系; 内存分配算法)
    3. 因此; CPU切换线程的开销 远比 进程的开销要小很多
3. 线程间通讯比进程间通讯更快速; 因为线程间共享 全局变量, 静态变量
   进程间通讯则需要更昂贵的代价: 比如网络、共享内存、信号量、消息队列
4. 多进程比多线程更健壮
    1. 因为一个线程挂掉了, 会导致线程都挂掉(共享地址空间)
    2. 反之一个进程挂掉了, 并不会对另一个进程造成很大影响

## Q: 线程分别共享了哪些资源 (6个)
1. 文件系统
2. 文件描述符的表
3. 地址空间
4. 信号回调
5. pending的信号队列
    + 除了共享的; 每个线程分别各自的 pending 信号队列
6. 堆内存; 环境变量

## Q: linux 进程的内存空间分布 (/proc/$pid/maps 从低到高) (6个)
1. 代码段 elf .text segment
2. 数据段 elf .bss .data .rodata
    + 数据段与brk不是直接相邻的;      brk-offset
3. Heap 堆内存;       往高地址增长
    + Heap 和 mmap 不是直接相邻的
4. mmap 的动态链接库; 往高地址增长 （往栈底方向增长)
    + mmap域与栈不是直接相邻的;       mmap-offset
5. Stack 栈内存; 往低地址增长 (`%esp 指针`)
    + Stack 和 Kernel 不是直接相邻的; stack-offset
6. Kernal Space: 可在编译时调整; 默认是1G

## Q: 内存溢出　内存泄漏　内存越界的区别? 什么是野指针
1. 内存泄漏: memory-leak; 程序申请内存后; 没有释放已经申请的内存空间
2. 内存越界: 程序访问超出既定访问的内存数据
3. 内存溢出: out-of-memory; 用户态程序向内核态申请内存时; 没有足够的存储空间　(OOM)

## Q: 进程中堆栈内存的区别
| 区别点   | 栈  |  堆 |
| :-:      | :-: | :-: |
| 管理方式 | 编译器自动管理 | 动态申请; 需要手动释放 |
| 空间大小 | 连续的 往低地址增长 | 不连续 往高地址增长(内存管理系统 用链表进行管理) |
| 碎片问题 | 没有 | 频繁申请和释放内存 会造成大量碎片 |
| 增长方式 | 往低地址增长 | 往高地址增长 |
| 分配方式 | fork阶段完成后; 运行过程中编译器完成 | 动态分配 |
| 分配效率 | 若干个指令的消耗 | 根据复杂的分配机制完成; 当碎片化严重时需要重新整理内存空间；毫秒级耗时 |

## Q: fork 做了啥

## Q: vfork 做了啥

## Q: clone 做了啥

## Q: malloc 做了啥

## Q: 内核内存伙伴算法; 内存分配; slabinfo

## Q: sleep 和 wait 有啥区别

## Q: 常见内存错误类型: double-free, 非法访问, 栈溢出, 堆溢出, 野指针
## Q: 如何避免内存碎片
## Q: 什么叫野指针; 踩了野指针会有什么影响?

## Q: epoll poll select 的区别
https://blog.csdn.net/qq_33436509/article/details/81946968
1. fd个数; select 最大1024; epoll 和 poll 没有限制
    + 怎么修改??
2. fd拷贝到内核: poll 和 select 每次等待都要把fd集合拷贝到内核
    + epoll 则在 epoll_ctrl 一次拷贝; epoll_wait多次等待
3. fd传入(要监听的)与传出(已发生的)事件是否分离:
    1. select 没有分离; 传入传出事件共用一个变量 fd_set; 下一次监听需要重新赋值
    2. poll   事件分离；传入pollfd->events要监听的; 传出pollfd->revents已发生的
    3. epoll_wait 已分离; 不需要再次传递fd; 传出的就是有事件的fd
4. fd 是否有事件
    1. select 每次返回都要遍历一整个数组 检查fd_set 是否有事件发生
    2. poll   每次返回都要遍历一整个数组 检查revents是否有事件发生
    3. epoll  返回的就是有事件的fd集合 不需要做额外的检查

## close 和 shutdown 的区别
https://blog.csdn.net/justlinux2010/article/details/20913755
1. close 是文件系统层的关闭
    + 单个进程close一个fd后不保证马上释放连接
    + 当fd在内核中的引用数为0才会释放连接
2. shutdown 直接对socket层的关闭 (不处理引用计数) (不能shutdown非socket的fd)
    + 调用后; 其他进程占有这个连接的fd; read返回EOF; write可能触发SIGPIPE(buffer满了)
    + 对于处于 TCP_CLOSE 状态的socket 会返回 ENOTCONN错误
3. shutdown 可以选择性 只关闭读 or 只关闭写 or 同时关闭读写
    + 但 close 必须同时关闭读写方向
4. shutdown只关闭socket不释放fd; 所以进程也要调用close释放fd
    + 但 close 释放fd 释放socket

## Q: shutdown 关闭读写方向的区别
1. shutdown(SHUT_RD) 读方向
    1. 丢弃接收缓冲区的数据；后续再收到新的数据，会对数据进行 ACK，然后悄悄地丢弃
    2. 即对端还是会接收到 ACK，根本不知道数据已经被丢弃
    + 因为这样对端连接才能维持, 然后本端才能继续发送, 写方向没有关闭的
2. shutdown(SHUT_WR) 写方向 称为"半关闭连接"
    1. 发送发送缓存区的数据; 并发送一个 FIN 报文给对端; 本地进程write/send 将报错
3. shutdown(SHUT_RDWR)：相当于 SHUT_RD 和 SHUT_WR 操作各一次，关闭读和写两个方向
