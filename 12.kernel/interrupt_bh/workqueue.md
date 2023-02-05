[TOC]
# workqueue 使用示例
```c++
bool schedule_work(struct work_struct *work);
// delay 是 jiffies 单位
bool schedule_delayed_work(struct delayed_work *dwork, unsigned long delay);
// 指定 cpu
bool queue_work_on(int cpu, struct workqueue_struct *wq, struct work_struct *work);

// 函数一直等待; 直到所有 work_struct 完成后才返回; 只能在进程上下文中调用
void flush_scheduled_work(void);

bool cancel_delayed_work(struct delayed_work *dwork);
bool cancel_delayed_work_sync(struct delayed_work *dwork);
```

# workqueue 故有队列
```c++
// 内核全局
extern struct workqueue_struct *system_wq;
extern struct workqueue_struct *system_highpri_wq;
extern struct workqueue_struct *system_long_wq;
extern struct workqueue_struct *system_unbound_wq;
extern struct workqueue_struct *system_freezable_wq;
extern struct workqueue_struct *system_power_efficient_wq;
extern struct workqueue_struct *system_freezable_power_efficient_wq;
```

# workqueue 对象定义
```c++
struct worker_pool {
    struct list_head	worklist;	/* L: list of pending works */
    struct list_head	workers;	/* A: attached workers */
}

struct worker {
	struct task_struct	*task;		/* I: worker task */
	struct worker_pool	*pool;		/* I: the associated pool */
}

/* the per-cpu worker pools */
static DEFINE_PER_CPU_SHARED_ALIGNED(struct worker_pool[NR_STD_WORKER_POOLS], cpu_worker_pools);

struct workqueue_struct {
	struct list_head	pwqs;		/* WR: all pwqs of this wq */
	struct list_head	list;		/* PL: list of all workqueues */
    struct pool_workqueue __percpu *cpu_pwqs; /* I: per-cpu pwqs */
};

struct pool_workqueue {
	struct worker_pool	*pool;		/* I: the associated pool */
	struct workqueue_struct *wq;		/* I: the owning workqueue */
    struct list_head	pwqs_node;	/* WR: node on wq->pwqs */
}

struct work_struct {
	atomic_long_t data;
	struct list_head entry;
	work_func_t func;
};



// cpu {
//     worker_thread -- woker -- woker_pool -> work_struct[]
//     worker_thread -- woker -- woker_pool -> work_struct[]
//     ...
// }

// workqueue_struct {
//     per_cpu pool_workqueue {
//         [cpu0] -> woker_pool
//         [cpu1] -> woker_pool
//         ...
//     }
// }
```
1. 每个 cpu 上运行着若干个 worker_thread, 最初是 NR_STD_WORKER_POOLS 个
    1. 一个 worker_thread 与 一个 worker 对应
    2. 一个 worker 与 一个 woker_pool 对应
2. NR_STD_WORKER_POOLS 为2 (0表示低优先级; 1表示高优先级)
3.  一个 workqueue_struct 有 NR_CPUS 个 pool_workqueue; pwqs 连接
    1. 一个 pool_workqueue 指向对应 cpu 的 worker_pool
4. NOTE:
    1. 对象定义没有说明 worker_pool 如何关联 pool_workqueue
    2. 对象定义没有说明 work_struct 如何关联 worker_pool
    3. work_struct 关联 worker_pool 的逻辑在 `__queue_work()` 中

# workqueue 初始化
## worker_pool worker worker_thread
```c++
enum {
    NR_STD_WORKER_POOLS	= 2,		/* # standard pools per cpu */
}

/* the per-cpu worker pools */
static DEFINE_PER_CPU_SHARED_ALIGNED(struct worker_pool[NR_STD_WORKER_POOLS], cpu_worker_pools);

static int __init init_workqueues(void) {
    /* HIGHPRI_NICE_LEVEL 即 MIN_NICE, -20, 即进程最高优先级 */
	int std_nice[NR_STD_WORKER_POOLS] = { 0, HIGHPRI_NICE_LEVEL };
	int i, cpu;
    // ...
    /* initialize CPU pools */
	for_each_possible_cpu(cpu) {  // 遍历每个一个
        // 遍历每个 cpu 的 worker_pool 数组, 目前只有两个
		for_each_cpu_worker_pool(pool, cpu) {
            // ...
			pool->cpu = cpu;  // cpu id
			pool->attrs->nice = std_nice[i++];   // 进程优先级
            // ...
        }
    }
	/* create the initial worker */
	for_each_online_cpu(cpu) {
		struct worker_pool *pool;
		for_each_cpu_worker_pool(pool, cpu) {
            // ...
            // 创建 worker 以及 worker_thread
            // 设置 worker_thread 优先级 set_user_nice(pool->attrs->nice)
			BUG_ON(create_and_start_worker(pool) < 0);
		}
	}
}

static int create_and_start_worker(struct worker_pool *pool) {
	struct worker *worker;
	worker = create_worker(pool);
    // ...
}

static struct worker *create_worker(struct worker_pool *pool) {
	struct worker *worker = NULL;
    // ...
	worker = alloc_worker();
    // ...
	worker->task = kthread_create_on_node(worker_thread, worker, pool->node, "kworker/%s", id_buf);
    // ...
    // 设置进程优先级
    set_user_nice(worker->task, pool->attrs->nice);
    // ...
}
```
1. per_cpu 变量 cpu_worker_pools
    1. 每个 cpu 初始化2个 worker_pool
    2. 一个表示普通优先级, 一个表示最高优先级
2. 每个 worker_pool 关联一个 worker
3. 每个 worker 创建一个 worker_thread

## workqueue_struct
```c++
static LIST_HEAD(workqueues);		/* PL: list of all workqueues */

#define alloc_workqueue(fmt, flags, max_active, args...)		\
	__alloc_workqueue_key((fmt), (flags), (max_active),		\
			      NULL, NULL, ##args)

// 创建一个 workqueue 的主要流程
struct workqueue_struct *__alloc_workqueue_key(const char *fmt,
					       unsigned int flags,
					       int max_active,
					       struct lock_class_key *key,
					       const char *lock_name, ...) {
	struct workqueue_struct *wq;
	struct pool_workqueue *pwq;
    // ...

    // 通过 per_cpu 成员变量 struct pool_workqueue __percpu *cpu_pwqs;
    // 把 workqueue 连接到 per_cpu 变量 worker_pool 中
	if (alloc_and_link_pwqs(wq) < 0)
		goto err_free_wq;

    // ...

    // 把 workqueue 连接到全局 workqueues 中
	list_add(&wq->list, &workqueues);
    // ...
}

static int alloc_and_link_pwqs(struct workqueue_struct *wq)
{
	bool highpri = wq->flags & WQ_HIGHPRI; // 是否高优先级
    // ...
	if (!(wq->flags & WQ_UNBOUND)) {
		wq->cpu_pwqs = alloc_percpu(struct pool_workqueue);
        // ...
		for_each_possible_cpu(cpu) {
            // 每个 workqueue 的 per_cpu 成员 cpu_pwqs
			struct pool_workqueue *pwq = per_cpu_ptr(wq->cpu_pwqs, cpu);
            // 全局 per_cpu 变量 cpu_worker_pools, 区分 cpu
			struct worker_pool *cpu_pools = per_cpu(cpu_worker_pools, cpu);
            // 全局 per_cpu 变量 cpu_worker_pools, 区分 优先级
			init_pwq(pwq, wq, &cpu_pools[highpri]);  // pwq->pool 指向 worker_pool
			mutex_lock(&wq->mutex);
			link_pwq(pwq); // per_cpu 的 pwq->pwqs_node 连接到全局的 wq->pwqs 中
			mutex_unlock(&wq->mutex);
		}
		return 0;
	} else if (wq->flags & __WQ_ORDERED) {  // 有序 workqueue
		ret = apply_workqueue_attrs(wq, ordered_wq_attrs[highpri]);
		/* there should only be single pwq for ordering guarantee */
		WARN(!ret && (wq->pwqs.next != &wq->dfl_pwq->pwqs_node ||
			      wq->pwqs.prev != &wq->dfl_pwq->pwqs_node),
		     "ordering guarantee broken for workqueue %s\n", wq->name);
		return ret;
	} else {  // 游离 workqueue
		return apply_workqueue_attrs(wq, unbound_std_wq_attrs[highpri]);
	}
}
```
1. 申请内存
2. 连接 wq->pwqs 作为 pool_workqueue 的头
    1. 非WQ_UNBOUND 类型: 把 per_cpu 成员 pool_workqueue 连接在 wq->pwqs
3. 连接 wq->list 作为 workqueues 的成员

# workqueue 执行 (内核线程 worker_thread)
```c++

```
1. process_one_work 执行入口




# workqueue 入列实现 (__queue_work)
```c++
static void __queue_work(int cpu, struct workqueue_struct *wq, struct work_struct *work) {
    // 根据 WQ_UNBOUND 选择对应 per_cpu 成员 wq->cpu_pwqs
    // 即选择 wq 对应中其中一个 pool_workqueue
    struct pool_workqueue *pwq;
	if (!(wq->flags & WQ_UNBOUND))
		pwq = per_cpu_ptr(wq->cpu_pwqs, cpu);
	else
		pwq = unbound_pwq_by_node(wq, cpu_to_node(cpu));
    // ...
    /*
     * 这里还有一个逻辑, 如果 work_struct 之前已经在相同 wq 的另一个 pool 执行过
     * 那么为了避免重入
     * 找到 work 之前运行的 pool, 并把 pwq 指向 work 之前运行的 pwq
     */
    spin_lock(&pwq->pool->lock);
    // ...
	if (likely(pwq->nr_active < pwq->max_active)) {
        // pool_workqueue 没有超过上限
		// ...
		pwq->nr_active++;
		worklist = &pwq->pool->worklist;  // 指向 pool->worklist
	} else {
        // pool_workqueue 已经超过上限
		work_flags |= WORK_STRUCT_DELAYED;
		worklist = &pwq->delayed_works;  // 指向 delayed_works
	}
	insert_work(pwq, work, worklist, work_flags); // 入列
    // ...
	spin_unlock(&pwq->pool->lock);
}

static void insert_work(struct pool_workqueue *pwq, struct work_struct *work, struct list_head *head, unsigned int extra_flags) {
    // ...
	list_add_tail(&work->entry, head);
    // ...
}
```
1. `queue_work` / `queue_work_on` / `schedule_work_on` / `schedule_work` 最终调用 `__queue_work`
2. 先根据 { `workqueue_struct`, cpu, 最近运行的 cpu } 找到合适的 pool_workqueue
3. 如果没有超过上限: 则入列到 pool_workqueue->worker_pool->worklist
4. 如果已经超过上限: 则入列到 pool_workqueue->delayed_works 缓存起来

# workqueue {创建 / 停止 / 销毁}
```c++
#define create_workqueue(name)	            // 每个CPU上都分配一个 (percpu 变量)
#define create_singlethread_workqueue(name)	// 仅运行于一个CPU
// 强制执行, 等待所有task执行完 [需要深刻了解]
// 该函数会阻塞, 深入了解一下 color 机制
// 如果, task有while, 该函数可能会卡住
// 如果, task在阻塞休眠, 需要先 wake_up_interruptible, 再flush
void flush_workqueue(struct workqueue_struct *wq);
void drain_workqueue(struct workqueue_struct *wq);
// 销毁队列
void destroy_workqueue(struct workqueue_struct *wq);
```

# work { 定义/ 提交 / 停止}
```c++
#define DECLARE_WORK(n, f)      // 编译时 静态定义
#define INIT_WORK(_work, _func) // 运行时 初始化
// 提交指定 workqueue
bool queue_work(struct workqueue_struct *wq, struct work_struct *work);
bool queue_work_on(int cpu, struct workqueue_struct *wq, struct work_struct *work)

// 提交默认 system_wq
bool schedule_work_on(int cpu, struct work_struct *work);
bool schedule_work(struct work_struct *work);

// 阻塞直到 work 运行完成; 无论work在pending/flight
bool flush_work(struct work_struct *work);
// 异步取消; 取消pending状态的work; flight状态的直接返回
bool cancel_work(struct work_struct *work);
// 同步取消; 取消pending状态的work; flight状态 阻塞直到 work运行完成
bool cancel_work_sync(struct work_struct *work);
```

# workqueue 设计小结
1. 对于使用者来说; `pool_workqueue` 是透明的
    1. `workqueue_struct` 作为句柄; 在 __queue_work 中封装了 路由逻辑
    2. 最终路由到关联 cpu 的 worker_pool 中
2. worker_thread -- worker -- worker_pool 可以理解为 生产-消费者 模型
    1. __queue_work 作为生产者; 把 work_struct 节点入列 worker_pool
    2. worker_thread 作为消费者; 遍历 worker_pool 的节点列表执行
