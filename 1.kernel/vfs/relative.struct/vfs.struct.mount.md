[TOC]
# struct mount
```c++
struct mount {
    struct list_head mnt_hash; /* 全局挂载点的 hash 表 */
    struct mount *mnt_parent;  /* 上级挂载点 */
    struct dentry *mnt_mountpoint; /* 挂载点的目录项 dentry */
    int mnt_count; /* 引用计数 */

    struct list_head mnt_mounts;  /* 下级挂载点连表 */
    struct list_head mnt_child;   /* 下级挂载点连表 */
    const char *mnt_devname;      /* 设备名称; 如 /dev/sda1 */
    struct mnt_namespace *mnt_ns; /* 命名空间 */
};
```

1. 每一个 fs 被实际挂载, 则有一个关联的 vfsmount 对象被实例化
