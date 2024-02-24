[TOC]
# rss (receive side scaling)
1. 旨在 nic 发挥 smp 的多核优势, 多核同时收包
2. rss 需要 驱动层 和 硬件层 同时支持
3. `/sys/class/net/$dev/queues/rx-$n` 表示 n 个队列
4. 对于 pcie 网卡; 观察 pci 总线是否支持多少个中断
   `ls /sys/bus/pci/devices/$bus/msi_irqs/`
5. 根据 `cpu 个数` 以及 `msi_irqs` 个数
    1. 绑定 cpu 收包中断数
    2. 申请 nic 收包队列数
6. 开启 irqbalance 精灵进程, 用于降低跨 cpu 传输的可能性
7. 收包队列 / 发包队列 通过 `ethtool -L $dev combined 4` 控制
	1. 其中, 网卡驱动回调根据传入 数字 进行实际的调整

# rps (receive packet steering) / rfs (receive flow steering)
源码走读 https://www.cnblogs.com/ouyangxibao/articles/12621172.html
解决 smp 网卡收包到用户态 cache miss 高, 从而提升效率
1. rps: 旨在网卡收包散列到多核
	+ https://lwn.net/Articles/362339/
	+ `netdev_rx_queue` + `rps_map`
2. rfs: 旨在相同协议流收包到相同核
	+ https://lwn.net/Articles/382428/
	+ `rps_sock_flow_table` + `rps_dev_flow_table`

# rps mgmt = enable / disable / show
```sh
# show
cat  /sys/class/net/eth0/queues/rx-*/rps_cpus
cat  /sys/class/net/eth0/queues/rx-*/rps_flow_cnt

# enable
find /sys/class/net/eth0/* -name 'rps_cpus'  -exec echo f > {} \;
find /sys/class/net/eth0/* -name 'rps_flow_cnt' -exec echo 4096 > {} \;
echo 16384 > /proc/sys/net/core/rps_sock_flow_entries

# disable
find /sys/class/net/eth0/* -name 'rps_cpus'     -exec echo 0 > {} \;
find /sys/class/net/eth0/* -name 'rps_flow_cnt' -exec echo 0 > {} \;
```

# rps 工作流程
1. 协议栈收包函数 `netif_rx()` 和 `netif_receive_skb`
    1. 调用 `get_rps_cpu()`
    2. `enqueue_to_backlog()`
2. 调用 `get_rps_cpu()` 计算处理的 cpu (由多个策略决定)
	1. 根据全局 `rps_sock_flow_table` hash(skb) 推断 (更高优先级)
	2. 根据设备 `rps_dev_flow_table`  hash(skb) 推断
3. 调通 `enqueue_to_backlog()` 入列到 percpu 的 `softnet_data.input_pkt_queue`
4. 软中断回调 `net_rx_action()` 中调用 `process_backlog()` 处理本地 cpu 的报文
5. 系统调用 `recvmsg()` 等, 在用户态收包后更新 `rps_sock_flow_table`

# rps 对象定义
```c++
struct net_device {
	char			name[IFNAMSIZ];
	struct netdev_rx_queue	*_rx;
	unsigned int		num_rx_queues;
	unsigned int		real_num_rx_queues;
};
struct netdev_rx_queue {
	struct rps_map __rcu		*rps_map;
	struct rps_dev_flow_table __rcu	*rps_flow_table;
	struct net_device		*dev;
    // ...
};
struct rps_map {
	unsigned int len;
	struct rcu_head rcu;
	u16 cpus[0];
};
struct rps_dev_flow_table {
	unsigned int mask;
	struct rcu_head rcu;
	struct rps_dev_flow flows[0];
};

// global
struct rps_sock_flow_table {
	u32	mask;
	u32	ents[0] ____cacheline_aligned_in_smp;
};
extern u32 rps_cpu_mask;  // roundup_pow_of_two(cpu数)-1
extern struct rps_sock_flow_table __rcu *rps_sock_flow_table;
```
1. 一个 net_device 有 `_rx[num_rx_queues]` 多个队列
	1. `ethtool -l $dev` 观察网卡队列数
	2. `ethtool -L $dev combined 4` 设置网卡队列数; 最终队列数由 硬件 回调设置
	   比如说 combined 4, 硬件可能针对性地 rx 设置为 4 or 8
2. 一个 netdev_rx_queue 有一个 `rps_map`
    1. 一个 `rps_map` 对应多个 `cpus[len]` 表示该队列绑定的 cpu
	2. `/sys/class/net/$dev/queues/rx-$n/rps_cpus` 配置 len
3. 一个 netdev_rx_queue 有一个 `rps_dev_flow_table`
    1. 一个 `rps_dev_flow_table` 对应多个 `flows[mask]` 表示该队列的流
	2.  `/sys/class/net/$dev/queues/rx-$n/rps_flow_cnt` 配置 mask 数
4. 全局 rps_sock_flow_table 记录用户态处理流(5元组)所在 cpu
	1. 一个 `rps_sock_flow_table` 存放多个 `ents[mask]` 表示入口数
	2. `/proc/sys/net/core/rps_sock_flow_entries` 配置 mask 数
	3. 用户态收包 syscall 中根据 skb 的 hash 值更新

# rps 驱动实现
```c++
#define alloc_etherdev(sizeof_priv) alloc_etherdev_mq(sizeof_priv, 1)
#define alloc_etherdev_mq(sizeof_priv, count) alloc_etherdev_mqs(sizeof_priv, count, count)

skb_record_rx_queue(skb, driver_skbmap(skb) % count);
```
1. 网卡驱动初始化
    1. 调用 `alloc_etherdev_mq(sizeof_priv, count);` count 指示队列数
    2. 如果网卡驱动不支持; 则调用 `alloc_etherdev(sizeof_priv)`
2. 网卡驱动收包上协议栈前
    1. 调用 `skb_record_rx_queue()` 设置 `skb->queue_mapping`