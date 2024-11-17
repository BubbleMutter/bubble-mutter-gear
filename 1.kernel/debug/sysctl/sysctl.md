[TOC]
```log
_> ls -1 /proc/sys/
abi
crypto
debug
dev
fs
kernel
net
user
vm
```

# sysctl
```c++
/**
 * @procname: 字符串ID
 * @data: 全局数据
 * @proc_handler: 数据类型格式化回调
 *                proc_dostring / proc_dointvec / proc_dointvec_minmax / proc_dointvec_jiffies
 *                proc_dointvec_ms_jiffies / proc_doulongvec_minmax / proc_doulongvec_ms_jiffies_minmax
 */
struct ctl_table
{
	const char *procname;		/* Text ID for /proc/sys, or zero */
	void *data;
	int maxlen;
	umode_t mode;
	struct ctl_table *child;	/* Deprecated */
	proc_handler *proc_handler;	/* Callback for text formatting */
	struct ctl_table_poll *poll;
	void *extra1;
	void *extra2;
} __randomize_layout;

/**
 * @note 单一接口
 *       注册 /proc/sys/ 第一层目录
 *       例如 register_sysctl("abi", ...) register_sysctl("net", ...)
 */
struct ctl_table_header *register_sysctl(const char *path, struct ctl_table *table);

/**
 * @note 批量接口
 *       注册 /proc/sys/ 第一层目录; 包括 `kernel` `vm` `debug` `fs` `dev`
 *       其中 struct ctl_table sysctl_base_table[] 通过 path 指示路径
 */
struct ctl_table_header *register_sysctl_table(struct ctl_table *table);


/**
 * @set: 主节点
 * @path: 新注册节点路径
 * @table: 新注册节点列表
 * @note 底层接口
 */
struct ctl_table_header *__register_sysctl_table(struct ctl_table_set *set, const char *path, struct ctl_table *table);
```
1. 对象 `struct ctl_table` 指示一个变量的名称/权限/变量指针
2. 方法 `register_sysctl` / `register_sysctl_table` / `__register_sysctl_table` 注册到 `proc/sys` 中

