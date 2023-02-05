[TOC]
# struct files_struct (进程 open 的文件表)
```c++
struct files_struct {
    atomic_t count;
    // 指向 下一个 files_struct; 构造单项链表
    struct fdtable __rcu *fdt;
    struct fdtable fdtab;

    // 访问锁
    spinlock_t file_lock ____cacheline_aligned_in_smp;
    // 下一个可用的 fd 数值
    int next_fd;
    // exec() 时关闭的 fd 链表
    unsigned long close_on_exec_init[1];
    // 正在打开的 fd 链表
    unsigned long open_fds_init[1];
    // 指向打开的 fd 对应的 struct file 对象; 默认是 64
    struct file __rcu * fd_array[NR_OPEN_DEFAULT];
};
```

1. 每个进程 `task_struct` 有一个 `files_struct` 最多存储 `NR_OPEN_DEFAULT` 个fd
2. 当超过 `NR_OPEN_DEFAULT` 时, alloc 新的 `files_struct`, 通过成员变量 `fdt` 连接
3. `clone()` 默认创建新的 `files_struct`
4. `clone()` 传入 `CLONE_FILES` 会共享同一个结构体
    1. `vfork()` 和 `pthread_create()` 从而共享进程的 fd 表