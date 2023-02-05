[TOC]
# virtqueue
```log
tx: virtqueue_add_outbuf -> virtqueue_kick -> virtqueue_notify
rx: vring_interrupt -> virtqueue_add_inbuf -> virtqueue_get_buf
```

# virtqueue_add*
```c++
int virtqueue_add_outbuf(struct virtqueue *vq, struct scatterlist sg[],
                         unsigned int num, void *data, gfp_t gfp);

int virtqueue_add_inbuf(struct virtqueue *vq, struct scatterlist sg[],
                        unsigned int num, void *data, gfp_t gfp);

int virtqueue_add_inbuf_ctx(struct virtqueue *vq, struct scatterlist sg[],
                            unsigned int num, void *data, void *ctx, gfp_t gfp);

int virtqueue_add_sgs(struct virtqueue *vq, struct scatterlist *sgs[],
                      unsigned int out_sgs, unsigned int in_sgs,
                      void *data, gfp_t gfp);
```
1. out = from frontend to backend
    1. guest driver send message to host-os
2. in  = from backend to frontend
    2. guest driver interrupt-handler receive buffer
3. procedure
    1. extract a vring_desc
    2. set buffer into vring_desc (dealing address issue)
    3. append vring_desc to avail_ring (modify idx)

# virtqueue_kick / virtqueue_notify
```c++
bool virtqueue_kick(struct virtqueue *vq) {
    // ...
	return virtqueue_notify(vq);
}
bool virtqueue_notify(struct virtqueue *_vq) {
	struct vring_virtqueue *vq = to_vvq(_vq);
    // ...
    vq->notify(_vq);
}
```
1. guest-os notify host-os there are buf in the virtqueue.
2. `vq->notify()` implement by guest-os driver to notify backend. such as:
    1. `virtio_ccw_kvm_notify()` in `drivers/s390/virtio/virtio_ccw.c`
    2. `vm_notify()` in `drivers/virtio/virtio_mmio.c`
    3. `vp_notify()` in `drivers/virtio/virtio_pci_legacy.c`
    4. `virtio_vdpa_notify()` in `drivers/virtio/virtio_vdpa.c`

# virtqueue_get* / detach_buf*
```c++
void *virtqueue_get_buf_ctx(struct virtqueue *_vq, unsigned int *len, void **ctx);
void *virtqueue_get_buf(struct virtqueue *_vq, unsigned int *len);

void detach_buf_split(struct vring_virtqueue *vq, unsigned int head, void **ctx);
void detach_buf_packed(struct vring_virtqueue *vq, unsigned int id, void **ctx);
```
1. `virtqueue_get_inbuf()` guest-os driver obtain buf from virtqueue
2. `detach_buf*` to clear data after it has been obtained

# vring_interrupt
```c++
irqreturn_t vring_interrupt(int irq, void *_vq) {
	struct vring_virtqueue *vq = to_vvq(_vq);
    // ...
	vq->vq.callback(&vq->vq);
	return IRQ_HANDLED;
}
```
1. `vring_interrupt` vring 中断回调统一入口,
   在 virtio driver (如 virtio_net/virtio_mmio/...) 中断回调中调用
2. 最终调用 virtio driver 实现的 `vq.callback()`
3. 一个 virtio driver 有多个 virtqueue
   每个 virtqueue 独立被中断唤醒
   唤醒后统一回调 virtio driver 的 callback
4. 一般在 callback 中调用 `virtqueue_add_inbuf()`, 形同中断上半部取硬件数据
5. 然后在 virtio driver 的中断后半部 `virtqueue_get_inbuf()` 获取数据
