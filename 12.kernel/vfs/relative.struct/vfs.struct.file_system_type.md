[TOC]
# 描述文件系统类型
```c++
struct file_system_type {
	const char *name;
	int fs_flags;
    // 挂载文件系统
	struct dentry *(*mount) (struct file_system_type *, int,
		       const char *, void *);
    // 终止访问 super_block
	void (*kill_sb) (struct super_block *);
	struct module *owner;

	struct file_system_type * next;
	struct hlist_head fs_supers;

	struct lock_class_key s_lock_key;
	struct lock_class_key s_umount_key;
	struct lock_class_key s_vfs_rename_key;
	struct lock_class_key s_writers_key[SB_FREEZE_LEVELS];

	struct lock_class_key i_lock_key;
	struct lock_class_key i_mutex_key;
	struct lock_class_key i_mutex_dir_key;
};
```
1. 每一个文件系统的实例)(对应分区), 都有一个 super_block
2. 但是同种类型的文件系统, 只有一个 file_system_type 对象
