
# 工具
```s
ip netns help    # from iproute2
lsns             # from linux-util

apt install newpid
apt install bubblewrap
```

我们看到的 namespace
```s
~$ ls -l /proc/self/ns
cgroup -> 'cgroup:[4026531835]'
ipc -> 'ipc:[4026531839]'
mnt -> 'mnt:[4026531840]'
net -> 'net:[4026531992]'
pid -> 'pid:[4026531836]'
pid_for_children -> 'pid:[4026531836]'
user -> 'user:[4026531837]'
uts -> 'uts:[4026531838]'
```

保存 namespace
```s
mkdir ./ns.uts && mount --bind /proc/$pid/ns/uts ./ns.uts # 保留 uts namespace
mkdir ./ns.ipc && mount --bind /proc/$pid/ns/ipc ./ns.ipc # 保留 ipc namespace
```

系统调用 `clone()` 的参数
CLONE_NEWIPC    = new system v ipc namespace
CLONE_NEWNET    = new network namespace
CLONE_NEWNS     = new process mount namespace
CLONE_NEWPID    = new process-id namespace
CLONE_NEWUSER   = new user-id namepsace
CLONE_NEWUTS    = new UTS namespace 系统调用 `utsname()`
CLONE_NEWCGROUP = new process cgroup

系统调用 `int setns(int fd, int nstype);`
fd     = 对应 `/proc/$pid/ns/$namespace` 或 通过 mount 挂载保存的
nstype = 检查 namespace 对应的类型, uts/pid/user 等

系统调用 `unshare()`

Q: 每一个 namespace 有什么隔离效果呢?
Q: 是不是 fork > setns > exec 从而实现不同进程 attach 到同一个 namespace 中

# namespace 子系统功能描述
## namespace::ipc
## namespace::pid
## namespace::uts
## namespace::user
## namespace::mount
1. 每个进程有自己的挂挂在点
2. 所以 /proc/mounts 指向 /proc/self/mounts

## namespace::cgroup
## namespace::network