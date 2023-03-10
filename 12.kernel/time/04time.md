[TOC]
# 1. 原理 以及 变量定义
## 1.1 HZ       每秒产出内核中断的次数 写入到硬件 硬中断进入内核
    + cat /proc/interrupts | grep timer
    + make menuconfig: `Processor type and features ---> Timer frequency (250 HZ)`
## 1.2 jiffies  记录HZ中断后进入内核态的次数, 作为时间轴
```c++
#include<asm/param.h>
# define HZ		CONFIG_HZ   // 写入到硬件, 定时硬中断. 
# define USER_HZ	HZ
# define CLOCKS_PER_SEC	HZ	/* frequency at which times() counts */

#include<linux/jiffies>
extern unsigned long volatile __jiffy_data jiffies;
```
## 1.3 限制
`<linux/timex.h>` 中的实现, 依赖于 HZ 在 (12,1535) 范围, RFC-1589中定义.
所以, 使用 timex.h 的场景中不要把HZ调高于 1535

# 2. 方法
## 2.1 比较函数 time_before / time_after / time_in_range
## 2.2 时间单位转换 （ jiffies / timespec / timeval )
+ `jiffies_to_clock_t()` / `clock_t_to_jiffies()`
+ `jiffies_to_msecs()` / `jiffies_to_usecs()` / `msecs_to_jiffies()` / `usecs_to_jiffies()`
+ `timespec_to_jiffies()` / `jiffies_to_timespec()` / `timeval_to_jiffies()` / `jiffies_to_timeval()`

## 2.3 获取当前时间 
1. `do_gettimeofday()`
2. `current_kernel_time()`

# 2.4 时钟注册方法
`clocksource_register_hz()`

# 3. 延时
```c++
// 阻塞性延时
#include <linux/delay.h>
void ndelay(unsigned long nsecs);
void udelay(unsigned long usecs);
void mdelay(unsigned long msecs);

// 非阻塞性延时; 
void msleep(unsigned int millisecs);
void ssleep(unsigned int seconds);
unsigned long msleep_interruptible(unsigned int millisecs);

// 如果是从用户态临时切换
// 让进程进入可中断的睡眠状态, 
// 返回被提早唤醒时剩余的毫秒数
```

# 4. 经验
1. 不建议在中断、频繁调用的函数中使用 current_kernel_time 方法; 该方法的实时性有偏差
    1. 直接用 jiffies 
2. 实际应用 jiffies 算出来的时间 delta; 跟 实际 rtc 算出来的 delta 是有差异的
   这种差异原因在于 两者 的根源是不同的,
   jiffies 由 cpu 硬件中计数, rtc 时间 由 rtc 芯片计数

# 99 high freq
1. 放大内核CPU软件系数
2. 关闭软狗
3. 放大 jiffies 每次中断后, 增加的数值, 如增加100