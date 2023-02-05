# spinlock
# 1. 方法 (创建 / 占用 / 释放)
```c++
// 创建
spinlock_t lock;
#define spin_lock_init(_lock)

// 占用
void spin_lock(spinlock_t *lock);
#define spin_lock_irqsave(lock, flags) // 保存中断上下文 停止其他中断
void spin_lock_irq(spinlock_t *lock);  // 忽略中断上下文 停止其他中断
void spin_lock_bh(spinlock_t *lock);   // 忽略中断上下文 仅停止软中 允许硬中断 (参考 中断机制)

// 释放
void spin_unlock(spinlock_t *lock);
void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags);
void spin_unlock_irq(spinlock_t *lock);
void spin_unlock_bh(spinlock_t *lock); // 释放后还原 bh状态

// 非阻塞性占用; 返回非零 表示成功占用; 成功占用后需要释放
int spin_trylock(spinlock_t *lock);
#define spin_trylock_irqsave(lock, flags)
int spin_trylock_bh(spinlock_t *lock);
int spin_trylock_irq(spinlock_t *lock);

// 检查
int spin_is_locked(spinlock_t *lock);
```
1. `spin_lock_irq`, `spin_lock_bh`, `spin_lock` 都必须在
   `spin_lock_irqsave` 和 `spin_unlock_irqrestore` 包住的代码块中

# 2. 实现 (仅讨论 smp 场景)
## 2.1 公共封装
```c++
// <linux/spinlock.h> 中
#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
# include <linux/spinlock_api_smp.h>
#else
# include <linux/spinlock_api_up.h>
#endif

// 调用链
// spin_lock > raw_spin_lock > _raw_spin_lock > __raw_spin_lock
// do_raw_spin_lock > arch_spin_lock (arch/$arch/include/spinlock*.h 由相应arch汇编实现)
static inline void __raw_spin_lock(raw_spinlock_t *lock) {
    preempt_disable();
    spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
    LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock);
}
// spin_unlock > raw_spin_unlock > _raw_spin_unlock > __raw_spin_unlock
// do_raw_spin_unlock > arch_spin_unlock (arch/$arch/include/spinlock*.h 由相应arch汇编实现)
static inline void __raw_spin_unlock(raw_spinlock_t *lock) {
	spin_release(&lock->dep_map, 1, _RET_IP_);
	do_raw_spin_unlock(lock);
	preempt_enable();
}
```
1. up 和 smp 的实现方式不同; 通过 CONFIG_SMP 区分
2. `preempt_disable()` `preempt_enable()` 占用前后关闭/开启内核抢占
3. `spin_acquire` `spin_release` 用于死锁检测
4. 核心逻辑: `arch_spin_lock` `arch_spin_unlock`

## 2.2 x86 实现说明 (非 CONFIG_PARAVIRT_SPINLOCKS 场景)
```c++
#define __TICKET_LOCK_INC	1   // 单个 cpu ticket 递增值
#define TICKET_SLOWPATH_FLAG	((__ticket_t)0)

typedef u8  __ticket_t;      // u8 可以为 255 个 cpu 排队
typedef u16 __ticketpair_t;

typedef struct arch_spinlock {  // 自旋锁使用两个 ticket 构造
    struct __raw_tickets {
        __ticket_t head, tail;
    } tickets;
} arch_spinlock_t;


static __always_inline void arch_spin_lock(arch_spinlock_t *lock) {
	register struct __raw_tickets inc = { .tail = TICKET_LOCK_INC };

	inc = xadd(&lock->tickets, inc);
	if (likely(inc.head == inc.tail))
		goto out;

	inc.tail &= ~TICKET_SLOWPATH_FLAG;
	for (;;) {
		unsigned count = SPIN_THRESHOLD;

		do {
			if (ACCESS_ONCE(lock->tickets.head) == inc.tail)
				goto out;
			cpu_relax();
		} while (--count);
		__ticket_lock_spinning(lock, inc.tail);
	}
out:	barrier();	/* make sure nothing creeps before the lock is taken */
}

static __always_inline void arch_spin_unlock(arch_spinlock_t *lock) {
    __add(&lock->tickets.head, TICKET_LOCK_INC, UNLOCK_LOCK_PREFIX);
}
```
1. 本质 ticket-based 的排队算法, 分为两个部分
    1. 一个表示队列的头部 head, 一个表示队列的尾部 tail
    2. 占用锁时, tail++, 然后等待初始的 head 等于 tail 才允许占用
    3. 释放锁时, head++
2. xadd 的函数功能: 原子性地 第一参数增加第二参数, 并返回原始的第一参数.
3. __ticket_lock_spinning 在非 CONFIG_PARAVIRT_SPINLOCKS 场景为空
4. barrier 内存屏障保证临界区代码不会提前执行

## 2.3 x86 实现说明 (CONFIG_PARAVIRT_SPINLOCKS 场景)

# 3. 自旋锁流程概述
+ 单处理器:
    1. local_save_irq    [optional]
    2. preempt_disable
    3. 执行代码
    4. preempt_enable
    5. local_restore_irq [optional]
+ 多处理器
    1. local_save_irq    [optional]
    2. preempt_disable
    4. arch_spin_lock
    5. 执行代码
    7. arch_spin_unlock
    8. preempt_enable
    9. local_restore_irq [optional]

# 4. 自选锁以及原子上下文的 场景
## 4.1 spinlock 占用前停止其他中断 (实现)
硬件中断回调(interrupt_handle) 不允许休眠，一般都使用spinlock
如果在 spinlock 被占用时, 产生了硬件中断回调, 而回调中占用spinlock 就会导致死锁
+ 因此 spinlock 占用前停止其他中断
+ 因此 硬件中断可能会丢掉

## 4.2 spinlock 占用时间尽可能短 (使用)
由于spinlock 占用前会停止其他中断 可能导致中断丢失
如果占用时间长 将提高中断丢失的概率
所以 spinlock 占用时间尽可能短

## 4.3 spinlock 占用后不能休眠 (使用)
spinlock 占用后如果休眠, 则CPU将在足够长的时间后才释放
而如此足够长的时间导致其他 代码无法响应，
其他代码可能也会尝试占用，导致CPU死锁

## 4.4 spinlock 多锁占用时 必须遵守同样的顺序占用
当存在资源需要多个 lock1 lock2 lockN 时
在不同的占用代码地方 都需要遵守同样的顺序占用

## 4.5 spin_lock_bh spin_unlock_bh 的场景
1. 获取锁后禁止本地所有下半部执行
2. 即允许硬件中断回调
3. 因此, 要求加锁的数据 不在硬件中断回调中共享
1. 下半部 可以抢占进程上下文
2. 下半部 与 进程有上下文 有共享数据时; 但硬中断回调与进程上下文无共享数据
3. 在进程上下文中；仅关闭 下半部 同时保留硬中断(保留硬件响应性能)
