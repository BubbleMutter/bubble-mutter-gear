
[TOC]
1. 中断下半部本质上有5种方式处理:
    1. sortirq
    2. tasklet
    3. workqueue
    4. timer(这里不讨论)
    5. 内核线程(这里不讨论)

# 1. 中断的本身限制 (下半部分 的 产生原因)
中断下半部分的软中断 跟系统调用的软中断时不一样的!!!
系统调用的软中断      = 进程上下文
中断下半部分的软中断  = 软中断上下文 ( NR_CPUS 个 ksoftirqd 内核线程 )
硬件中断              = 硬件中断上下文

# 2. 发展历史 bh > softirq > tasklet > workqueue
1. 先有软中断 percpu 实现多 cpu 并发;
    1. 需要额外逻辑保证序列化; (因为 percpu 并行)
2. 再有 tasklet；一种禁止多cpu 并发的软中断子任务;
    1. 自带序列化保证
3. 最后 workqueue 由 kworker 内核线程 运行在进程上下文中的可休眠任务
    1. 可休眠的代价是 开销大; 因为需要内核线程切换 以及 进程上下文切换
    2. 遇到频繁产生的中断(如 网卡); workqueue 并不合适
    3. 需要额外逻辑保证序列化；(因为 内核线程 并发)

# 3. 中断下半部分的使能 (local_bh_disable / local_bh_enable)
2. `local_bh_disable`
    + `&current_thread_info()->preempt_count += SOFTIRQ_DISABLE_OFFSET`;
3. `local_bh_enable`
    1. `&current_thread_info()->preempt_count -= SOFTIRQ_DISABLE_OFFSET - 1` 避免重复执行
    2. do_softirq();
    3. `&current_thread_info()->preempt_count -= 1` 恢复为0

# 4. interrupt_bh 特性对比
## bottom half (2.2 内核之前才有)
1. 编译时静态创建, 链表, 最大32个节点
2. 上半部通过一个32位整数中的一位来标识出哪个bottom half可执行
3. 每个BH都在全局范围内进行同步，即使分属于不同的处理器
4. 在任意一时刻, 系统只能有一个CPU可以执行 BH 代码 (无法发挥 smp 的特性)
5. BH函数不允许嵌套

## softirq
1. 编译时静态创建, 数组, 最大32个节点, 运行时不可以增删
   linux-3.16.78 初始占用了10个
2. 同一个 softirq 可以同时在多个 cpu 上并发
   每个 cpu 都有自己的 软中断触发运行机制
   softirq 提交的 cpu 即当前运行 cpu
3. 软中断不会嵌套
   在软中段执行前, 关掉了本地软中断
   即当 软中断运行过程中, 触发了硬件中断,
   那么新的硬件中断结束后不会执行软中断,
   而是回到 被打断的软中断 执行的代码中
4. 软中断上下文中 允许响应硬中断; 但软中断中不能休眠

## tasklet (占用 softirq id)
1. 动态创建, 可增删
2. 运行在 原子上下文, 不能休眠, 不会被调度, 受硬件中断影响
3. 执行和提交的 cpu 是相同, 即只有中断上半部分处理完,
   才会跑当前cpu中的软中断, 其中执行到当前cpu提交的 tasklet
4. 同一个 tasklet 不能同时在多个 cpu 上并发
   tasklet_struct->state 标记 TASKLET_STATE_RUN   避免并发 (原子操作)
5. 同一个 tasklet 不能嵌套在一个 cpu 上
   tasklet_struct->state 标记 TASKLET_STATE_SCHED 避免嵌套 (非原子操作)
6. tasklet 提交 cpu 同于 运行 cpu

## workqueue
1. 动态创建, 可增删
2. 运行在进程上下文, 可休眠, 受 中断 和 调度器 影响
   由内置内核线程 work_thread 运行,
   该线程优先级低于用户线程
4. 实现描述
    1. 有多个内核线程 work_thread, 消费多个 workqueue_pool 中的 workqueue
    2. 一个队列, 每个队列都包含一个由等待调用的函数组成链表
    3. 根据其所处队列的位置, 这些函数会在某个时刻被执行
    4. 驱动程序可根据需要把它们自己的下半部注册到合适的队列上去
5. workqueue 提交 cpu 可以不同于运行 cpu
    + 提交时可以指定 cpu; 也可以由内核动态分配

# 5. 下半部加锁问题
1. **进程上下文**: 通常是 workqueue or 系统调用
2. **中断下半部**: 包括 softirq 和 tasklet
3. 加锁是为了避免多 cpu 的并文冲突
   还要需要考虑本地 cpu 的中断嵌套导致的冲突
4. 当 **进程上下文** 与 **中断下半部** 共享数据;
   那么在加锁前需要先禁用 **中断下半部**.
   因为 **进程上下文** 是允许响应硬件中断, 在回调完成后进入 **中断下半部**,
   如果在加锁前没有禁用中断下半部, 那么将在本地 cpu 的中断下半部中死锁.
5. 当 **中断上下文** 与 **中断下半部** 共享数据;
   那么在加锁前需要先禁止中断.
   因为 **中断下半部** 是允许响应硬件中断,
   如果在加锁前没有禁用中断, 那么将在本地 cpu 响应硬件中断时死锁.

## preempt_count 计数器
1. 每个进程 task_struct 都有一个 preempt_count 计数器
2. preempt_count 为0时, **中断下半部** 才允许被执行
3. 禁止 **中断下半部** 即 `preempt_count++`
4. 允许 **中断下半部** 即 `preempt_count--`
5. inc/dec 的设计 是为了满足 嵌套且成对的禁止/允许
    1. 如果只用 bit; 则难以满足 嵌套且成对的禁止/允许
