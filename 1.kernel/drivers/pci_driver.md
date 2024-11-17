[TOC]

# pci
1. pci layer 是 arch 相关的
2. debug
    + `/sys/class/pci_bus`
    + `/proc/bus/pci/devices`
3. 获取设备 pci_bus
    ```log
    anonymous@domain # realpath /sys/class/net/eth0 | awk -F/ '{print $(NF-2)}'
    0000:0c:00.0
    anonymous@domain # lspci -s 0000:0c:00.0
    0c:00.0 Ethernet controller: Realtek Semiconductor Co., Ltd. RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller
    ```

# struct pci_driver
```c++
struct pci_driver {
    // 全局 pci 设备链表节点
	struct list_head	node;
	const char		*name;
    // 设备的 pci id (即 lspci 看到的 vendor ID) 列表
	const struct pci_device_id *id_table;
    // 当关联 pci id 设备插入时, 由 pci layer 回调
	int  (*probe)(struct pci_dev *dev, const struct pci_device_id *id);
    // 当关联 pci id 设备移除时, 由 pci layer 回调
	void (*remove)(struct pci_dev *dev);
    // pm (电源管理) suspend 即 睡眠回调
	int  (*suspend)(struct pci_dev *dev, pm_message_t state);	/* Device suspended */
    // pm (电源管理) resume  即 唤醒回调
	int  (*resume)(struct pci_dev *dev);	/* Device woken up */
    // ...
};

/**
 * @brief 注册 pci_driver
 */
int pci_register_driver(struct pci_driver *drv);

/**
 * @brief 注册 pci_driver
 */
void pci_unregister_driver(struct pci_driver *drv);
```
1. `module_init()` 调用 `pci_register_driver()` 注册把 pci id 的回调
2. `module_exit()` 调用 `pci_unregister_driver()` 注销
3. 当 pci 系统遇到事件触发; 则匹配 pci id 表; 执行相关回调

# struct pci_dev
```c++

```

# struct pci_device_id
```c++

```

# /sys
```sh
# found pci driver directory
readlink /sys/class/net/$driver

# disable
echo 1 > /sys/devices/$bus/$main/$sub/remove

# rescan
echo 1 > /sys/devices/$bus/$main/rescan
```
