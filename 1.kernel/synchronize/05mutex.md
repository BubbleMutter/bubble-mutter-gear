# mutex
# 方法
```c++
struct mutex {
	/* 1: unlocked, 0: locked, negative: locked, possible waiters */
	atomic_t		count;
	spinlock_t		wait_lock;
	struct list_head	wait_list;
};
void mutex_lock(struct mutex *lock);
int __must_check mutex_lock_interruptible(struct mutex *lock);
int __must_check mutex_lock_killable(struct mutex *lock);
int mutex_trylock(struct mutex *lock);
void mutex_unlock(struct mutex *lock);
```
1. mutex 的本质时 count 只能为 1 的 semphore
2. 与 semphore 相同, 通过 wait_list 实现等待进程先到先得的特性
