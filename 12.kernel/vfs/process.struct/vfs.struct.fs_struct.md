[TOC]
# struct fs_struct
```c++
struct fs_struct {
    int users; /* 引用计数 */
    spinlock_t lock; // 自旋锁
    seqcount_t seq;  // 序列锁
    int umask;
    int in_exec;     // 可执行文件
    struct path root; // 进程所在命名空间的 根目录
    struct path pwd;  // 进程当前工作目录的 路径
};
```

1. 每个进程 `task_struct` 有一个 `fs_struct`, 用以记录 pwd 和 root
2. `clone()` 默认创建新的 `fs_struct`
3. `clone()` 传入 `CLONE_FS` 会共享同一个结构体
    1. `vfork()` 和 `pthread_create()` 从而共享进程 pwd 和 root