虚拟化，容器化，云端化
虚拟机 = VMWare, OpenStack, VirtualBox

虚拟机 使用 Hypervisor 隔离操作系统
容器   使用 CGroups    隔离进程集合

Docker Hub 最常用的 Registry

1. cgroup
2. overlayfs
3. namespace

# docker 理念
1. Build, Ship and Run => 搭建、发送、运行
2. Build once, Run anywhere => 搭建一次，到处能用

# docker 概念
1. 镜像 Image          = rootfs 压缩包
    1. 容器运行必要的 .exe, .so, .conf 和其他资源
    2. 没有动态文件 /dev /sys /proc 等
2. 容器 Container      = 动态运行的 image
    1. 内部包含 /dev /sys /proc
    2. 以及内部 活动的进程
3. 仓库 Repository
    1. 特殊镜像的存储地点
4. 服务 Registry
    1. 仓库管理

https://www.docker.com/
https://docs.docker.com/
https://github.com/docker
https://github.com/docker/cli
https://github.com/docker/docker-ce


软件包主要分为两个部分
docker-ce   服务端
docker-cli  客户端


# restriction
1. bridge + veth 模型下, 经过 iptables nat 转发
    1. 当连接数较多时, nat session 占用较多内存
    2. container 中的进程, 无法通过 ip 协议取得对端的网络信息
