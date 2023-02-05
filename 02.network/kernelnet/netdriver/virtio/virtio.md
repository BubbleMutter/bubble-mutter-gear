[TOC]
# virtio framework
```log
{ virtio_blk/virtio_net/virtio_pci/virtio_scsi }-> { virtio } -> { virtio_ring } -> { backend }
```
1. frontend = running in guest-os    = device-driver
2. virtio = running in guest-os      = queue-abstract
3. virtio_ring = running in guest-os = queue-implement
4. backend = running in host-os = communicate between hardware and kernel

# virtio.ko { virtio_bus/virtio_dev_match/virtio_dev_probe }
```c++
struct bus_type virtio_bus = {
	.name  = "virtio",
	.match = virtio_dev_match,
	.dev_groups = virtio_dev_groups,
	.uevent = virtio_uevent,
	.probe = virtio_dev_probe,
	.remove = virtio_dev_remove,
};

virtio_dev_match(struct device *_dv, struct device_driver *_dr) {
	struct virtio_device *dev = dev_to_virtio(_dv);
	const struct virtio_device_id *ids;

	ids = drv_to_virtio(_dr)->id_table;
	for (i = 0; ids[i].device; i++)
		if (virtio_id_match(dev, &ids[i]))
			return 1;
	return 0;
}

virtio_dev_probe() {
	/* Figure out what features the driver supports. */
	driver_features = 0;
	for (i = 0; i < drv->feature_table_size; i++) {
        // ...
	}
    // ...
	/* Transport features always preserved to pass to finalize_features. */
	for (i = VIRTIO_TRANSPORT_F_START; i < VIRTIO_TRANSPORT_F_END; i++)
		if (device_features & (1ULL << i))
			__virtio_set_bit(dev, i);
    // ...
	if (drv->validate) {
		u64 features = dev->features;
        // ...
		err = drv->validate(dev);
    }
    // ...
	err = drv->probe(dev);
    // ...
	if (drv->scan)
		drv->scan(dev);
}
virtio_init() { bus_register(&virtio_bus); }
virtio_exit() {	bus_unregister(&virtio_bus); }

```
1. 初始化, 注册内核 bus 对象
2. 每当有设备事件, 内核调用 `virtio_dev_match()` 回调
   如果返回为 true, 则调用 `virtio_dev_probe()` 回调
3. `virtio_dev_match()`
   由内核bus层, 逐个遍历注册在 virtio_bus 下的驱动 (`/sys/bus/virtio/drivers`)
   往 `virtio_dev_match()` 传入 内核设备对象 以及 驱动句柄
   而 `virtio_dev_match()` 负责根据 驱动句柄的 id_table 匹配设备
4. `/sys/bus/virtio/drivers` 包括但不限于 `virtio_blk/virtio_net/virtio_pci`
5. `virtio_dev_probe()`
   先从 driver的features 和 dev的features 取交集,
   然后调用 driver 句柄的 validate 回调, 校验 features 的完备性
   调用 driver的probe回调 以及 driver的scan回调
   最终绑定并初始化单个设备

# struct virtio_device 结构 { virtqueue/vring }
```c++
struct virtio_device {
    // ...
	struct list_head vqs;
};

struct vring_virtqueue {
	struct virtqueue vq;
    // ...
    struct vring vring;
};

struct virtqueue {
	struct list_head list;
	struct virtio_device *vdev;
    // ...
};

```
1. 一个  virtio_device 包含多个 vring_virtqueue (链表连接)
2. 一个 vring_virtqueue 包含一个 virtqueue 和 一个 vring
3. virtqueue 中的 vdev 指向 virtio_device

# struct vring 结构 (vring_desc/vring_avail/vring_used/vring_used_elem) 
```c++
struct vring {
	unsigned int num;
	struct vring_desc *desc;
	struct vring_avail *avail;
	struct vring_used *used;
};

#define vring_used_event(vr) ((vr)->avail->ring[(vr)->num])
#define vring_avail_event(vr) (*(__virtio16 *)&(vr)->used->ring[(vr)->num])

static inline unsigned vring_size(unsigned int num, unsigned long align) {
	return ((sizeof(struct vring_desc) * num + sizeof(__virtio16) * (3 + num)
		 + align - 1) & ~(align - 1))
		+ sizeof(__virtio16) * 3 + sizeof(struct vring_used_elem) * num;
}

struct vring_avail {
	__virtio16 flags;
	__virtio16 idx;
	__virtio16 ring[];
};

struct vring_used {
	__virtio16 flags;
	__virtio16 idx;
	struct vring_used_elem ring[];
};

struct vring_used_elem {
	__virtio32 id;
	__virtio32 len;
};
```
1. 内存分布
    `vring|vring_desc[num]|vring_avail|ring[num]|vring_avail_event|vring_used|ring[num]|vring_used_event`
    + vring 头
    + vring_desc 数组
    + vring_avail 头
    + vring_avail.ring 数组
    + vring_avail_event 数值, 2bytes, 记录剩余个数
    + vring_used  头
    + vring_used.ring  数组
    + vring_used_event  数值, 2bytes, 记录剩余个数
2. struct vring_avail
    + idx 指向当前 ring数组成员下标 `ring[idx]`
    + `ring[idx]` 指向 vring_desc 对象 `desc[i]`
3. struct vring_used
    + idx 指向当前 ring 数组成员下标 `ring[idx]`
    + `ring[idx].id` 指向 vring_desc 对象 `desc[i]`
4. struct vring_desc
    ```c++
    struct vring_desc {
        __virtio64 addr;
        __virtio32 len;
        __virtio16 flags;
        __virtio16 next;
    };
    ```
    + 类似`struct page`, 记录 GPA
    + 成员 next 形同单项链表的 next指针, 指示 `desc[next]`
