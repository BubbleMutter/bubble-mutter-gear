[TOC]
# notifier
# implement
1. 事件源初始化一个链表头
    ```c++
    ATOMIC_NOTIFIER_HEAD(netevent_notif_chain);
    RAW_NOTIFIER_HEAD(netdev_chain);
    BLOCKING_NOTIFIER_HEAD(netlink_chain);
    ```
2. 事件回调节点
    ```c++
    /**
     * @action: integer value; most use for event-type
     * @data:
     * @return NOTIFY_DONE / NOTIFY_OK / NOTIFY_STOP / NOTIFY_BAD
     */
    typedef	int (*notifier_fn_t)(struct notifier_block *nb,
			unsigned long action, void *data);
    /**
     * @notifier_call: 回调函数
     * @next: linked-list node
     */
    struct notifier_block {
        notifier_fn_t notifier_call;
        struct notifier_block __rcu *next;
        int priority;
    };
    ```
3. 注册监听, 即在链表中添加节点
    ```c++
    // wrapper method
    int register_netdevice_notifier(struct notifier_block *nb);
    int register_netevent_notifier(struct notifier_block *nb);
    /**
     * @brief underlying method
     * @nl: linked-list head
     * @n: callback node
     */
    int notifier_chain_register(struct notifier_block **nl,
                                struct notifier_block *n);
    ```
4. 销毁监听, 即在链表中删除节点
5. 事件触发, 遍历链表中的函数
    ```c++
    // wrapper method
    int call_netdevice_notifiers_info(unsigned long val,
					                  struct netdev_notifier_info *info);
    int call_netevent_notifiers(unsigned long val, void *v);
    /**
     * @brief  underlying method
     * @nl: linked-list head
     * @val: notifier_fn_t param `action'
     * @v: notifier_fn_t param `data'
     */
    int notifier_call_chain(struct notifier_block **nl,
			                unsigned long val, void *v,
			                int nr_to_call, int *nr_calls);
    ```

# 监听示例 netdevice 事件
```c++
static int m_device_event(struct notifier_block *unused,
				unsigned long event, void *ptr) {
    struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	switch (event) {
	case NETDEV_REGISTER:
        printk("netdevice %s is registered\n", dev->name);
		break;
	case NETDEV_UNREGISTER:
        printk("netdevice %s is unregistered\n", dev->name);
		break;
	}
	return NOTIFY_DONE;
}

static struct notifier_block m_notifier_block = {
	.notifier_call	= m_device_event,
};

static void m_notifier_init(void) {
    register_netdevice_notifier(&m_notifier_block);
}

static void m_notifier_fini(void) {
	unregister_netdevice_notifier(&m_notifier_block);
}
```

# event 列表
## register_netdevice_notifier()
```c++
enum netdev_cmd {
	NETDEV_UP	= 1,	/* For now you can't veto a device up/down */
	NETDEV_DOWN,
    // ...
    NETDEV_REGISTER,
	NETDEV_UNREGISTER,
	NETDEV_CHANGEMTU,	/* notify after mtu change happened */
	NETDEV_CHANGEADDR,	/* notify after the address change */
};
```

## register_netevent_notifier()
> NETEVENT_NEIGH_UPDATE
> NETEVENT_DELAY_PROBE_TIME_UPDATE
