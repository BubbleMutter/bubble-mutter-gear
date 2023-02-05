[TOC]
# initcall
1. 只在内核初始化时执行
2. 一次执行后续不再执行

# do_initcalls
```c++
extern initcall_entry_t __initcall_start[];
extern initcall_entry_t __initcall0_start[];
extern initcall_entry_t __initcall1_start[];
extern initcall_entry_t __initcall2_start[];
extern initcall_entry_t __initcall3_start[];
extern initcall_entry_t __initcall4_start[];
extern initcall_entry_t __initcall5_start[];
extern initcall_entry_t __initcall6_start[];
extern initcall_entry_t __initcall7_start[];
extern initcall_entry_t __initcall_end[];

static initcall_entry_t *initcall_levels[] __initdata = {
	__initcall0_start,
	__initcall1_start,
	__initcall2_start,
	__initcall3_start,
	__initcall4_start,
	__initcall5_start,
	__initcall6_start,
	__initcall7_start,
	__initcall_end,
};


static void __init do_initcall_level(int level) {
    // ...
	initcall_entry_t *fn;
    // ...
	for (fn = initcall_levels[level]; fn < initcall_levels[level+1]; fn++)
		do_one_initcall(initcall_from_entry(fn));
}

static void __init do_initcalls(void) {
	int level;

	for (level = 0; level < ARRAY_SIZE(initcall_levels) - 1; level++)
		do_initcall_level(level);
}
```
1. 遍历 `initcall_levels[]` 数组; 执行初始化函数

