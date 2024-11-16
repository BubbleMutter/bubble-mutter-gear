# kernel
https://0xax.gitbooks.io/linux-insides/content/Concepts/linux-cpu-1.html 另一个内核学习网站 (目录结构不错)
https://github.com/0xAX/linux-insides/blob/master/SUMMARY.md 0xAX 配套markdown
https://lwn.net/Articles/262464/ RCU introduction part 1
+ Linux Kernel Development (E3)

# topic
+ 中断 = 中断的实现
+ 定时器的实现
+ 进程调度
+ 内存 = 内存屏障/原子变量/RCU/自旋锁的实现

# memory:
slabinfo: trace the slabinfo entry in kernel, create by proc_create()
kmemleak: have a look on kmemleak dump file
https://linux-kernel-labs.github.io/refs/heads/master memmory section

# lkd3 plan
next plan:
chapter 11 time
chapter 12 memory

优先级低:
chapter 13: vfs
chapter 14: block i/o
chapter 16: page cache
chapter 17: device and module

不看:
chapter 18 ~ chapter 20: bullish

# Development Tips
## source directory
| directory | description |
| --: | :-- |
| arch/ | 指架构的源码 |
| block/| 块设备 I/O |
| crypto/ | 加密 api |
| drivers/ | 设备驱动 |
| firmware/ | 某些设备驱动运行前需要下发的固件描述文件 |
| fs/ | vfs 和 各种文件系统的实现 |
| include/ | 内核头文件 |
| init/ | 内核引导和初始化
| ipc/ | 进程间通讯 的 实现 |
| kernel/ | 核心子系统; 如 调度器 |
| lib/ | 通用内核函数 |
| mm/ |内存管理子系统 和 核心子系统 |
| net/ | 网络子系统 |
| scripts/ | 编译过程中使用的脚本 |
| security/ | 内核安全模块 |
| sound/ | 语音子系统 |
| usr/ | 内核初始化前期的用户态代码 (initramfs) |
| tools/ | 在 linux 开发中用到的工具 |
| virt/ | 虚拟化基础结构 |

## beginner pitfalls
1. 没有 glibc, 部分库代码在 lib/ 实现
2. gnu c 标准, 支持内联汇编 asm 编译器指令 (形同函数)
3. 没有内存保护; 非法访问会出现 oops
4. 不能使用 浮点
5. 内核程序的栈空间 很小而且固定尺度
6. 同步和并发:
    1. 抢占式(可关掉) 多任务系统; 需要处理对进程切换时的同步问题
    2. 支持多对称处理器架构(SMP): 资源访问需要同步