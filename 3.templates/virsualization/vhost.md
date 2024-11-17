# vhost_net
```txt
                          |
                          |
nicdrv <-> bridge <-> tun | <-> vhost_net
                          |
                          |
                          |
```
1. vmm 在 host 端用户态, 通过 tun 与 host 的物理网络交互
2. vmm 在 实现 vhost_net 协议, 与 vm 的虚拟网络交互

# vfio
```txt
                |
nicdrv <-> vfio | <-> nicdrv-vm
                |
                |
                |
```
1. nicdrv 支持 vfio 功能
2. host 通过 vfio 驱动配置实现与 vm 的网卡驱动交互
