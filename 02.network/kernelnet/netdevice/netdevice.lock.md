[TOC]
# netdevice lock

# dev_base_list
```c++
DEFINE_RWLOCK(dev_base_lock);
```

# rtnl_mutex
```c++
static DEFINE_MUTEX(rtnl_mutex);

void rtnl_lock(void) {
	mutex_lock(&rtnl_mutex);
}

void __rtnl_unlock(void) {
    // ...
	mutex_unlock(&rtnl_mutex);
    // ...
}
```
1. `rtnl_unlock()` 不是实际上意义的 unlock
    1. 因为 `rtnl_unlock()` 调用 `netdev_run_todo()`
    2. `netdev_run_todo()` 完成 netdev 异步任务
    3. `netdev_run_todo()` 最终调用 `__rtnl_unlock()` unlock
