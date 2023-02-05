[TOC]
# 1. tasklet 对象定义
```c++
struct tasklet_head // 单向不成环 链表
{
	struct tasklet_struct *head;
	struct tasklet_struct **tail; // 指向尾部对象的next
};

struct tasklet_struct {
    struct tasklet_struct *next; // tasklet 以链表形式存在
    unsigned long state;         // TASKLET_STATE_*; 锁
    atomic_t count;              // 引用计数
    void (*func)(unsigned long); // 回调
    unsigned long data;          // 回调自身的上下文
};

// tasklet_struct->state; 中有两个锁; 每个bit一个; 分别用原子访问
// 实际上只有 smp 架构中; 另一个cpu才能看到 TASKLET_STATE_RUN;
// 单核cpu 架构中; 只能看到 TASKLET_STATE_SCHED
// TASKLET_STATE_SCHED: 单 cpu 避免重复调度
// TASKLET_STATE_RUN: cpu 间实现互斥;
enum {
    TASKLET_STATE_SCHED, /* Tasklet is scheduled for execution */
    TASKLET_STATE_RUN    /* Tasklet is running (SMP only) */
};

#define DECLARE_TASKLET(name, func, data)
#define DECLARE_TASKLET_DISABLED(name, func, data)
void tasklet_init(struct tasklet_struct *t, void (*func)(unsigned long),  unsigned long data);

// softirq 初始化时; 初始化了 tasklet
void __init softirq_init(void) {
    // 其他初始化操作 ...
    for_each_possible_cpu(cpu) { // 每一个 cpu 都有 tasklet 链表
        per_cpu(tasklet_vec, cpu).tail = &per_cpu(tasklet_vec, cpu).head;
        per_cpu(tasklet_hi_vec, cpu).tail = &per_cpu(tasklet_hi_vec, cpu).head;
    }
    open_softirq(TASKLET_SOFTIRQ, tasklet_action); // 赋值软中断回调 tasklet_action
	open_softirq(HI_SOFTIRQ, tasklet_hi_action);   // 赋值软中断回调 tasklet_hi_action
}
// tasklet 的高低优先级如何体现?
// tasklet_action, tasklet_hi_action 为两个软中断ID的回调
// 其中 tasklet_hi_action 优先级更高, 即 软中断 ID 更低, 更早执行
```
1. tasklet 的初始化在软中断初始化中
2. 显然; 每个 cpu 有自己的 tasklet 队列


# 2. tasklet 执行 (软中断回调)
```c++
/* 回调本身 tasklet_action tasklet_hi_action 的执行思路
 * 1. 原子性 取出 tasklet_vec头, 一个链表指针, 把 tasklet_vec头置NULL
 * 2. 遍历 tasklet->list 链表
 * 3.   if tasklet_trylock                            // 保证只能一个CPU执行
 * 4.      if tasklet->count == 0                     // 引用计数器, > 1表示使能
 * 5.         test_and_clear_bit(TASKLET_STATE_SCHED) // 关闭 调度等待, 对应schedule中开启调度等待
 * 6.         tasklet->func                           // 调度
 * 7.         tasklet_unlock                          // 执行完 中断代码, 解锁
 * 8.         continue                                // 下一层遍历
 * 9.      else tasklet_unlock                        // tasklet禁止运行, 解锁
 * 10.  把 禁止的/被锁的 tasklet 重新入列 (也就是说 没重新入列的, 就已经出列了)
 */
static void tasklet_action(struct softirq_action *a) {
    struct tasklet_struct *list;

    // 1. 与入列同样道理; 出列操作避免 数据错乱要关闭中断
    // 2.1 这里是一个trick；把一整个队列(单向链表)取出来
    // 2.2 实际上 初始化时也是 { head = NULL; tail = &head; }
    local_irq_disable();
    list = __this_cpu_read(tasklet_vec.head);
    __this_cpu_write(tasklet_vec.head, NULL);
    __this_cpu_write(tasklet_vec.tail, &__get_cpu_var(tasklet_vec).head);
    local_irq_enable();

    while (list) {
        struct tasklet_struct *t = list;
        list = list->next;

        if (tasklet_trylock(t)) {   // 实现 smp 互斥 加锁
            if (!atomic_read(&t->count)) {  // 异步禁止检查 tasklet_disable_nosync
                if (!test_and_clear_bit(TASKLET_STATE_SCHED, &t->state)) // 重新允许调度
                    BUG();
                t->func(t->data);
                tasklet_unlock(t);  // 重新允许其他 cpu 执行
                continue;
            }
            tasklet_unlock(t);
        }
        // 1. 与入列同样道理; 出列操作避免 数据错乱要关闭中断
        // 2. 加锁失败; 说明正在被其他 cpu 运行
        // 3. 重新入列; 插入回本 cpu 的队列
        local_irq_disable();
        t->next = NULL;
        *__this_cpu_read(tasklet_vec.tail) = t;
        __this_cpu_write(tasklet_vec.tail, &(t->next));
        __raise_softirq_irqoff(TASKLET_SOFTIRQ);
        local_irq_enable();
    }
}
```
1. 体现了 中断禁止 与 加锁 的典型使用
    1. 本 cpu 数据一致性通过 中断禁止 保护
    2. cpu 间数据一致性通过加锁(原子变量)保护

# 3. tasklet 互斥 (避免并发)
```c++
#ifdef CONFIG_SMP
// state-bit[TASKLET_STATE_RUN] = 1, 加锁, 加锁前先检查
static inline int tasklet_trylock(struct tasklet_struct *t) {
	return !test_and_set_bit(TASKLET_STATE_RUN, &(t)->state);
}
// state-bit[TASKLET_STATE_RUN] = 0, 解锁
static inline void tasklet_unlock(struct tasklet_struct *t) {
    smp_mb__before_atomic();
    clear_bit(TASKLET_STATE_RUN, &(t)->state);
}
#else
#define tasklet_trylock(t) (1)
#define tasklet_unlock(t) do { } while (0)
#endif
```

# 4. tasklet 提交 (避免嵌套)
```c++
/* Q: HI_SOFTIRQ 和 TASKLET_SOFTIRQ 相对其他 softirqs 有哪些不同?
 * A: 有指定的中断触发函数, 先处理 tasklet 的逻辑
 *    然后分别 raise_softirq_irqoff(TASKLET_SOFTIRQ);
 *            raise_softirq_irqoff(HI_SOFTIRQ);
 *    从而实现一种特殊的 softirqs
 * details:
 *   if (!test_and_set_bit(TASKLET_STATE_SCHED)) // 还没有被调度
 *   local_irq_save();                   // 实现中断不嵌套 -- begin
 *   插入 tasklet 到 tasklet_vec 的尾部, 入列
 *   raise_softirq_irqoff(TASKLET_SOFTIRQ or HI_SOFTIRQ);
 *     然后调用 tasklet_action or tasklet_hi_action 回调
 *   local_irq_restore();                // 实现中断不嵌套 -- end
 */
static inline void tasklet_schedule(struct tasklet_struct *t) {
    // 如果已经调度了; 则马上返回;
    // test_and_seet_bit 是原子操作
    // 该 bit 实现中断不嵌套
    if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state))
        __tasklet_schedule(t);
}

void __tasklet_schedule(struct tasklet_struct *t) {
    unsigned long flags;

    local_irq_save(flags);  // 关闭本地中断;
    t->next = NULL;
    *__this_cpu_read(tasklet_vec.tail) = t;
    __this_cpu_write(tasklet_vec.tail, &(t->next));
    raise_softirq_irqoff(TASKLET_SOFTIRQ);
    local_irq_restore(flags);
}
EXPORT_SYMBOL(__tasklet_schedule);
```
1. TASKLET_STATE_SCHED 实现中断不嵌套 (因为只针对本 cpu 所以不需要原子操作)
2. 关闭本地中断 防止入列操作 和 唤醒操作 因为中断导致数据错乱
3. tasklet 入列当前 CPU

# 5. tasklet 使能
```c++
// 异步禁止, tasklet_struct->count++
static inline void tasklet_disable_nosync(struct tasklet_struct *t) {
	atomic_inc(&t->count);
	smp_mb__after_atomic();
}
// 同步禁止, 然后 tasklet_unlock_wait
static inline void tasklet_disable(struct tasklet_struct *t) {
	tasklet_disable_nosync(t);
	tasklet_unlock_wait(t);
	smp_mb();
}

// 使能, tasklet_struct->count--
static inline void tasklet_enable(struct tasklet_struct *t) {
	smp_mb__before_atomic();
	atomic_dec(&t->count);
}
// NOTE: 使能不区分同步异步; 都是标记 count 由 __do_softirq 执行

// 阻塞等待
static inline void tasklet_unlock_wait(struct tasklet_struct *t) {
	while (test_bit(TASKLET_STATE_RUN, &(t)->state)) { barrier(); }
}
```
1. 使能对 count 进行操作; 必须是原子化的, 因为 tasklet_struct 多 cpu 共享
2. 禁止有两种:
    1. tasklet 已提交但未执行; 禁止使得后续不会执行
       对应 tasklet_action 中对 tasklet_struct->count 的判断
    2. tasklet 已提交且已执行; 阻塞地等待已提交的 tasklet 完成
3. 禁止操作的内存屏障, 设计逻辑:
    1. 设计上避免 **禁止后** 的代码块与 tasklet 竞争
    2. 如果 **禁止前** 的代码块与 tasklet 竞争, 需要加锁处理
4. 使能操作的内存屏障, 设计逻辑
    1. 设计上避免 **使能前** 的代码块与 tasklet 竞争
    2. 如果 **使能后** 的代码块与 tasklet 竞争, 需要加锁处理

# 6. tasklet 终止
```c++
void tasklet_kill(struct tasklet_struct *t) {
    // ...

    // 1. 等待所有 cpu 不再调度了
	while (test_and_set_bit(TASKLET_STATE_SCHED, &t->state)) {
		do {
			yield();
		} while (test_bit(TASKLET_STATE_SCHED, &t->state));
	}
    // 2. 同步等待已执行的 tasklet 执行完成
	tasklet_unlock_wait(t);
    // 3. 重新允许调度
	clear_bit(TASKLET_STATE_SCHED, &t->state);
}
```
1. TASKLET_STATE_SCHED 的双重 while 用于处理极端场景
    1. cpu0 在 tasklet_trylock 中 clear(TASKLET_STATE_SCHED)
    2. cpu1 紧接着 tasklet_schedule 中 set(TASKLET_STATE_SCHED)
    3. 这个极端场景是无尽的 cpu0->cpu1->cpu0->cpu1 ...
    4. 最终在 TASKLET_STATE_SCHED 确定为 0 时; set(TASKLET_STATE_SCHED)
    5. 从而使得其他 cpu 无法再次调度
2. tasklet_unlock_wait 等待当前 tasklet_struct->func 执行完成
3. 重新允许调度
4. 总结: tasklet_kill 不同于进程的 kill; 只是为了阻塞地等待 tasklet 完成

# 7. STATE_RUN 和 STATE_SCHED 的设计
1. 避免中断嵌套 与 避免多核并发 只需要 STATE_SCHED 就足够了
2. STATE_RUN 是为了提高吞吐量
3. 如果只有 STATE_SCHED, 那么在 tasklet 没运行完之前, 无法重新提交
4. 而新增 STATE_RUN 避免多核并发
   在 tasklet_trylock 后 clear(STATE_SCHED)
   然后执行 tasklet_struct->func 的过程中
   允许相同 tasklet 重新提交, 但还是无法多核并发(因此需要引入 STATE_RUN 实现)
5. 从而降低中断后半部被丢弃的概率
    1. 极端情况下在 cpu0 的 tasklet_struct->func 执行时间相当较长
    2. 同时, cpu1 相同硬件触发新的中断, 回调中提交 tasklet
    3. 那么, cpu1 的提交操作使得 tasklet 重新入列
