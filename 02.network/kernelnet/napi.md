[TOC]
https://wiki.linuxfoundation.org/networking/napi
https://lwn.net/Articles/30107/

# 原理
1. 当网卡大量收包; 不要每收一个包就取包上协议栈
   而是在收到足够多的包之后, "通知"软中断
   在软中断中回调中一次把该网卡多个包取出上协议栈

# 对象定义
```c++
struct softnet_data {
    // ...
    struct list_head poll_list;
    struct napi_struct backlog;
    // ...
	struct sk_buff_head	input_pkt_queue;
    struct sk_buff_head	process_queue;
    // ...
};

struct napi_struct {
	struct list_head	poll_list;
	int			(*poll)(struct napi_struct *, int);
};

static int __init net_dev_init(void) {
    // ...
    for_each_possible_cpu(i) {
        // ...
        sd->backlog.poll = process_backlog;
        // ...
    }
    // ...
}
```
1. `softnet_data.poll_list` 待 poll 的 napi 网卡队列
    1. 节点对象是 `struct napi_struct`
    2. 其中 `poll` 对象方法, 在 `net_rx_action` -> `napi_poll` 中调用
2. `softnet_data.backlog` 内置 napi 设备
    1. 用于 非 napi 网卡的收包 (虚拟网卡都是非 napi)
    2. 其中 `poll` 对象方法设置为 `process_backlog`
    3. 网卡收包中断回调, 调用 `enqueue_backlog()` 把 skb 入列 `softnet_data.input_pkt_queue`
    4. 而在 `process_backlog()` 中处理 `softnet_data.input_pkt_queue` 的 skb
3. `struct napi_struct` 支持 napi 的网卡实现 `poll` 对象方法
    1. 在 `poll` 方法中检查收包是否达到阈值包个数, 检查是否超过阈值时间
    2. 满足取包条件后, 从 DMA 中一次多次取包
4. `enqueue_to_backlog()`: skb 入列 **input_pkt_queue**
5. `process_backlog()`: **input_pkt_queue** 先 slice 到 **process_queue** 再处理

# 运行机制
## napi_schedule() -> raise(NET_RX_SOFTIRQ) (入列 softnet_data.poll_list)
```c++
static inline void napi_schedule(struct napi_struct *n) {
    /* 检查 n->state
     * a) NAPIF_STATE_DISABLE 使能
     * b) NAPIF_STATE_SCHED   防重入 */
	if (napi_schedule_prep(n))
		__napi_schedule(n);
}
void __napi_schedule(struct napi_struct *n) {
    /* 关硬中断, 避免硬中断嵌套,
     * 实际上是, 避免硬件收包个数超过给定 budget, 以及中断处理冲突
     * 关掉中断, 调度过程中, 如果系统过载, 就会丢包
     * 过载本质: 调度过程中, 硬件过快地填满DMA,
     *          但 DMA 还没来得及处理时硬件继续收到包 */
	local_irq_save(flags);
	____napi_schedule(this_cpu_ptr(&softnet_data), n);
	local_irq_restore(flags);
}
static inline void ____napi_schedule(struct softnet_data *sd, struct napi_struct *napi) {
    /* 每个 cpu 有自己的 软中断矢量,
     * a) 把传入 napi 对象, 入列当前 cpu 的 napi 队列
     * b) 拉起收包 软中断
     *    note: 一般是通过 硬中断跑进 napi_schedule, 所以这里会延后收包 */
	list_add_tail(&napi->poll_list, &sd->poll_list);
	__raise_softirq_irqoff(NET_RX_SOFTIRQ);
}
```
1. 网卡收包中断回调; 当收包个数达到阈值; 把 napi 句柄入列 `softnet_data.poll_list`

## net_rx_action() -> napi_poll() (遍历 softnet_data.poll_list)
```c++
static __latent_entropy void net_rx_action(struct softirq_action *h) {
	struct softnet_data *sd = this_cpu_ptr(&softnet_data);
	unsigned long time_limit = jiffies + usecs_to_jiffies(netdev_budget_usecs);
	int budget = netdev_budget; /* 这个值是全局固定的, 没有默认接口修改 */
	LIST_HEAD(list);
	LIST_HEAD(repoll);

    /* 关掉硬中断, splice 取出当前 cpu 的所有 napi 节点 */
	local_irq_disable();
	list_splice_init(&sd->poll_list, &list);
	local_irq_enable();

	for (;;) {
		struct napi_struct *n;

		if (list_empty(&list)) { /* napi 任务全部完成, 退出循环 */
            // ...
			break;
		}
        /* 拿出一个 napi 节点 */
		n = list_first_entry(&list, struct napi_struct, poll_list);
        /* a) 根据 n->state 防重入, 执行 napi->poll 回调
         * b) 对于没有执行的 poll 的 napi 节点, 传出到 repoll 中 */
		budget -= napi_poll(n, &repoll);

		/* If softirq window is exhausted then punt.
		 * Allow this to run for 2 jiffies since which will allow
		 * an average latency of 1.5/HZ.
		 */
		if (unlikely(budget <= 0 /* 限制一次 rx_action 处理的包个数 */ ||
			     time_after_eq(jiffies, time_limit) /* 限制一次 rx_action 执行的时间 */)) {
			sd->time_squeeze++;
			break;
		}
	}

    /* a) 关掉硬中断
     * b) 把 当前 cpu 的所有 napi 节点 取出  (这部分节点是  执行 poll 时硬中断触发提交的)
     * c) 把 repoll   的所有 napi 节点 取出  (这部分节点是  执行 poll 时不处理的)
     * d) 把 之前两个步骤的 napi 节点取出, 重新放回 当前 cpu 的所有 napi 队列 */
	local_irq_disable();
	list_splice_tail_init(&sd->poll_list, &list);
	list_splice_tail(&repoll, &list);
	list_splice(&list, &sd->poll_list);

    /* 当前 cpu 的所有 napi 队列还还有, 则重新拉起收包软中断 */
	if (!list_empty(&sd->poll_list))
		__raise_softirq_irqoff(NET_RX_SOFTIRQ);
    /* 重新开启硬终端 */
	net_rps_action_and_irq_enable(sd);
    // ...
}

static int napi_poll(struct napi_struct *n, struct list_head *repoll) {
    // ...
	work = 0;
	if (test_bit(NAPI_STATE_SCHED, &n->state)) {
		work = n->poll(n, weight);
        // ...
	}
    // ...
}
```
1. 遍历 `softnet_data.poll_list` 取得 `struct napi_struct`
2. 通过 `napi_poll()` 调用网卡驱动回调 `poll` 取包
3. `net_rx_action` 的公平性:
    1. 每个 napi 对象 每次只能取有限个包
    2. 每次 `net_rx_action()` 只能取有限个包
    3. 每次 `net_rx_action()` 只执行有限时间

## netif_rx
1. 函数流程:
    1. 当 rps 使能时 `enqueue_backlog()` 到对应 cpu
    2. 当 rps 禁用时 `enqueue_backlog()` 到当前 cpu
2. 用于非 napi 网卡收包中断中, 调用该函数实现默认的 poll 机制
    1. 实际上, 这类网卡在每个收包中断中马上从 DMA 中取包

## netif_receive_skb() -> netif_receive_skb_internal()
1. 函数流程:
    1. 当 rps 使能时 `enqueue_backlog()` 到对应 cpu
    2. 当 rps 禁用时 `netif_receive_skb()` 收包上协议栈
2. 用于带 napi 网卡的 `napi_struct` 成员函数 `poll()` 中
    1. 实际上, 这类网卡在满足"阈值"后, 才一次性从 DMA 中取N个包
3. 用于自实现中断下半部的网卡, 在下半部收包上协议栈
    1. 比如说, 绑定CPU核的内核线程 满转收包 ()

# example
```c++
/**
 * @brief 设备创建注册前, 加入到 napi 中
 */
static int device_create() {
    struct net_device *netdev;
    netif_napi_add(netdev, netdev_priv(netdev)->napi, netdev_rx_poll, budget);
    register_netdev(netdev);
}

/**
 * @brief 设备销毁注销后, 移出 napi,
 * @note  free_netdev 包含该逻辑, 不需要手动移出
 */
static int device_delete() {
    unregister_netdev(netdev);
    free_netdev(dev); /* free_netdev(dev) will call netif_napi_del() */
}

/**
 * @brief 网卡 up 回调中, 使能 napi, 并执行一次调度
 */
static int netdev_open(struct net_device *netdev) {
	netif_carrier_on(netdev);
	napi_enable(netdev_priv(netdev)->napi);
    napi_schedule(netdev_priv(netdev)->napi);
}

/**
 * @brief 网卡 down 回调中, 关掉 napi
 */
static int netdev_close(struct net_device *netdev) {
    netif_carrier_off(netdev);
    napi_disable(netdev_priv(netdev)->napi);
}

/**
 * @brief 收包中断中, 只调用 napi_schedule, 有内核判断调用 poll 回调的时机
 */
static int netif_hard_receive() { /* 硬中断回调 */
    /* 使用 napi 不要直接在硬中断回调中 关掉中断,
     * 因为 napi 就是为了提高吞吐量而用的 */
    napi_schedule(netdev_priv(netdev)->napi);
}

/**
 * @brief poll 回调; 从硬件中获取 数据, 构造 skb
 * @param  budget 硬件poll最大包个数
 * @note  函数中, 拿到数据后收包, 必须用 netif_receive_skb
 *        出错了, 需要清理 硬件poll缓存 和 软件napi缓存
 */
int netdev_rx_poll(struct napi_struct *napi, int budget) {
    int receive_packets = 0;
    while (receive_packets < budget) {
        skb = get_skb_phys_to_virt(dma); /* 从硬件中获取; 这里面有中断屏蔽逻辑 */
        if (!skb) {
            goto error;
        }
        netif_receive_skb(skb); // poll 回调中不能用 netif_rx
        receive_packets++;
    }

    return receive_packets;
error:
    hard_complete(netdev_priv(netdev)); /* 硬件 poll 逻辑清理 */
    napi_complete(netdev_priv(netdev)->napi);
    return receive_packets;
}
```

# question
1. `napi_gro_*` 是什么东西?
2. 完整 napi 接口在收包协议栈种如何执行的 ?
```c++
// scheduel
bool napi_disable_pending(struct napi_struct *n);
bool napi_schedule_prep(struct napi_struct *n);
void napi_schedule(struct napi_struct *n);
bool napi_reschedule(struct napi_struct *napi);
void napi_complete(struct napi_struct *n);

// napi global table: add/get/del
struct napi_struct *napi_by_id(unsigned int napi_id);
void napi_hash_add(struct napi_struct *napi);
void napi_hash_del(struct napi_struct *napi);

// napi
void napi_disable(struct napi_struct *n);
void napi_enable(struct napi_struct *n);
void napi_synchronize(const struct napi_struct *n);

// napi gro
gro_result_t napi_gro_receive(struct napi_struct *napi, struct sk_buff *skb);
int napi_gro_complete(struct sk_buff *skb);
void napi_gro_flush(struct napi_struct *napi, bool flush_old);

gro_result_t napi_skb_finish(gro_result_t ret, struct sk_buff *skb);
napi_reuse_skb(struct napi_struct *napi, struct sk_buff *skb);

gro_result_t napi_gro_frags(struct napi_struct *napi);

struct sk_buff *napi_get_frags(struct napi_struct *napi);
void napi_free_frags(struct napi_struct *napi);
```