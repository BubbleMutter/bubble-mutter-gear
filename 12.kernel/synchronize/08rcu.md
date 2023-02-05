[TOC]
# RCU (Read-Copy-Update)
http://www2.rdrop.com/users/paulmck/rclock/
http://www2.rdrop.com/users/paulmck/RCU/RCU.2018.11.21c.PSU-full.pdf

(Create)、检索(Retrieve) 更新(Update) 和 删除(Delete)

# 1. 特性
1. 适用场景: 读多, 写非常非常少的场景
2. 读写锁
   1. 当存在读线程时, 写线程会阻塞
   2. 当存在写线程时, 读线程会阻塞. (已经优于 互斥锁)
3. RCU
   1. 存在写线程时, 读线程不阻塞.
   2. 存在读线程时, 写线程可能阻塞.
4. 所以由于多写并发 rcu 写线程需要加锁 </br>
   在频繁写and低频读场景中 RCU 则不适用, 建议使用原始 rwlock

# 2. 使用

## Get
```c++
void get() {
    rcu_read_lock();
    list_for_each_entry_rcu() {
        if (found) _entry = entry;
    }
    rcu_read_unlock();
    return _entry;
}
```

## Del
```c++
struct entry {
    int id;
    struct list_head list;
    struct rcu_head rcu;
};
void __del(struct rcu_head *head) {
    struct entry *e = container_of(head, struct entry, rcu);
    kfree(e);
}

void del() {
spin_lock(); // 多写并发才需要加锁; 单写不加锁
list_for_each_entry() {
    // NOTE: 多写并发的读操作必须是互斥的, 避免两个写线程取得相同节点
    if (found) {
        _entry = entry;
    }
}
list_del_rcu();
spin_unlock(); // 多写并发才需要加锁; 单写不加锁

/* NOTE: synchronize_rcu()/call_rcu() 是阻塞等待; 避免再加锁块中 */
#if 0 // 旧接口
synchronize_rcu();
free(_entry)
#else // 使用新接口,
call_rcu(_entry->rcu, __del);
#endif
}
```

# 3. 原理 (示例为单写多读; 不考虑多写并发)
## Get
1. 在读操作前后 `rcu_read_lock` / `rcu_read_unlock` 占用和释放 锁
2. 占用和释放仅原子操作全局变量, 不会阻塞

## Add
```c++
static inline void __list_add_rcu(struct list_head *new,
		struct list_head *prev, struct list_head *next) {
	new->next = next;
	new->prev = prev;
	rcu_assign_pointer(list_next_rcu(prev), new);
	next->prev = new;
}

#define rcu_assign_pointer(p, v) smp_store_release(&p, RCU_INITIALIZER(v))
#define smp_store_release(p, v)						\
do {									\
	compiletime_assert_atomic_type(*p);				\
	smp_mb();							\
	ACCESS_ONCE(*p) = (v);						\
} while (0)
```
1. 分析源码, 插入操作分为3个步骤
   1. 新节点的前置指针 和 后置指针分别指向 前置节点和后置节点
   2. **内存屏障**
   3. 前置节点连接新节点 和 后置节点连接新节点
2. 必须通过**内存屏障**保证新节点初始化后再连接 </br>
   否则, 当其他线程通过 前置节点.next 访问新节点 </br>
   并尝试访问 新节点.next 时, 新节点.next 指针未初始化导致非法访问 </br>
   同理, 通过 后置节点.prev 访问新节点, 也会有相同问题 </br>


## Del
```c++
// 步骤1
static inline void list_del_rcu(struct list_head *entry)
{
	__list_del_entry(entry);
	entry->prev = LIST_POISON2;
}
static inline void __list_del_entry(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}
// 步骤2
static inline void synchronize_rcu(void)
{
	synchronize_sched();
}
void synchronize_sched(void)
{
	rcu_lockdep_assert(!lock_is_held(&rcu_bh_lock_map) &&
			   !lock_is_held(&rcu_lock_map) &&
			   !lock_is_held(&rcu_sched_lock_map),
			   "Illegal synchronize_sched() in RCU-sched read-side critical section");
	if (rcu_blocking_is_gp())
		return;
	if (rcu_expedited)
		synchronize_sched_expedited();
	else
		wait_rcu_gp(call_rcu_sched);
}
// 步骤3
kfree(entry);
```
1. 分析源码, 删除分3个步骤
   1. 剪断 **待删除节点** </br>
      其他 cpu 不再通过前后置节点访问 **待删除节点** </br>
      但是可能存在 cpu 正在访问 **待删除节点**
   2. 阻塞等待 synchronize_rcu 实际上是等待其他所有 cpu rcu_read_unlock
      从而保证没有 cpu 占用全局 rcu 锁 (锁是全局的, 与具体节点无关)
      进一步保证没有使用**待删除节点**的内存
   3. 销毁 **待删除节点**

## Set
```c++
// 步骤1
struct object * oldptr = search();
struct object * newptr = deep_copy(old);
update(newptr);

// 步骤2
static inline void list_replace_rcu(struct list_head *old,
				struct list_head *new)
{
	new->next = old->next;
	new->prev = old->prev;
	rcu_assign_pointer(list_next_rcu(new->prev), new);
	new->next->prev = new;
	old->prev = LIST_POISON2;
}

// 步骤3
synchronize_rcu();
kfree(oldptr);
```
1. 分析源码, 更新分3个步骤
    1. Read-Update-Copy, 即 查找-复制-更新 (而不是直接更新)
    2. 形同新增操作的3个步骤: 初始化新节点-内存屏障-连接新节点
    3. 形同删除操作的3个步骤: 剪断**待删除节点**-阻塞等待-销毁**待删除节点**


# 8. Question
## Q1: seqlock 也能做到单读单写并发; seqlock 和rcu有何区别? RCU 优势在何处?
1. seqlock 在读的过程中如果发生写并发; 则需要调用 `read_seqretry()`
2. `read_seqretry()` 返回 true 后读操作需要重新执行
3. 而 rcu  在读的过程中如果发生写并发; 则只需要继续执行读操作, 无感知

## Q2: What prevents the list_for_each_entry_rcu from getting a segfault if it happens to execute at exactly the same time as the list_add_rcu()?
1. __list_add_rcu 的 rcu_assign_pointer 中的 内存屏障
2. 先保证新增节点已经连接 前后置节点; 通过内存屏障; 再把 前后置节点 连接 新增节点

## Q3: How would you modify the deletion example to permit more than two versions of the list to be active?
1. 多写并发需要加锁; 锁需要在写操作 获取 待更新节点前; 避免更新相同节点

## Q4: How many RCU versions of a given list can be active at any given time?
1. That depends on the synchronization design.
2. If a semaphore protecting the update is held across the grace period, then there can be at most two versions, the old and the new.
3. However, if only the search, the update, and the list_replace_rcu() were protected by a lock, then there could be an arbitrary number of versions active, limited only by memory and by how many updates could be completed within a grace period.
4. But please note that data structures that are updated so frequently probably are not good candidates for RCU. That said, RCU can handle high update rates when necessary.

## Q5: How can RCU updaters possibly delay RCU readers, given that the rcu_read_lock() and rcu_read_unlock() primitives neither spin nor block?
1. 问题: 为什么 rcu_read_lock() 可以实现 spin_lock 的逻辑, 延迟读并发, 从而实现数据同步?
    + 这个问题实际上是讨论 RCU 的硬件实现本质
2. 答案: RCU 发布后, 其CPU 把 cache-lines 的数据 "无效化", 迫使其他CPU 遭遇"昂贵"的 cache-misses
3. 原文作者的额外疑问: Can you design an algorithm that changes a data structure without inflicting expensive cache misses on concurrent readers? On subsequent readers?

## Q7: diff between rcu_dereference and rcu_dereference_protected
1. rcu_dereference() should be used at read-side, protected by rcu_read_lock() or similar.
2. rcu_dereference_protected() should be used at write-side (update-side) by the single writer,
   or protected by the lock which prevents several writers from concurrent modification
   of the dereferenced pointer.
   In such cases pointer cannot be modified outside of the current thread,
   so neither compiler- nor cpu-barriers are needed.

# 9. 经验
## synchronize_rcu_bh 错误示范
1. 这个带 bh 后缀的rcu同步, 实际上是不能完全用在 中断下半部的,
2. 因为这个函数的实现, 某个版本开始, 是可调度的,
3. 只能用于 workqueue, 系统调用
4. 不能用在 软中断, tasklet 中
5. 2021/04/08 没看实现, 踩坑了

## rcu_dereference / rcu_dereference_protected
1. rcu_dereference
    1. 用于读操作
2. rcu_dereference_protected
    1. 额外传入写锁, 避免多写并发
    2. 不用于读操作, 只用于写操作
    3. 实践中, 一般用于写操作前, 取得旧变量

## rcu_read_lock_bh / rcu_read_unlock_bh
1. 在 rcu_read_lock 基础上, 关掉本cpu的软中断
   避免中断切走后执行太多的软中断, 从而提高 section 中代码的实时性
2. 用途1: 期望 crtical-section (1) 足够复杂的逻辑, (2) 尽可能地执行完
    1. 如: 协议栈的全局表动态更新, 需要更新得足够快.

## rcu_read_lock / rcu_read_unlock
1. 已经处于 关掉中断 的上下文中 (local_irq_save / local_irq_restore)
2. 已经处于 关掉下半部的上下文中 (local_bh_disable / local_bh_enable)

## rcu_replace_pointer
+ rcu_replace_pointer() = rcu_dereference_protected() + rcu_assign_pointer()

## preempt issue
1. no preempt rcu = `rcu_read_lock()` call `preempt_disable()`
    1. no preempt kernel = `preempt_disable()` is redundancy
    2. do preempt kernel = `rcu_read_lock()` is to disable preempt
2. do preempt rcu = `rcu_read_lock()` incre the counter
    1. no preempt kernel = read-side critical sections will never block
    2. do preempt kernel = read-side critical sections will block depending on scheduler
