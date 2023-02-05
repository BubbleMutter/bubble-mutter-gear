[TOC]
# 1. 基本概念
1. 中断分层, 处理硬件中断分两个部分处理, 上部分 和 下部分.
    1. 目的 = 处理中断处理程序 运行效率快 和 承载业务多 的矛盾
2. 上半部: top half
3. 下半部: bottom half

# 2. 必须在上半部处理的场景 (开发经验)
1. 如果一个任务对时间非常敏感，感觉告诉我还是将其放在中断处理程序中执行是个好的选择。
2. 如果一个任务和硬件相关，还是将其放在中断处理程序中执行吧。
3. 如果一个任务要保证不被其他中断(特别是相同的中断)打断，那就将其放在中断处理程序中吧。
4. 其他所有任务，除非你有更好的理由，否则全部丢到下半部执行。

# 3. 共享中断
1. request_irq 的参数 flags 带上 IRQF_SHARED 标志
2. request_irq 的 dev_id 在相同中断ID中, 必须是唯一且非空
3. 共享中断的中断回调 需要有 检查对应中断是否触发的前置逻辑

# 4. 调试信息
1. `cat /proc/interrupts`
    + 中断ID, 每个CPU中断次数, ?中断信息? 中断名称(注册方法::传入的name参数)
    + 实现函数 `show_interrupts()`
2. `cat /proc/stat | grep intr`
    + 第一列数字表示总的 中断次数;
    + 第二列数字表示中断ID为 0 的中断次数
    + 第N 列数字表示中断ID为 N - 2 的中断次数 周

# 5. 接口
```c++
// kernel/irq/manage.c
/**
 * @brief
 */
typedef irqreturn_t (*irq_handler_t)(int, void *);

// 注册/销毁
/**
 * @param irq    中断号; CPU硬件相关
 * @param name   对应 /proc/interrupts
 * @param dev_id 传递给中断回调的上下文
 *               非 IRQF_SHARED 可以为NULL; 一般为设备对象
 *               当 IRQF_SHARED 时基本上需要指定, 以区分不同设备对象
 * @note request_irq 是会导致休眠的
 *       request_irq > kzalloc(GFP_KERNEL);
 */
int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
	    const char *name, void *dev_id);

/**
 * @param dev_id 共享中断线 IRQF_SHARED 时提供
 *               当 IRQF_SHARED 的中断ID 上所有 dev 被释放; irq 才被关闭
 *               非 IRQF_SHARED 的中断ID 被释放后; irq 将随之关闭
 */
void free_irq(unsigned int irq, void *dev_id);

// 注册/销毁 每个CPU
int request_percpu_irq(unsigned int irq, irq_handler_t handler,
		   const char *devname, void __percpu *percpu_dev_id);
void free_percpu_irq(unsigned int irq, void __percpu *dev_id);

// 使能整个中断线
void disable_irq_nosync(int irq);
void disable_irq(unsigned int irq);
void disable_percpu_irq(unsigned int irq);
void enable_irq(unsigned int irq);
void enable_percpu_irq(unsigned int irq, unsigned int type);
void synchronize_irq(unsigned int irq);

// 使能 当前CPU的中断
void local_irq_disable(void);  // 关闭当前CPU中断
/* 这之间进行 原子操作 */
void local_irq_enable(void);   // 开启当前CPU中断

void local_irq_save(unsigned long flags);    // 关闭当前CPU中断, 保存之前的中断状态
void local_irq_restore(unsigned long flags); // 关闭当前CPU中断, 还原之前的中断状态

int irqs_disabled();  // 如果本地中断传递被禁止，则返回非0；否则返回0
int in_interrupt();   // 如果在中断上下文中，则返回非0；如果在进程上下文中，则返回0
int in_irq();         // 如果当前正在执行中断处理程序，则返回非0；否则返回0
```
## 5.1 enable/disable vs save/restore
1. `local_irq_enable`/`local_irq_disable` 是强制的, 不会保存之前中断状态 </br>
   需要调用者确保 该中断上下文中没有其他中断; 可能会导致开始是关闭的中断被打开   </br>
   安全性考虑, 建议使用 `local_irq_save`/`local_irq_restore`
2. 使用 save/restore 的场景:   do_softirq, raise_softirq, tasklet_schedule, tasklet_hi_schedule
3. 使用 enable/disable 的场景: tasklet_action, tasklet_hi_action
4. 实际上必须先使用 save/restore, 而在 {save,restore} 之间使用 enable/disable

## 5.2 enable/disable issue
1. `enable` is recommended to be called as many as `disable` has been called
    1. 对于同一个中断ID, `enable` 和 `disable` 必须成对出现
2. 共享中断不允许使用 `enable` / `disable`; 否则将屏蔽其他设备的中断

# 回调 flags
1. IRQF_SHARED 共享中断 (如: pci bus, universal-serial-bus)
  1. 注册时 flag 带上 IRQF_SHARED, 使得同一个 中断ID 被多个硬件 以及 多个中断回调 共享
  2. 多个共享中断的中断回调; 以单向链表的方式组织起来;
  3. 共享中断的中断回调 需要有 检查对应中断是否触发的前置逻辑
2. IRQF_PROBE_SHARED
3. IRQF_TIMER          (__IRQF_TIMER | IRQF_NO_SUSPEND | IRQF_NO_THREAD)
4. IRQF_PERCPU
5. IRQF_NOBALANCING
6. IRQF_IRQPOLL        只能用于shared中断; 以提高性能
7. IRQF_ONESHOT        没触发一次中断后; 销毁中断回调
8. IRQF_NO_SUSPEND     suspend(睡眠)中; 中断不停止; 即允许唤醒
9. IRQF_FORCE_RESUME   睡眠后启动时; 强制开启中断
10. IRQF_NO_THREAD      该中断无法 `线程化`?
11. IRQF_EARLY_RESUME   睡眠后启动时; 在syscore流程中开启中断; 而不是硬件恢复阶段开启


# 中断掩码检测
+ 检测机制: 关闭全局中断; 触发单个设备中断; 从而获取到实际的中断bit
## 内核检测方法 probe_irq_on + probe_irq_off
```c++
unsigned long probe_irq_on(void); // 开启 irq 检测
int probe_irq_off(unsigned long); // 停止 irq 检测

void usage() {
    // 1. clear device state
    int mask = probe_irq_on();
    // 2. enable and trigger the device's interrupt once
    // 3. delay  some time for detection
    int irq = probe_irq_off(mask);
    int ret = request_irq(irq, handler, irq_flags, devname, data);
}
```
