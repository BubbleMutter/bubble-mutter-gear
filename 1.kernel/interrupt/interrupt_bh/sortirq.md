[TOC]
# softirq 对象定义
```c++
// 类型
struct softirq_action {
    void (*action)(struct softirq_action *); // 回调实体
};

// 3.16.78 内核注册的软中断; 10 个软中断ID
enum {
    HI_SOFTIRQ=0,   // 0: 高优先级tasklet
    TIMER_SOFTIRQ,  // 1: 定时器
    NET_TX_SOFTIRQ, // 2: 发包
    NET_RX_SOFTIRQ, // 3: 收包
    BLOCK_SOFTIRQ,  // 4:
    BLOCK_IOPOLL_SOFTIRQ,
    TASKLET_SOFTIRQ, // 6: 低优先级tasklet
    SCHED_SOFTIRQ,   // 7:
    HRTIMER_SOFTIRQ, // 8:
    RCU_SOFTIRQ,    /* Preferable RCU should always be the last softirq */
    NR_SOFTIRQS
};
// 对象 (软中断表); 即软中断个数, 只有在编译时确定, 不可增删;
// 全局软中断矢量, 全CPU共享
static struct softirq_action softirq_vec[NR_SOFTIRQS] _cacheline_aligned_in_smp;

// 赋值 软中断回调; 除了赋值就没其他了
void open_softirq(int nr, void (*action)(struct softirq_action *)) {
	softirq_vec[nr].action = action;
}

/*
 * 每个CPU有ta自己的一个上下文, irq_cpustat_t
 * 每个CPU虽然都有它自己的触发和控制机制，
 * 并且只执行他自己所触发的软中断请求，
 * 但是各个CPU所执行的软中断服务例程却是相同的
 * 结论: 某一段softiqrs 同一时刻可以在不同CPU并发运行
 */
#define NR_IPI	6 // CPU个数
typedef struct {
  unsigned int __softirq_pending;
#ifdef CONFIG_LOCAL_TIMERS
  unsigned int local_timer_irqs;
#endif
#ifdef CONFIG_SMP
  unsigned int ipi_irqs[NR_IPI];
#endif
} ____cacheline_aligned irq_cpustat_t;
irq_cpustat_t irq_stat[NR_CPUS];
```
1. NR_SOFTIRQS 最大值为32
   1. 因为 raise_softirq 中把 __softirq_pending 对应的 bit 设为 1
   2. 而 do_softirq 中检查 __softirq_pending 为1的bit 执行 软中断
2. percpu 变量实现并发

# softirq 机制
## softirq 触发 raise_softirq
```c++
/**
 * @brief 触发指定软中断; 关闭硬中断-raise_softirq_irqoff-开启硬中断
 */
void raise_softirq(unsigned int nr);

/**
 * @brief 触发指定软中断, 仅限与硬中断关闭的上下文
 * @note  实现:
 *        __raise_softirq_irqoff
 *        如果中断关闭, 则 wake_up_process 唤醒 ksoftirqd 线程, 否则不执行
 *        唤醒的本意是加入 "运行队列"
 */
void raise_softirq_irqoff(unsigned int nr);

/**
 * @brief 触发指定软中断, __softirq_pending 的 bit 设置为 1
 * @param nr nr为中断ID, XX__SOFTIRQS
 */
void __raise_softirq_irqoff(unsigned int nr);
```
1. 事实上; `raise_softirq_irqoff` 所谓唤醒内核线程 不是马上实时调度的
2. 而是 `sched_class->task_waking` > `ttwu_queue`; 提高一下响应延迟 (在调度器插队)

## softirq 执行 do_softirq -> __do_softirq
```c++
// 遍历 __softirq_pending; 执行已触发软中断的回调 `softirq_vec[i]->action()`
void __do_softirq(void) {
    __u32 pending;
	int softirq_bit;
    h = softirq_vec;  // per_cpu 变量
    // ...
    // 当前 cpu 的 __softirq_pending
	pending = local_softirq_pending();
    // ...
    // 关闭当前 cpu 中断后半部; 避免软中断嵌套
    __local_bh_disable_ip(_RET_IP_, SOFTIRQ_OFFSET);
    // ...
    local_irq_enable(); // 允许软中断执行过程中相应硬件中断
    // ...
	while ((softirq_bit = ffs(pending))) {
        h += softirq_bit - 1;  // softirq_bit 是从1开始的
        // ...
    	h->action(h);
        // ...
		h++;
		pending >>= softirq_bit;
    }
    // ...
    local_irq_disable();
    // ...
    __local_bh_enable(SOFTIRQ_OFFSET);  // 使能当前 cpu 中断后半部
}

/**
 * @brief find first bit set
 * @note  等同 man ffs
 *        最低位为 1, 最高为 32
 * @return 返回数字 [1, 32]
 */
long ffs(int x);
```
1. __do_softirq 遍历 __softirq_pending 的每个 bit
2. softirq 执行时机
   1. 硬件中断回调返回后, 执行一次软中断
   2. ksoftiqrd 内核线程中执行
   3. 显式检查并执行软中断的系统 (如网络子系统)

## 软中断运行入口
```c++
void irq_exit(void) {
    // ...
	if (!in_interrupt() && local_softirq_pending())
		invoke_softirq();
    //...
}
```
1. 软中断触发机制
    1. 硬中断结束 `irq_exit` -> `invoke_softirq` -> `__do_softirq`
    2. 软中断进程 `run_ksoftirqd` -> `__do_softirq`
    3. 自行触发

# ksoftirqd 内核线程
1. 每个处理器都有一个 ksoftirqd `ps aux | grep ksoftirqd | grep -v grep`
2. ksoftirqd 是解决 do_softirq 一次遍历后触发新的软中断的问题
    1. 如果 do_softirq 反复遍历 pending 标记并执行软中断 action
       将在负载较高时导致当前 cpu 其他任务被饿死; 因为软中断没有调度
    2. 如果 do_softirq 只有遍历 pending 标记一次,
       将会导致一轮遍历软中断过程中, 产生的新的中断,
       需要等待下次 do_softirq 执行时才能被检测, 影响实时性
    3. 基于上述两个矛盾, 采取一次遍历 + 异步常驻线程轮询的策略
       既避免了饿死问题, 也提高了场景2中的响应实时性
```c++
DECLARE_PER_CPU(struct task_struct *, ksoftirqd); // 进程对象

// smp 内核线程; 即 percpu 线程
// 形同一个进程 while (1) { if (smp_thr->thread_should_run()) smp_thr->thread_fn(); }
static struct smp_hotplug_thread softirq_threads = {
	.store			= &ksoftirqd,
	.thread_should_run	= ksoftirqd_should_run,
	.thread_fn		= run_ksoftirqd,
	.thread_comm		= "ksoftirqd/%u",
};

// 初始化时初始化; ksoftirqd 线程
static __init int spawn_ksoftirqd(void) {
	register_cpu_notifier(&cpu_nfb);
	BUG_ON(smpboot_register_percpu_thread(&softirq_threads)); // 注册
	return 0;
}

static int ksoftirqd_should_run(unsigned int cpu) { // 判断有 pending 的软中断需要执行
	return local_softirq_pending();                 // this_cpu_read(irq_stat.__softirq_pending) 掩码; 形同信号
}

/*
 * 正是因为在 run_softirqd 中会关掉本地
 */
static void run_ksoftirqd(unsigned int cpu)
{
    // 软中断检查执行; 先关掉本地cpu中断;
    // 在 __do_softirq 将重新开启
    // 以支持单个软中断执行过程中, 相应硬件中断

    local_irq_disable();
    if (local_softirq_pending()) { // __softirq_pending 非零
        /*
         * We can safely run softirq on inline stack, as we are not deep
         * in the task stack here.
         */
        __do_softirq();  // 遍历vec表 前会开启本地中断; 遍历完成后关掉
        local_irq_enable(); // 软中断执行结束后; 开启本地cpu中断
        cond_resched();

        preempt_disable();
        rcu_note_context_switch(cpu);
        preempt_enable();
        return;
    }
    local_irq_enable(); // 没有软中断; 恢复本地cpu中断
}
```

# 软中断设计策略
## 1. 平衡吞吐量 和 用户响应 __do_softirq
1. 单次 `__do_softirq()` 遍历 vec表后; 检查新触发的软中断时间;
2. 在一定间隔内; 重新遍历;
3. 单次 `__do_softirq()` 中可能会遍历多次 vec表; 但是比对的原始时间时固定的
    + 即随着时间推移; 一定能从 `__do_softirq()` 中返回
