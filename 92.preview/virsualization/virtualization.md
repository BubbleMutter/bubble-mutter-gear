一台物理机上可以模拟出多台虚拟机（Virtual Machine，简称VM）
提供虚拟化的平台被称为VMM(Virtual Machine Monitor)
在其上运行的虚拟机被称为guest VM（客户机）
guset VM的运行模式可分为完全虚拟化（Full Virtualization）和类虚拟化（Para Virtualization）


如果一个基于硬件运行（native）的OS不需要修改任何代码就可以直接跑在VMM上，
也就是guest OS根本感知不到自己运行在一个虚拟化环境中（可以说VMM是骗术高明的），
这种就被称为“完全虚拟化”。在这种模式下，VMM需要正确处理guest所有可能的指令。
如果使用完全虚拟化，就需要通过二进制代码翻译（binary translation),
扫描并修改guest的二进制代码，将难以虚拟化的指令转换成支持虚拟化的指令（ABI级）

如果能直接修改guest的操作系统内核代码（API级），就可以使得内核避免产生这些难以虚拟化的指令，这就是“类虚拟化”.

的虚拟化技术的实现架构可分为三类:
hypervisor: VMM 是一个完备的操作系统，它除了具备传统操作系统的功能，还具备虚拟化功能。
            包括CPU、内存和I/O设备在内的所有物理资源都归VMM所有，因此VMM不仅要负责虚拟机环境的创建和管理，还承担着管理物理资源的责任。
            I/O设备种类繁多，管理所有设备就意味着大量的驱动开发工作。在实际的产品中
            厂商会根据产品定位，有选择的支持一些I/O设备，而不是对所有的I/O设备都提供支持。

host:  物理资源由 host OS 管理; 统操作系统并不是为虚拟化而设计的，因此本身并不具备虚拟化功能，实际的虚拟化功能由VMM来提供
       VMM作为host OS中一个独立的内核模块，通过调用host OS的服务来获得资源，实现CPU、内存和I/O设备的虚拟化。
       VMM创建出虚拟机之后，通常将虚拟机作为host OS的一个进程参与调度。
       优点: VMM不需要为各种I/O设备重新实现驱动，可以专注于物理资源的虚拟化
       缺点: 由于VMM是借助host OS的服务来操作硬件，而不是直接操作硬件，因此受限于host OS服务的支持，可能导致硬件利用的不充分。
       由Qumranet公司开发的KVM（Kernel-based Virtual Machine）就是属于host模型的; 2008年被Red Hat收购.
       随着越来越多的虚拟化功能被加入到Linux内核当中，Linux已经越来越像一个hypervisor了，从这个角度看，KVM也可以算是hypervisor模型了。

混合模型:
       为了利用现有操作系统的I/O设备驱动程序，
       VMM会将大部分的I/O设备交由一个运行在特权级别的虚拟机操作系统（Service OS）来处理，
       VMM自己则主要负责CPU管理和内存管理。
       优点: 既不需要另外开发I/O设备驱动程序，又可以通过直接控制CPU和内存实现对这些物理资源的充分利用
       缺点: guest OS的I/O请求发送到VMM后，VMM需要将这些请求转发到service OS，这无疑增加了上下文的开销
       混合模型的代表有 Xen, Acrn, minos


传统的x86架构的运行级别有4种，从ring 0到ring 3，
ring 0是最高特权级通常用于操作系统内核
ring 3是最低特权级，通常用于用户程序。


对于KVM，其基于的host OS（也就是linux内核）运行在root mode的ring 0级别，
host userspace运行在root mode的ring 3级别，还是ring 0和ring 3，和linux的传统用法一样，因而对linux来说不用做什么大的修改。
guest OS和guest Apps则分别运行在 none-root mode的ring 0和ring 3级别。
当 guest os 提权的 none-root-ring-0 则切到 root-ring-0 中


CPU 虚拟化: 
MEM 虚拟化: EPT (Intel); NPT (AMD)
I/O 虚拟化: Passthrough; Emulation

# cpu virtualization
虚拟机（guest VM）所使用的CPU 称虚拟CPU; vCPU
宿主机 (host ) 所适用的CPU 称物理CPU; pCPU
VMExit: none-root-mode 切到 root-mode
VMEntry: root-mode 切到 none-root-mode
VMCS: 形同进程调度时的上下文切换; guest VM 之间上下文切换需要保存的信息 (比进程复杂)
VMPTRLD/VMCLEAR: x86 汇编指令, 用于绑定/解除 pCPU 和 vCPU 的绑定关系
VMXON/VMXOF: x86 汇编指令, 用于开启/关闭虚拟化;
切换流程: VMXON > VMCS > VMPTRLD > VM Exit > ...
         VMXOFF < VMCS < VMCLEAR < VM Entry < ...
VT-x (Intel):
AMD-V (AMD):

# mem virtualization
1. 单个 VM 进程 虚拟地址空间是相同的 (GVA)
2. 不同 VM 进程 物理地址空间是相同的 (GPA)
3. 不同 VM 最终 物理地址空间是不同的 (HPA)

LA: Logical address (等同 VA)
VA: Virutal Address (等同 LA)
PA: Physical Address

GVA: Guest Virtual Address (VMM 中的 VA)
GPA: Guest Physical Address (VMM 中的 PA)
IPA: Intermediate Physical Address (VMM 中的 PA)
HPA: Host Physical Address
gPT: Guest Page Table (实际上就是 os 维护的 PT, 不过特指 Guest OS)
sPT: Shadow Page Table (VMM 维护的 GPA <-> HPA 的映射表)

EPT (Intel): EPT: Extended Page Table (Intel)
NPT (AMD): Nested Page Table (AMD)
EPT/NPT MMU: EPT/NPT 本质上是硬件 MMU 用于虚拟化; 每个CPU核都有一个
EPT = NPT

HOST memmap: VA <-> PA
Guest memmap: GVA <-> GPA <-> HPA
Guest memmap solution: a) sPT  b) EPT/NPT

sPT: a) Shadow Page Table, 维护 GPA <-> IPA 的映射
     b) VMM 为 每个 Guest OS 的每个进程多维护一个 Page Table;
     c) VMM 捕获 Guest OS 的 page fault 异常(page 增删将触发),
        经过 VMExit/VMEntry, 更新 sPT
     d) 单个 VM 中进程切换时, VMM 捕获上下文切换事件
        把相应进程的 sPT 更新到硬件 CR3-register
     e) cpu 和 mem 开销都非常大

EPT/NPT: a) 硬件上同时支持GVA <-> GPA <-> HPA的地址转换, 硬件本质上是一块扩展的 MMU
         b) VMM 为每个 VM 维护一个 EPT/NPT 即可, 记录 GPA<->HPA 的映射 (mem 消耗降低)
         c) 不同 VM 之间切换时, 需要 VM 的 EPT/NPT 更新到硬件 (? 这个操作是谁负责的, Host OS 还是 VMM ?)
         d) 单个 VM 进程切换时, 只需要把 gPT 更新到硬件, 形同 Host OS 的进程间切换
         e) 每当 VM 发生 page fault; 大部分情况只经过 Guest OS 处理即可, 无须切到 VMM 中 (cpu 消耗降低)

# I/O virtualization
IOMMU (Intel): MMU for dma device; 
VT-d (AMD): (Virtualization Technology for Direct I/O) MMU for dma device
SMMU (ARM): (System MMU) MMU for dma device
IOMMU = VT-d = SMMU: 不在单个 CPU 核中, 多核共享; 功能以及结构上不同于 MMU

Device Passthrough:
Device Emulation:

# virtio
1. guest device access flow:
   guest-userspace > guest-kenrelspace > VMExit > host-kernelspace
   host-kernelspace 切到 host-userspace; qemu 实现硬件 emulation
   host-kernelspace > VMEntry > guest-kenrelspace > guest-userspace
2. virtio 是一种标准协议, qemu 实现该协议, 简化 host 设备模拟 和 guest 驱动实现
3. 由于网络流量大, 针对网卡驱动进行额外优化
4. libvirt 提供管理 qemu 的 api, 包括 virtio_net / vhost_net

# net virtualization
网卡本质上可以通过 cpu mem i/o 的方案进行虚拟化
为了统一 VMM 的网卡实现, guest os 对 VMM 虚拟设备的驱动实现
而发展出更好的方案

virtio_net: 存在于用户态的网卡驱动  (qemu 实现)
vhost_net: 存在于内核态的网卡驱动 (kernel 实现)

virtio_net 数据流:
   guest::virtio_net -(VMExit)-> kvm::ioeventfd > qemu::virtio_net > 
   host::tap > host::bridge > hardware
   qemu::virtio_net > kvm::irqfd -(VMEntry)-> guest::virtio_net

vhost_net 数据流:
   guest::vhost_net -(VMExit)-> kvm > host::vhost_net >
   host::tap > host::bridge > hardware >
   host::vhost_net > kvm -(VMEntry)-> guest::virtio_net
1. `ps ax | grep vhost` 找到 `[vhost-$pid]` 的内核线程, $pid 为对应 qemu 进程
2. 内核驱动
3. vhost_net 的设备配置; 需要 qemu 负责
4. dataplane 透传, ctrlplane 由 qemu 负责


我初步了解了虚拟化原理, 包括 CPU MEM I/O 三个方面. 大概理解 vhost_net 的框架
我这边的疑问有
1. 我之前做的业务上云是通过 docker 部署, 网络方案是 veth + bridge
   我们业务除了这种方案以外还会用到 哪些其他的 ? (引深到下个问题)
2. 之前 tcp-zero-copy-recv 的讨论中, 提到 vhost_net
   我们实际适用 vhost_net 的方案是怎么样的 ?
   是用 qemu 虚拟化吗? 还是有其他场景?
备注:
```txt
vhost_net 数据面我理解到是这样子的, 后续再看 vhost_net 驱动代码
guest::vhost_net -(VMExit)-> kvm > host::vhost_net >
host::tap > host::bridge > hardware >
host::vhost_net > kvm -(VMEntry)-> guest::virtio_net
```