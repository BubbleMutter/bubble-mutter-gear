[TOC]
# xps (transmit packet steering)
1. smp 场景下, 相同协议流发包 **net_tx_action** 和 **发包中断** 在相同核
2. 单核场景下, xps 无明显作用
## 对象定义
```c++
struct net_device {
	char			name[IFNAMSIZ];
	struct netdev_queue	*_tx;
	unsigned int		num_tx_queues;
	unsigned int		real_num_tx_queues;
};
```
