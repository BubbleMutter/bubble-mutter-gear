[TOC]
# struct mnt_namespace
```c++
struct mnt_namespace {
    // 引用计数
    atomic_t        count;
    unsigned int        proc_inum;
    // 根目录挂载点
    struct mount *    root;
    struct list_head    list;
    // 命名空间; 每个进程有独立的命名空间
    struct user_namespace    *user_ns;
    u64            seq;    /* Sequence number to prevent loops */
    // 轮询等待队列
    wait_queue_head_t poll;
    u64 event;
    // 命名空间中, 挂载点的 排序号
    unsigned int        mounts; /* # of mounts in the namespace */
    unsigned int        pending_mounts;
};
```

1. 每个进程 `task_struct` 有一个 `mnt_namespace`
2. `clone()` 调用, 默认继承同一个 `mnt_namespace`
3. `clone()` 调用, `CLONE_NEWS` 才会创建一个新的命名空间