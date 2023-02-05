[TOC]
# virtio
virtio 1.1 版本, 新增 packed 模式
原生 split 模式, 又叫兼容模式

主机下  dpdk      支持 虚机 { virtio-split 模式，virtio-packed 模式 } 两种模式
主机下  vhost-net 支持 虚机 { virtio-split 模式 }

virtio.ko
virtio_ring.ko
virtio_net.ko
virtio_blk.ko
virtio_pci.ko
