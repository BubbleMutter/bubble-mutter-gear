[TOC]
# netdevice lookup
```c++
struct net {
    // ...
	struct hlist_head 	*dev_name_head;
	struct hlist_head	*dev_index_head;
    // ...
};

struct net_device *__dev_get_by_index(struct net *net, int ifindex);
struct net_device *__dev_get_by_name(struct net *net, const char *name);
```

1. 一个 netns 下分别有 name 和 ifindex 的 hash 索引
2. 通过 `__dev_get_by_index()` 和 `__dev_get_by_name()` 方法查询
3. 如果需要获取特殊网卡对象的引用, `register_netdevice_notifier()`
    + 监听 `NETDEV_REGISTER` 和 `NETDEV_UNREGISTER`
