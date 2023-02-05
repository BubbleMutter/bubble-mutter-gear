[TOC]
BUG: sleeping function called from invalid context
导致报错的直接原因是 在原子上下文中 调用休眠函数; 但是实际情况比较复杂
# 1. 可能1: 自旋锁没有解锁
```c++
dummy_callback() {
    spin_lock_irqsave();
    if (do_task()) {
        return -1;
    }
    spin_lock_irqrestore();
}
```
1. 示例代码中, 错误返回时, 没有还原 中断标记.
2. 使得 上下文标记位 一直认为在 原子上文中
3. 在后来其他地方调用休眠函数时, 会报错

# 2. 可能2: 在自旋锁加锁后 调用休眠函数
```c++
task() {
    spin_lock();
    might_sleep_function();
    spin_unlock();
}
```
1. 自选锁设计上是只运行在原子上下文的,
   使得加锁时标记为原子上下文,
   休眠函数是禁止的

# 3. 可能3: read_lock_bh write_lock_bh 没有解锁
+ 实战例子, 我们在 neigh_fill_info 里面新增一个rta字段
```c++
static int neigh_fill_info(struct sk_buff *skb, struct neighbour *neigh,
			   u32 pid, u32 seq, int type, unsigned int flags) {
    struct custom_rta_t custom_rta;
    // ...
    read_lock_bh(&neigh->lock);
    // ...
#if 1 /* 我们加的代码 */
    custom_rta_build(neigh, &custom_rta);
	if (nla_put(skb, NDA_CUSTOM, sizeof(custom_rta), &custom_rta)) {
		goto nla_put_failure;
	}
#endif

    read_unlock_bh(&neigh->lock);
    // ...
nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}
```
1. 当 arp 表巨大时, nla_put 将超过 socket 的收包缓存
    + 业务上需要上调收包缓存
2. 使得 nla_put(skb, NDA_CUSTOM) 将会失败
3. 但是没有解锁 `read_unlock_bh(&neigh->lock);`, 然后系统调用就返回了
4. 使得后面再有 进程的上下文(如系统调用) 调用可休眠函数时
5. 就会报错 BUG: sleeping function called from invalid context
6. 改动, 就是在 nla_put 失败后, 解锁`read_unlock_bh(&neigh->lock);`