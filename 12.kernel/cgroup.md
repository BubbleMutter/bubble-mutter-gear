[TOC]
cgroup    控制组, 进程集合 + subsystem参数 集合
subsystem 子系统，资源控制器, 使用cgroup提供的工具和接口来管理进程集合的内核模组
hierarchy 层级树, 负责一类的 subsystem. 包含同种配置的多个 "subsystem参数" 的集合.
cgroupfs 文件系统,

# 实现初探
task_struct 成员变量 `struct css_set __rcu *cgroups;` 指向一个 css_set `cat /proc/self/cgroup`
task_struct 成员变量 `struct list_head		cg_list;` 双向链表, 连接同一个 css_set 的所有 tasks
init/main.c 中, 在内核 bootup 时, 初始化 cgroup 和 css_set 结构体
task_struct 创建时, 绑定 css_set
task_struct 销毁时, 解绑 css_set

css_set 成员变量 `cgroup_subsys_state *subsys[CGROUP_SUBSYS_COUNT];`
一个 subsystem 只能从属于一个 hierarchy
一个 hierarchy 对应一个或多个 subsystem (相似资源)
一个 hierarchy 包含一个或多个 cgroup_subsys_state
一个 cgroup_subsys_state 对应一个或多个 hierarchy
一个 css_set 对应一个或多个不 cgroup_subsys_state
一个 css_set 最多对应一个 hierarchy 中的一个 cgroup_subsys_state
一个 css_set 包含多个 task_struct
一个 task_struct 只能对应一个 css_set

# subsystem 列表
1. cpu      = 限制进程 cpu 使用率
2. cpuacct  = 统计 cgropus 进程的 cpu 使用率
3. cpuset   = 分配 cgroups 进程的 cpu 节点和内存节点
4. memory   = 限制进程 mem 使用量
5. blkio    = 限制进程 块设备 io
6. devices  = 控制进程 访问指定设备
7. net_cls  = 标记 cgroups 进程的收发包, 使用 tc 系统控制标记的包
8. net_prio = 设计网络流量的优先级
9. freezer  = 挂起 或 恢复 cgroups 中的进程
10. ns      = 使得 cgroups 进程使用不同的 namespace
11. hugetlb = 限制进程 大页文件系统的使用

```s
~$ mount -t cgroup
cgroup on /sys/fs/cgroup/systemd type cgroup (rw,nosuid,nodev,noexec,relatime,xattr,name=systemd)
cgroup on /sys/fs/cgroup/devices type cgroup (rw,nosuid,nodev,noexec,relatime,devices)
cgroup on /sys/fs/cgroup/memory type cgroup (rw,nosuid,nodev,noexec,relatime,memory)
cgroup on /sys/fs/cgroup/cpu,cpuacct type cgroup (rw,nosuid,nodev,noexec,relatime,cpu,cpuacct)
cgroup on /sys/fs/cgroup/freezer type cgroup (rw,nosuid,nodev,noexec,relatime,freezer)
cgroup on /sys/fs/cgroup/rdma type cgroup (rw,nosuid,nodev,noexec,relatime,rdma)
cgroup on /sys/fs/cgroup/perf_event type cgroup (rw,nosuid,nodev,noexec,relatime,perf_event)
cgroup on /sys/fs/cgroup/net_cls,net_prio type cgroup (rw,nosuid,nodev,noexec,relatime,net_cls,net_prio)
cgroup on /sys/fs/cgroup/cpuset type cgroup (rw,nosuid,nodev,noexec,relatime,cpuset)
cgroup on /sys/fs/cgroup/pids type cgroup (rw,nosuid,nodev,noexec,relatime,pids)
cgroup on /sys/fs/cgroup/hugetlb type cgroup (rw,nosuid,nodev,noexec,relatime,hugetlb)
cgroup on /sys/fs/cgroup/blkio type cgroup (rw,nosuid,nodev,noexec,relatime,blkio)
# /sys/fs/cgroup            = cgroupfs
# /sys/fs/cgroup/$hierarchy = hierarchy
# /sys/fs/cgroup/$subsystem = subsystem

~$ ls /sys/fs/cgroup/cpu*
/sys/fs/cgroup/cpuacct -> cpu,cpuacct
/sys/fs/cgroup/cpu -> cpu,cpuacct
/sys/fs/cgroup/cpu,cpuacct
/sys/fs/cgroup/cpumask
# 可以看到同类 subsystem 通过软连接指向同一个 hierarchy
# 统一起见, 后续讨论这个路径统一使用 hierarchy 作为词汇
```

# notify_on_release release_agent
1. 当一个 hierarchy 下层节点全部删除 且 hierarchy 节点本身的 task 全部删除
2. 开启 notify_on_release 将会通知内核该 hierarchy节点 销毁事件
3. 然后执行顶层 hierarchy 的 release_agent 程序
4. 注意 notify_on_release 在每个 hierarchy 节点都有

```s
# release_agent 只存在于 顶层 hierarchy
~$ find /sys/fs/cgroup/ -name release_agent
/sys/fs/cgroup/blkio/release_agent
/sys/fs/cgroup/hugetlb/release_agent
/sys/fs/cgroup/pids/release_agent
/sys/fs/cgroup/cpuset/release_agent
/sys/fs/cgroup/net_cls,net_prio/release_agent
/sys/fs/cgroup/perf_event/release_agent
/sys/fs/cgroup/rdma/release_agent
/sys/fs/cgroup/freezer/release_agent
/sys/fs/cgroup/cpu,cpuacct/release_agent
/sys/fs/cgroup/memory/release_agent
/sys/fs/cgroup/devices/release_agent
/sys/fs/cgroup/systemd/release_agent
```

# 使用
```s
# 创建隔离组
mkdir /sys/fs/cgroup/$hierarchy/$group_name
# 销毁隔离组
rm -rf /sys/fs/cgroup/$hierarchy/$group_name
# 创建下层隔离组
mkdir /sys/fs/cgroup/$hierarchy/$group_name_l1/$group_name_l2

# hierarchy 本身以及每层隔离组都有相同参数项
# 下层的隔离组部分参数项受限与上层参数项
# hierarchy 最上层 tasks 文件包含未加入下层隔离组的 pid
cat /sys/fs/cgroup/cpu,cpuacct/cpuacct.usage
cat /sys/fs/cgroup/cpu,cpuacct/system.slice/cpuacct.usage
cat /sys/fs/cgroup/cpu,cpuacct/system.slice/NetworkManager.service/cpuacct.usage

# 参数配置, 直接 echo

# 进程加入 hierarchy 节点
echo $pid > /sys/fs/cgroup/cpu,cpuacct/$group_name_l1/$group_name_l2/tasks
# 进程退出 hierarchy 节点, 实际上是把进程放到最上层
echo $pid > /sys/fs/cgroup/cpu,cpuacct/tasks
```