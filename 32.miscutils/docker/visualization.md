[TOC]
全虚拟化: vmware::二进制翻译技术, qemu::软件虚拟指令集
半虚拟化: kvm,
硬件虚拟化: VT-x AMD-V

x86 的虚拟化问题

容器
LXC: LinuX Container
cgroup + namespace

Full Virtualization:
Hypervisor (VWM) 协调 guest os 和 hardware, 捕获/处理 异常指令.

Para Virtualization:
guest os 提供虚拟化支持 (通常是驱动),
guest os 直接部分访问 hardware
Hypervisor (VWM) 无需捕获/处理 guest os 的异常指令
实际上是让 guest os 感知到自己处于虚拟化环境并针对特殊操作进行处理

OpenStack 的5个组件

这个起初由 NASA 和Rackspace 在 2010年末合作研发的开源项目
旨在打造易于部署、功能丰富且易于扩展的云计算平台
OpenStack项目的首要任务是简化云的部署过程并为其带来良好的可扩展性

KVM 集成到 Linux 内核的 Hypervisor
运行在支持硬件虚拟化的X86(Intel VT或AMD-V)架构
KVM 提供如任务调度, 内存管理与硬件设备交互等功能, 实现性能更优化的全虚拟化

kvm xen

虚拟化 > 云计算

# openstack 
## openstack layer1 ~ layer4
1. layer1 (基础设施层): nova(计算) glance(镜像) keystone(认证)
2. layer2 (扩展基础设施层): cinder(块存储) swift(对象存储) neutron(网络) designate(dns) ironic(裸机部署)
3. layer3 (可选增强特性层): ceilometer(计算) horizon(用户界面) barbican(秘钥管理)
4. layer4 (消费型服务): heat(Orchestration) Magnum(CaaS 容器) Sahara(大数据集群) Solum(应用开发) Murano(应用存储 如app store)
## openstack 组件
OpenStack Compute(Nova)  = 一套控制器，用于虚拟机计算或使用群组启动虚拟机实例;
OpenStack镜像服务(Glance) = 一套虚拟机镜像查找及检索系统，实现虚拟机镜像管理;
OpenStack对象存储(Swift)  = 一套用于在大规模可扩展系统中通过内置冗余及容错机制，以对象为单位的存储系统，类似于Amazon S3;
OpenStack Keystone       = 用于用户身份服务与资源管理
OpenStack Horizon        = 基于Django的仪表板接口 是个图形化管理前端

# openstack 和 docker 结合的项目
1. docker: PaaS Application Centric. 以 应用 为中心
2. openstack: IaaS Resource Centric. 以 资源 为中心
    + 为上层 PaaS 平台 提供存储, 网络, 计算等资源
## 1. Nova Docker Driver
## 2. Heat Docker Driver
## 3. Magnum
## 4. Murano
Mirantis贡献的，并且也进了OpenStack Namespace。
也和K8S集成了，用户可以通过Murano使用K8S的功能。
可以通过 Murano 部署 Pod、Service、Replication Controller 等。