[TOC]
# 1. 内存屏障
## 1.1 barrier() （compiled barrier)
```c++
# define barrier() __memory_barrier()

```
内存屏障: 编译器层的控制
  Compiled code stores to memory all values that are currentlymodified and resident in CPU registers,
  and rereads them later when they areneeded.
  A call tobarrierprevents compiler optimizations across the barrier butleaves the hardware free to do its own reordering.
+ 与硬件完全无关
+ 内存屏障的本质之一 = 停止 编译优化/CPU流水线; 避免 优化导致硬件读写的时序问题

### 1.2 mb() / rmb() / wmb() / read_barrier_depends()（hardware barrier)
```c++
// friendly macro
#define set_mb(var, value)  do { (var) = (value); mb(); } while (0)
```
1. 代码中包含 barrier(); 且通过调用arch指令; 实现硬件级的 内存屏障
2. rmb()  读屏障
3. wmb()  写屏障
4. mb() 读写屏障

### 1.3 smp() (smp hardware barrier)
```c++
void smp_rmb(void);
void smp_read_barrier_depends(void);
void smp_wmb(void);
void smp_mb(void);
```