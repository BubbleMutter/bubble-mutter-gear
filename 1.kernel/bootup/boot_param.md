# boot param
```c++
// fn 类型
int (*setup_func)(char *);

// 参数回调
struct obs_kernel_param {
	const char *str;
	int (*setup_func)(char *);
	int early;
};

#define __setup(str, fn)
#define early_param(str, fn)

// usage
int __init netdev_boot_setup(char *str);
__setup("netdev=", netdev_boot_setup);
__setup("ether=", netdev_boot_setup);
```
1. `struct obs_kernel_param` 对象记录每个参数的回调
    1. `__setup()`     设置 `early = 0`
    2. `early_param()` 设置 `early = 1`
2. 实现原理:
    1. 源码中编译标记 `struct obs_kernel_param`
    2. 编译器解析标记 `struct obs_kernel_param` 写入 elf section `.init.setup`
    3. 运行阶段反序列化 `struct obs_kernel_param`
2. Two-Pass Parsing; parse_args 函数将会执行两次
    1. 第一次解析 `early_param()` 即 `early = 1` 的 `struct obs_kernel_param`
    2. 第二次解析 `__setup()`     即 `early = 0` 的 `struct obs_kernel_param`
3. 传递到 init 进程
    1. 当 Two-Pass Parsing 都无法解析的 kv-pair 将传递到 init 进程
    2. 传递方式可能是 环境变量`env[]` 或者 命令行参数`args`
    3. 该逻辑通过 `run_init_process()` 实现

# parse_args