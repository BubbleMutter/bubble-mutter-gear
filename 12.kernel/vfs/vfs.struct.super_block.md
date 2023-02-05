[TOC]
# struct super_block (部分成员说明)
```c++
struct super_block {
	struct list_head	s_list;             /* 连接 super_blocks */
	dev_t s_dev;		                    /* 指向 设备信息; 对应驱动 MKDEV() */
    unsigned char s_dirt;                   /* 脏数据标记 (存在为写入到磁盘的修改数据) */
    struct file_system_type	*s_type;        /* 文件系统类型 */
    const struct super_operations	*s_op;  /* 抽象方法, 由 filesystem 驱动实现 */
    unsigned long		s_flags;            /* 挂载标记 */
    struct dentry		*s_root;            /* 目录挂载点 */
    struct semaphore     s_lock;            /* 信号量; 互斥访问 */
    int                 s_count;            /* 引用计数 */
    int             s_need_sync;            /* 尚未同步标记 */
    struct list_head   s_inodes;            /* inodes 链表 */
    struct list_head   s_dirty;             /* 脏数据 链表 */
    struct blocK_device *s_bdev;            /* 相关的块设备 */
    struct mtd_info     *s_mtd;             /* 存储磁盘信息 */
};
static LIST_HEAD(super_blocks); // 所有 super_block->s_list
```
+ `super_block` 文件系统的metadata
    + 在存储设备分区开头
    + 挂载之后读取磁盘数据构造对象

# struct super_operations (部分成员说明)
```c++
struct super_operations {
    struct inode *(*alloc_inode)(struct super_block *sb);  /* 分配一个 inode */
    void (*clear_inode)(struct inode *inode);      /* 释放一个 inode */
    void (*dirty_inode)(struct inode *);           /* inode 有写入数据时的回调 */
    int (*write_inode)(struct inode *, int wait);  /* 把 inode 数据写入 disk */
    int (*drop_inode)(struct inode *);             /* 引用数减少到0时, 回调 */
    void (*delete_inode)(struct inode *);          /* 删除 disk 中的文件节点 */
    void (*put_super)(struct super_block *sb);     /* umount 时的回调; 减少引用数 */
    void (*write_super)(struct super_block *);    /* 更新 disk 中的 super_block 信息 */
    int (*sync_fs)(struct super_block *sb, int wait);  /* 同步所有 metadata 到 disk */
    int (*freeze_fs) (struct super_block *);       /* 加锁; 互斥操作的前置 */
	int (*unfreeze_fs) (struct super_block *);     /* 释放; 互斥操作的后置 */
	int (*statfs) (struct dentry *, struct kstatfs *);  /* 获取文件系统状态信息 */
	int (*remount_fs) (struct super_block *, int *, char *); /* remount 系统调用的回调 */
    void (*umount_begin) (struct super_block *); /* 在 mount 过程中进行 umount 的回调; NFS 会用到 */
};
void usage() {
    struct super_block *sb;
    /* get super_block */
    sb->s_op->write_super(sb);
}
```