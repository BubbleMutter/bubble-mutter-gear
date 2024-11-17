[TOC]
# struct file
```c++
struct file {
    union {
        // 文件对象链表
        struct llist_node    fu_llist;
        // 释放后的 rcu 链表
        struct rcu_head     fu_rcuhead;
    } f_u;
    // struct path 其中包含 struct dentry
    struct path        f_path;  // 指向 dentry
    struct inode        *f_inode; // inode
    // 操作方法
    const struct file_operations    *f_op;
    // 对象锁; NOTE: 不用于 flock() 系统调用
    // flock() 系统调用使用 inode->i_flock
    spinlock_t        f_lock;
    // 引用计数
    atomic_long_t        f_count;
    // open() 传入的标记
    unsigned int         f_flags;
    // 访问权限, 如 644
    fmode_t            f_mode;
    // 文件指针 锁
    struct mutex        f_pos_lock;
    // 文件指针
    loff_t            f_pos;
    // 私有数据; 用于 fs驱动层 or 网络 socket
    void            *private_data;
    // mmap() 系统调用
    struct address_space    *f_mapping;
#ifdef CONFIG_EPOLL
    // epoll() 系统调用的,
    struct list_head    f_ep_links;
    struct list_head    f_tfile_llink;
#endif /* #ifdef CONFIG_EPOLL */
};
```

1. 进程中每个打开的文件, 关联对应的 `struct file` 对象
2. 这些 `struct file` 对象由 `struct files_struct` 文件表索引起来
3. 同一个文件在同一个进程中, 可能有多个 `struct file` 对象
   1. 均指向同一个的 `struct inode` 和 `struct dentry`
4. 同一个文件在不同的进程中, 对应着不同的 `struct file` 对象
   1. 均指向同一个的 `struct inode` 和 `struct dentry`
5. 文件是否 dirty 的标记在 `struct inode` 中

# struct file_operations
```c++
struct file_operations {
    struct module *owner;
    // 系统调用 `llseek()`
    loff_t (*llseek) (struct file *, loff_t, int);
    // 系统调用 `read()`; 给定文件偏移 loff_t, 读取 size_t 字节数据到 __user* 缓存
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    // 系统调用 `write()`; 给定文件偏移 loff_t, 从 __user* 缓存中 写入 size_t 字节数据
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    // 系统调用 `aio_read()`
    ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
    // 系统调用 `aio_write()`
    ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
    // 系统调用 `poll()` 遍历传入 fd 数组的每个 fd
    unsigned int (*poll) (struct file *, struct poll_table_struct *);
    // 系统调用 `ioctl()`; 优先实现 unlocked_ioctl
    long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
    // 系统调用 `ioctl()`; 用于在 32位程序, 在64位系统上的 安全实现; 主要解决字节大小问题
    long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
    // 系统调用 `mmap()`
    int (*mmap) (struct file *, struct vm_area_struct *);
    // 系统调用 `open()`
    int (*open) (struct inode *, struct file *);
    // 系统调用 `flush()`
    int (*flush) (struct file *, fl_owner_t id);
    // 系统调用 `close()` 只有当引用为0时, 才会真正地释放, 因为 多个fd 可能指向同一个 struct file
    int (*release) (struct inode *, struct file *);
    // 系统调用 `fsync()` 主动同步到磁盘
    int (*fsync) (struct file *, loff_t, loff_t, int datasync);
    // 系统调用 `aio_fsync()` 主动同步到磁盘
    int (*aio_fsync) (struct kiocb *, int datasync);
    // 系统调用 `flock()` 实现
    int (*lock) (struct file *, int, struct file_lock *);
    int (*flock) (struct file *, int, struct file_lock *);
};
```
1. vfs 提供部分接口的默认实现, fs驱动的定义 `file_operations` 时 使用默认接口 or 自行实现
   1. `.read` 接口 = `do_sync_read` / `new_sync_read`
   2. `.write` 接口 = `do_sync_write` / `new_sync_write` / `vfs_write`
   3. `.llseek` 接口 = `generic_file_llseek` / `default_llseek`
2. fs 驱动的定义, 可通过正则式 `struct *file_operations [a-zA-Z_]* =` 搜索
3. 例如
   1. `struct file_operations pipefifo_fops` 作为 `pipe()` 的底层实现
   2. `struct file_operations generic_ro_fops` vfs 提供默认的 read-only 实现
   3. `struct file_operations simple_dir_operations` vfs 提供默认的 目录fd实现
   4. `struct file_operations ext4_file_operations` fs驱动 ext4 的实现
   5. `struct file_operations debugfs_file_operations` fs驱动 procfs 实现
