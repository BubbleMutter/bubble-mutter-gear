[TOC]
# struct inode 部分成员解析
```c++
struct inode {
    umode_t i_mode;  /* 操作权限 */
    const struct inode_operations *i_op;
    struct super_block    *i_sb;
    struct address_space *i_mapping;

    unsigned long i_ino; /* 全局唯一的 inode uuid */

    unsigned int i_nlink;     /* 硬连接数 */
    dev_t i_rdev;             /* 设备驱动层的 ID */
    loff_t i_size;            /* 文件大小内容, 单位 bytes */
    struct timespec i_atime;  /* 最后访问时间 */
    struct timespec i_mtime;  /* 最后修改时间 */
    struct timespec i_ctime;  /* 最后改变时间 */
    spinlock_t i_lock;
    unsigned short i_bytes;   /* 包括 metadata 的文件大小 */
    blkcnt_t i_blocks;        /* 块计数 */

    // i_hash 和 i_list 指向全局 inode 表
    struct hlist_node i_hash;    /* 索引节点散列表 */
    struct list_head i_lru;      /* 索引节点链表 */
    struct list_head i_sb_list;  /* super_block 链表 */
    union {
        struct hlist_head i_dentry;  /* 目录项 list */
        struct rcu_head i_rcu;
    };
    u64 i_version;  /* 版本号; 这个是给 fs 用的; 不是 vfs 用的 */
    atomic_t i_count;  /* 引用计数 */

    const struct file_operations *i_fop; /* former ->i_op->default_file_ops */
    struct file_lock *i_flock;           /* flock 系统调用 */
    struct address_space i_data;

    union {  /* 设备文件对应的 inode; 以下三种是互斥的, 或者都不是 */
        struct pipe_inode_info *i_pipe;  /* pipe */
        struct block_device *i_bdev;     /* block device */
        struct cdev *i_cdev;             /* char device */
    };

    // 用于 inotify 系统调用
    __u32 i_fsnotify_mask; /* 监听信息掩码 */
    struct hlist_head i_fsnotify_marks; /* 监听标记, 即事件发生后, 遍历通知 */

    void *i_private;  /* fs 私有数据 */
};
```

# struct inode_operations 部分成员解析
```c++
struct inode_operations {
    struct dentry * (*lookup) (struct inode *,struct dentry *, unsigned int);
    void * (*follow_link) (struct dentry *, struct nameidata *);   /* vfs 调用; 查找符号连接指向的 inode */
    int (*permission) (struct inode *, int);
    struct posix_acl * (*get_acl)(struct inode *, int);

    int (*readlink) (struct dentry *, char __user *,int);           /* syscall readlink 解析软连接 */
    void (*put_link) (struct dentry *, struct nameidata *, void *); /* vfs 调用; follow_link 返回对象句柄后; 销毁 */

    int (*create) (struct inode *,struct dentry *, umode_t, bool);  /* syscall open(O_CREAT) 和 create 回调 */
    int (*link) (struct dentry *,struct inode *,struct dentry *);   /* syscall link 创建硬连接 */
    int (*unlink) (struct inode *,struct dentry *);                 /* syscall unlink 在目录中删除索引节点 */
    int (*symlink) (struct inode *,struct dentry *,const char *);   /* syscall symlink 创建软连接 */
    int (*mkdir) (struct inode *,struct dentry *,umode_t);          /* syscall mkdir 创建一个新目录 */
    int (*rmdir) (struct inode *,struct dentry *);                  /* syscall rmdir 删除目录中的一项 */
    int (*mknod) (struct inode *,struct dentry *,umode_t,dev_t);    /* syscall mknod 创建设备文件 */
    int (*rename) (struct inode *, struct dentry *,
            struct inode *, struct dentry *);                       /* syscall rename 移动文件 */
    int (*rename2) (struct inode *, struct dentry *,
            struct inode *, struct dentry *, unsigned int);
    int (*setattr) (struct dentry *, struct iattr *);               /* 事件回调; inode 被修改后; notify_change 调用 */
    int (*getattr) (struct vfsmount *mnt, struct dentry *, struct kstat *); /* 事件回调; inode 需要从磁盘更新后; vfs 调用 */
    // 文件扩展属性的 设置/读取/删除 方法 */
    int (*setxattr) (struct dentry *, const char *,const void *,size_t,int);
    ssize_t (*getxattr) (struct dentry *, const char *, void *, size_t);
    ssize_t (*listxattr) (struct dentry *, char *, size_t);
    int (*removexattr) (struct dentry *, const char *);
    int (*fiemap)(struct inode *, struct fiemap_extent_info *, u64 start,
              u64 len);
    int (*update_time)(struct inode *, struct timespec *, int);
    int (*atomic_open)(struct inode *, struct dentry *,
               struct file *, unsigned open_flag,
               umode_t create_mode, int *opened);
    int (*tmpfile) (struct inode *, struct dentry *, umode_t);
    int (*set_acl)(struct inode *, struct posix_acl *, int);
};
```