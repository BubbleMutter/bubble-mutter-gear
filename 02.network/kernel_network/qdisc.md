[TOC]
# 对象定义
```c++
struct softnet_data {
    // ...
	struct Qdisc		*output_queue;
	struct Qdisc		**output_queue_tailp; // 初始化: 指向 &sd->output_queue
	struct sk_buff		*completion_queue;
    // ...
};

struct Qdisc {
    // ...
	int (*enqueue)(struct sk_buff *skb, struct Qdisc *sch, struct sk_buff **to_free);
	struct sk_buff * (*dequeue)(struct Qdisc *sch);
	unsigned int flags;
    struct Qdisc *next_sched; // 用于构造单向链表
    // ...
};
```
1. output_queue     待发送队列; 形同收方向 `seftnet_data.poll_list`
2. completion_queue 已发送队列

# flow: net_tx_action -> qdisc* -> ...
```c++
void net_tx_action(struct softirq_action *h) {
    // ...
	if (sd->output_queue) {
		struct Qdisc *head;
        // ...
		head = sd->output_queue;
        // ...
		while (head) {
			struct Qdisc *q = head;
			head = head->next_sched;
            // ...
            qdisc_run(q);
        }
}

void qdisc_run(struct Qdisc *q) {
	if (qdisc_run_begin(q)) {
		__qdisc_run(q);
		qdisc_run_end(q);
	}
}

void __qdisc_run(struct Qdisc *q) {
    // ...
	while (qdisc_restart(q, &packets)) {
        // ...
	}
}

bool qdisc_restart(struct Qdisc *q, int *packets) {
    // ...
    return sch_direct_xmit(skb, q, dev, txq, root_lock, validate);
}
```
1. 发包软中断 `net_tx_action()` 遍历 `softnet_data.output_queue`
2. `qdisc_run()` ->  `__qdisc_run()` -> `qdisc_restart()`
3. `sch_direct_xmit()` -> `dev_hard_start_xmit()` -> `xmit_one()`
4. `netdev_start_xmit()` -> `__netdev_start_xmit()` -> `net_device_ops->ndo_start_xmit()`

# flow: dev_queue_xmit -> qdisc* -> ...
```c++
int dev_queue_xmit(struct sk_buff *skb) {
	return __dev_queue_xmit(skb, NULL);
}

int __dev_queue_xmit(struct sk_buff *skb, struct net_device *sb_dev) {
    // ...
    // 场景1: egress
	if (static_branch_unlikely(&egress_needed_key)) {
		skb = sch_handle_egress(skb, &rc, dev);
		if (!skb)
			goto out;
	}
    // ...
    // 场景2: qdisc
	txq = netdev_core_pick_tx(dev, skb, sb_dev);
	q = rcu_dereference_bh(txq->qdisc);
	if (q->enqueue) {
		rc = __dev_xmit_skb(skb, q, dev, txq);
		goto out;
	}
    // ...
    // 场景3: 直接发包
	if (dev->flags & IFF_UP) {
		int cpu = smp_processor_id(); /* ok because BHs are off */
        // ...
		if (READ_ONCE(txq->xmit_lock_owner) != cpu) {
            // ...
			if (!netif_xmit_stopped(txq)) {
				// ...
				skb = dev_hard_start_xmit(skb, dev, txq, &rc);
                // ...
            }
            // ...
        }
        // ...
    }
}
```
1. 协议栈转发函数 `dev_queue_xmit` -> `__dev_queue_xmit()`
2. `__dev_queue_xmit()` 有两个路线
    1. `sch_handle_egress()`; 去向可能是 丢包/xdp/转发
    2. `__dev_xmit_skb()` -> `qdisc_*` 走 qdisc 逻辑
    3. `dev_hard_start_xmit()` -> ...  直接走硬件发包

# 调度机制 __netif_schedule / __netif_reschedule / netif_wake_queue / netif_tx_wake_queue
```c++
void __netif_schedule(struct Qdisc *q) {
	if (!test_and_set_bit(__QDISC_STATE_SCHED, &q->state))
		__netif_reschedule(q);
}

static void __netif_reschedule(struct Qdisc *q) {
    // ...
	sd = this_cpu_ptr(&softnet_data);
	q->next_sched = NULL;
	*sd->output_queue_tailp = q;
	sd->output_queue_tailp = &q->next_sched;
	raise_softirq_irqoff(NET_TX_SOFTIRQ);
    // ...
}

// netdev_tx_completed_queue() -> netif_schedule_queue()
void netdev_tx_completed_queue(struct netdev_queue *dev_queue,
					     unsigned int pkts, unsigned int bytes);
void netif_schedule_queue(struct netdev_queue *txq);

// netif_wake_queue() -> netif_tx_wake_queue()
void netif_wake_queue(struct net_device *dev);
void netif_tx_wake_queue(struct netdev_queue *txq);
```
1. `__netif_schedule()` 和 `__netif_reschedule()` 主要逻辑
    1. 检查发包队列是否已经调度 (防重入)
    2. 把网卡对象入列 `softnet_data.output_queue`
    3. raise 发包软中断 `NET_TX_SOFTIRQ`
2. `__netif_schedule()` 的调用方式, 主要分为3种:
    1. 网卡 `netif_wake_queue()` -> `netif_tx_wake_queue()` -> `__netif_schedule()`
    2. 网卡 `netdev_tx_completed_queue()` -> `netif_schedule_queue()` -> `__netif_schedule()`
    3. `net/sched/` qdisc 发包调度系统直接调用
3. `netif_wake_queue()` / `netdev_tx_completed_queue()` 调用场景
    1. 网卡 `netif_stop_queue()` 后资源重新可用, 一般在发包结束的中断回调中 (`xxx_tx_complete()`)
    2. 网卡 发包超时回调 `net_device_ops.ndo_tx_timeout` 中重新唤醒发包队列
