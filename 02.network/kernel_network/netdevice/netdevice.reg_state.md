[TOC]
# dev->reg_state
```c++
enum { NETREG_UNINITIALIZED=0,
        NETREG_REGISTERED,	/* completed register_netdevice */
        NETREG_UNREGISTERING,	/* called unregister_netdevice */
        NETREG_UNREGISTERED,	/* completed unregister todo */
        NETREG_RELEASED,		/* called free_netdev */
        NETREG_DUMMY,		/* dummy device for NAPI poll */
} reg_state:8;

bool netdev_unregistering(const struct net_device *dev);
const char *netdev_reg_state(const struct net_device *dev);
```
1. `dev->reg_state` 记录注册状态
2. `netdev_reg_state()` 返回状态的描述