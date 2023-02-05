[TOC]
# ipc
1. pipe
2. sem / shm / msq
3. signal
4. socket

# sem/shm/msq 两套实现接口
1. system v (XSI)
    1. sem(`#include <sys/sem.h>`): semget semctl semop
    2. shm(`#include <sys/shm.h>`): shmget shmctl shmop shmat shmdt
    3. msg(`#include <sys/msg.h>`): msgget msgctl msgsnd msgrcv
2. posix
    1. sem(`#include <semaphore.h>`): sem_open sem_unlink sem_close sem_wait sem_post sem_getvalue sem_init sem_destroy
    2. shm(`#include <sys/mman.h>`):  shm_open shm_unlink mmap munmap
    3. msg(`#include <mqueue.h>`): mq_open mq_unlink mq_close mq_send mq_receive mq_notify mq_getattr mq_setattr
3. xsi 接口无论是否存在竞争, 都需要进入内核态;
   posix 接口实现 futex 机制, 在不存在竞争时不进入内核态, 在性能上更优.

https://www.cnblogs.com/xiaolincoding/p/13402297.html

# 1. pipe 管道
1. 管道的本质是内核中一段缓存
    1. 写入时, 写入到内核缓存中
    2. 读取时, 从内核缓存中读取
2. 特性:
    1. 优势: 机制简单, 清晰易懂
    2. 劣势: 信方式效率低，不适合进程间频繁地交换数据
    3. 匿名管道: 没有实体文件, fork父子关系的进程间通讯 or sendmsg无关系进程间通讯
    4. 命名管道: 创建实体文件, 无关系进程间能通讯
4. 通讯方式: 先进先出; 不支持 lseek 文件定位; 支持多对一, 不支持一对多

# 2. msq  消息队列
1. 消息队列是保存在内核中的消息链表
    1. 在发送数据时，会分成一个一个独立的数据单元(message-body or data-block) 插入链表中
    2. 在接收数据后, 内核就会把这个消息体从链表中删除
    3. 消息链表 以及 未读数据 不会随着创建进程的销毁而销毁
    4. 通讯过程中, 读进程与内核间 / 写进程与内核间 存在数据拷贝的开销 copy_to_user / copy_from_user
2. 特性
    1. 优势: 高效率地 实现 进程间频繁交换数据
    2. 劣势: 通讯不及时; 数据单元大小有限制; 不适合大量数据传输

# 3. shm  共享内存
1. 机制描述
    1. 不同的进程分别选取一块 **虚拟内存** (地址可能不同) 出来
    2. 同时映射到同一块 **物理内存** 中
    3. 读写进程分别对该内存进行 直接读写 (取值/赋值/memcpy 等)

# 4. sem  信号量
1. sem 其实是内核中的一个整形计数器
    1. 实现方法a) shm and futex
    2. 实现方法b) 直接系统调用
2. 用于实现进程间的互斥与同步 (而不是缓存进程间通信的数据)
3. sem 有两种操作, up 和 down
4.  up  操作, `sem--`,
    1. 如果 `sem < 0`, 说明资源被占用, 阻塞等待直到 `sem >= 0`
5. down 操作, `sem++`,
    1. 如果 `sem == 0`, 表明有进程阻塞, 唤醒阻塞的进程
    2. 如果 `sem  > 0`, 表明无进程等待
6. 使用场景
    1. sem 的初始值, 设置为可并发次数 (使用 shm 的场景一般是1)
    2. 使用互斥资源前, 执行  up  操作
    3. 使用互斥资源后, 执行 down 操作
7. note:
    1. sem 可以用于解决 shm 读写冲突问题
    2. sem 实际上可以用于冲突资源 不能 被同时占用的问题


# 5. signal 信号
1. 一个进程给另一个进程发信号
2. 信号进程有且只有以下三种响应
    1. 执行默认操作
    2. 捕捉信号, 执行用户态回调
    3. 忽略信号 (但 SIGKILL 和 SEGSTOP 无法被捕捉)

# 6. socket 套接字

# 9. 综合对比
## 9.1 msq 和 pipe 对比
1. 不同点:
    1. msq 解决 pipe 进程间频繁交换数据效率低的问题 (??)
    2. 生命周期:
       pipe 随着创建进程销毁而销毁
       msq 消息链表 and 消息本体 不随创建进程 or 发送进程销毁而销毁
    3. 数据长度: pipe 通讯数据不定长, msq 通讯数据是单次定长的
    4. 通讯方向: pipe 一个管道中通讯只能是单向的, 而 msq 是双向的
4. 相同点:
    1. 通讯双方都需要 内核态 和 用户态 的数据拷贝 copy_to_user / copy_from_user
    2. 都不适合大量数据传输
## 9.2 shm 和 msq 对比
1. 不同点:
    1. msq 可以实现阻塞性等待, 实现 订阅/派发, 实现时序性功能
    2. shm 需要额外的机制实现读写时序性, 如 sem / 加锁
    3. shm 高效率的大量数据传输, 避免了 copy_to_user / copy_from_user
2. 相同点:
    1. 生命周期: 不随创建进程销毁而销毁
    2. 通讯方向: 同一个 shm / msq 都是可以是双向通讯

## 9.3 sem 和 shm 在实际中的使用模式
生产者进程:  up, 写数据, down
消费者进程:  up, 读数据, down

## 9.4 signal 与 {msq/shm/sem} 对比
1. 通讯场景不同
    1. { msq / shm / sem } 一般用于正常场景的通讯
    2. signal  一般用于异常场景的通讯
2. 通讯方式不同
    1. signal 唯一的 **真·异步通信机制**

## 9.5 socket 与 { pipe / msq / shm / sem / signal } 对比
1. socket 不仅可以在本机通讯, 而且可以在 不同主机间通讯
