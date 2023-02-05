[TOC]
# dev->priv_flags (netdev_priv_flags)
```c++
enum netdev_priv_flags {
	IFF_802_1Q_VLAN			= 1<<0,  // vlan
	IFF_EBRIDGE			    = 1<<1,  // 桥
	IFF_BONDING			    = 1<<2,  // 聚合
    IFF_LOOPBACK			= 1<<3,  // 回环口
    IFF_DONT_BRIDGE			= 1<<6,
    IFF_PROMISC			    = 1<<8,  // 混杂模式; 用于把 forward/drop 的包也上协议栈
};
```

## 判断是否是 vlan 口; 取得 vlan_id
```c++
static inline bool is_vlan_dev(const struct net_device *dev) {
    return dev->priv_flags & IFF_802_1Q_VLAN;
}

u16 vlan_dev_vlan_id(const struct net_device *dev) {
	return vlan_dev_priv(dev)->vlan_id;
}
EXPORT_SYMBOL(vlan_dev_vlan_id);

void usage() {
    if (is_vlan_dev(dev)) {
        uint16_t vid = vlan_dev_vlan_id(dev) & VLAN_VID_MASK;
    }
}
```