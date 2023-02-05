[TOC]
# struct dentry
```c++
struct dentry {
    struct hlist_bl_node d_hash; /* lookup hash list; inode_operations->lookup 方法 */
    struct dentry *d_parent;     /* parent directory */
    struct inode *d_inode;       /* Where the name belongs to */
    unsigned char d_iname[DNAME_INLINE_LEN]; /* small names */

    struct lockref d_lockref;    /* per-dentry lock and refcount */
    const struct dentry_operations *d_op;
    struct super_block *d_sb;    /* The root of the dentry tree */

    struct list_head d_lru;        /* LRU list */
    struct list_head d_child;    /* child of parent list */
    struct list_head d_subdirs;    /* our children */
};
```

## 目录项状态 (3种)2. "被使用的" dentry 列表:
1. 被使用: d_inode 指向有效对象; d_lockref 引用计数为正
2. 未使用: d_inode 指向有效对象; d_lockref 引用计数为零
   1. 如果需要使用该 dentry 时, 可以顺序查到 d_inode
   2. 如果需要回收内存, 未使用状态的 dentry 可以被回收
3. 负状态: d_inode 指向 NULL, d_inode 已删除
   1. 用于进程 open 该路径时, 马上返回 ENOENT, 而无须重新查找
   2. 实际上用得比较少

# 目录缓存
## 背景
1. dentry 的初始化相当耗时; 需要逐个解析绝对路径的每个元素
2. 如 /usr/bin/gcc 需要解析 `/`, `usr`, `bin`, `gcc` 4个元素
3. 所以把解析出来的dentry 缓存起来
## 组成部分
```c++

// 全局 hash 表
static struct hlist_bl_head *dentry_hashtable __read_mostly;
// 计算一个 dentry 的 hash 值
static inline struct hlist_bl_head *d_hash(const struct dentry *parent, unsigned int hash);
// 查找 hash 表中的 dentry; 如果查找不到, 则返回 NULL
struct dentry *d_lookup(const struct dentry *parent, const struct qstr *name);
```
1. hash 表 `dentry_hashtable`, 通过 `dentry->d_hash` 连接


# struct dentry_operations 部分成员解析
```c++
struct dentry_operations {
    // 判断 dentry, vfs 从 dcache 中使用 dentry 前, 调用该函数检查
    int (*d_revalidate)(struct dentry *, unsigned int);
    // 计算 hash 值, 输出到 qstr->hash 中
    int (*d_hash)(const struct dentry *, struct qstr *);
    int (*d_compare)(const struct dentry *, const struct dentry *,
            unsigned int, const char *, const struct qstr *);
    // 当 d_lockref 引用计数为0, vfs 调用该函数, 调用前加锁 ->d_lock
    int (*d_delete)(const struct dentry *);
    // 当 dentry 需要被销毁时, vfs 调用该函数
    void (*d_release)(struct dentry *);
    // 当 dentry 需要 drop 其 inode, vfs 调用该函数qwerzxcv
    void (*d_iput)(struct dentry *, struct inode *);
    char *(*d_dname)(struct dentry *, char *, int);
};
```
1. 实际接口由 fs 驱动实现
2. d_revalidate 大部分 fs 驱动置空, 认为 dcache 目录项中对象总是有效