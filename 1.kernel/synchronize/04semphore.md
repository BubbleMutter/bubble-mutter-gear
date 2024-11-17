[TOC]
# semaphore.h
1. 信号量的释放 up 不会导致阻塞; 但会导致唤醒等待链表的进程
2. 信号量的占用down可能导致阻塞

## 1.1. 方法 ( 创建 / 占用 / 释放 )
```c++
#define DEFINE_SEMAPHORE(name)  // 信号值为1
void sema_init(struct semaphore *sem, int val);

//  占用
void down(struct semaphore *sem); // 阻塞
int down_trylock(struct semaphore *sem);
int down_timeout(struct semaphore *sem, long jiffies);
int down_killable(struct semaphore *sem); // 阻塞, 但只有SIGKILL才能唤醒
int down_interruptible(struct semaphore *sem); // 一般用这个, 阻塞且可中断, 上层根据retval 处理异常逻辑

// 释放
extern void up(struct semaphore *sem);
```

## 1.2. 实现 ( spinlock + count + schedule )
``` c++
struct semaphore {
	raw_spinlock_t		lock;
	unsigned int		count;
	struct list_head	wait_list;
};

void down(struct semaphore *sem) {
    unsigned long flags;
    raw_spin_lock_irqsave(&sem->lock, flags); // 加自旋锁
    if (likely(sem->count > 0))
        sem->count--; // 直接占用
    else
        __down(sem);  // __down_common(sem, TASK_UNINTERRUPTIBLE, MAX_SCHEDULE_TIMEOUT);
    raw_spin_unlock_irqrestore(&sem->lock, flags); // 释放
}
int __sched __down_common(struct semaphore *sem, long state, long timeout) {
    struct task_struct *task = current;
    struct semaphore_waiter waiter;

    list_add_tail(&waiter.list, &sem->wait_list); // 1. a)  等待队列;
    waiter.task = task;
    waiter.up = false;

    for (;;) {
        if (signal_pending_state(state, task))   // 2. a) 收到信号
            goto interrupted;
        if (unlikely(timeout <= 0)) // 3. a) 超时检测
            goto timed_out;
        __set_task_state(task, state); // 4. a) 调度准备; 设置 TASK_UNINTERRUPTIBLE
        raw_spin_unlock_irq(&sem->lock); // 4. b) 调度准备；释放锁; 以便异步唤醒
        timeout = schedule_timeout(timeout); // 4. c) 进行调度; 并记录调度时间
        raw_spin_lock_irq(&sem->lock); // 4. d) 调度完成; 继续加锁
        if (waiter.up) // 1. b) 等待队列; 在 up 中异步唤醒
            return 0;
    }

timed_out:
    list_del(&waiter.list); // 4. c) 超时；不再等待
    return -ETIME;

interrupted:
    list_del(&waiter.list); // 2. a) 收到信号; 不再等待阻塞; -EINTR
    return -EINTR;
}
```
```c++
void up(struct semaphore *sem) {
    unsigned long flags;
    raw_spin_lock_irqsave(&sem->lock, flags); // 自旋锁
    if (likely(list_empty(&sem->wait_list)))
        sem->count++;
    else
        __up(sem); // 唤醒等待队列中的进程
    raw_spin_unlock_irqrestore(&sem->lock, flags); // 加锁
}
void __sched __up(struct semaphore *sem) {
    struct semaphore_waiter *waiter
        = list_first_entry(&sem->wait_list, struct semaphore_waiter, list);
    list_del(&waiter->list); // 1. 移出等待链表
    waiter->up = true;       // 2. 设置标记位
    wake_up_process(waiter->task); // 3. 调度
}
```
1. sem 保证先到先得的逻辑:
    1. 在占用时, 如果需要等待, 把自身加入 等待者队列 的尾部 `list_add_tail`
    2. 在释放时, 如果存在等待, 则优先唤醒队列头部的节点 `list_first_entry`

# rwsem.h
## 2.1 方法 ( 创建 / r占用 / w占用 / r释放 / w释放 )
```c++
#define DECLARE_RWSEM(name)
#define init_rwsem(sem)

// r占用
void down_read(struct rw_semaphore *sem);
int down_read_trylock(struct rw_semaphore *sem);

// w占用
void down_write(struct rw_semaphore *sem);
int down_write_trylock(struct rw_semaphore *sem);

// r释放
up_read(struct rw_semaphore *sem);

// w释放
up_write(struct rw_semaphore *sem);

// 把 w占用 转换为 r占用
void downgrade_write(struct rw_semaphore *sem);
```
1. 原文是说 当一个写保护后，跟着一个耗费时间长的读保护 才适用 downgrade_write
    + 本质 = 原子化  w占用 切换 r占用
    + 目的 = 避免其他线程消耗更多的 r占用时间

## 2.2 实现
