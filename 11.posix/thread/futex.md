[TOC]
man 2 futex
man 7 futex

Documentation/pi-futex.txt
Documentation/futex-requeue-pi.txt
Documentation/locking/rt-mutex.txt
Documentation/locking/rt-mutex-design.txt
Documentation/robust-futex-ABI.txt

# 内核态加锁阻塞的实现
futex 有 userspace 和 kernelspace, 这里是确定跑进 kernelspace 的讨论
## 代码 futex_lock_pi
```c++
static int futex_lock_pi(u32 __user *uaddr, unsigned int flags, ktime_t *time, int trylock) {
    // ...
retry:
	ret = get_futex_key(uaddr, flags & FLAGS_SHARED, &q.key, VERIFY_WRITE);
	if (unlikely(ret != 0))
		goto out;
retry_private:
	hb = queue_lock(&q);

	ret = futex_lock_pi_atomic(uaddr, hb, &q.key, &q.pi_state, current, 0);
	if (unlikely(ret)) {
		/*
		 * Atomic work succeeded and we got the lock,
		 * or failed. Either way, we do _not_ block.
		 */
		switch (ret) {
		case 1:
			/* We got the lock. */
			ret = 0;
			goto out_unlock_put_key;
		case -EFAULT:
			goto uaddr_faulted;
		case -EAGAIN:
			/*
			 * Two reasons for this:
			 * - Task is exiting and we just wait for the
			 *   exit to complete.
			 * - The user space value changed.
			 */
			queue_unlock(hb);
			put_futex_key(&q.key);
			cond_resched();
			goto retry;
		default:
			goto out_unlock_put_key;
		}
	}
    // ...
}
```
堆栈:
SYSCALL_DEFINE6(futex > 
do_futex >
futex_lock_pi

## 分析
1. 通过 get_futex_key 取得 uaddr 到 key 到映射
2. 原子操作 futex_lock_pi_atomic 尝试加锁
3. case 1: 取得锁, 返回用户态
4. case -EAGAIN: 锁已被占用, 先调度走, 然后重试
    + 所以, 线程/进程将一直阻塞在内核态, 且需要调度才能取得锁