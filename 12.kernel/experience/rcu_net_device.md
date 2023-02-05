## rcu 保护的 net_device 错误置空
```c++
dev_put(mtbl[id]);
mtbl[id] = NULL;
```
## 应该修正为
```c++
dev = mtbl[id];
rcu_assign_pointer(mtbl[id], NULL);
synchronize_rcu();
dev_put(dev);
```