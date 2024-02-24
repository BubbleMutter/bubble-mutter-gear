[TOC]
# DPDK
+ Introduction
    + https://zhuanlan.zhihu.com/p/344194786
+ Features:
    + DPDK 主要的任务都是在用户态, 际上四层负载均衡并不需要完整的协议栈，但是需要基本的网络组件
    + Master/Worker模型, Master/Worker 线程绑定到一个 CPU 物理核心上, 并且禁止这些 CPU 被调度, 以此避免上下文切换
    + 利用DPDK 的 API 实现一个网卡的一个收发队列对应一个CPU核心和一个Worker进程
    + 连接表无锁化 = 不同的网络数据流按照 N 元组被定向到不同的CPU核心, 每个CPU核心维护的是不相同的连接表
    + 全局表无锁化 = 通过跨CPU通信(dpdk:rte_ring)来实现的跨CPU无锁同步, 每个CPU核心需要维护同样的表 (邻居表 和 路由表)
+ performance optimize strategy (iQiYi)
    + Share Nothing = no lock, no cpu switching
    + Polling (replace interrupt)
    + Rx Steering (no kernel)
    + Zero Copy = Skip Kernel Space
    + Batching = CPU SIMD 指令集
    + mempool (replace alloc/free)
    + NUMA Awareness
    + Huge Page (htb)

# many core issues
1. 32 cores, dpdk need large mempool, n=2097120 and cache_size=512
```c++
mp = rte_pktmbuf_pool_create(poolname,
                2097120 /* n elements */,
                512 /* cache_size */,
                sizeof(struct mbuf_priv), RTE_MBUF_DEFAULT_BUF_SIZE, 0 /* numa 0 */);
rte_eth_rx_queue_setup(port_id, qid, n, 0 /* numa 0 */, NULL, mp);
```