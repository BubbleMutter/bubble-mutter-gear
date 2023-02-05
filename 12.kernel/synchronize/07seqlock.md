
# seqlock 只用于一个integer (arch相关) (高性能锁) (乐观锁)
1. 使用场景: 写比较罕见但很快; 但是读比较频繁的场景;
2. 使用逻辑: 非阻塞性读; 读完之后处理; 处理后检查是否有写更新;
3. 使用范式
```c++
 // Expected non-blocking reader usage:
do {
    seq = read_seqbegin(&value);
    //    ...
} while (read_seqretry(&value, seq));
```
4. 使用例子 kernel/time/jiffies.c
```c++
u64 get_jiffies_64(void)
{
    unsigned long seq;
    u64 ret;

    do {
        seq = read_seqbegin(&jiffies_lock);
        ret = jiffies_64;
    } while (read_seqretry(&jiffies_lock, seq));
    return ret;
}
```

# 方法 (初始化 / r占用 / r释放 / w占用 / w释放)
```c++
#define seqlock_init(x) // 初始化

// r占用
unsigned read_seqbegin(const seqlock_t *sl);
unsigned int read_seqbegin_irqsave(seqlock_t *lock, unsigned long flags);
// r释放
int read_seqretry_irqrestore(seqlock_t *lock, unsigned int seq, unsigned long flags);
// r占用后检查是否需要重来
unsigned read_seqretry(const seqlock_t *sl, unsigned start);

// w占用
void write_seqlock(seqlock_t *lock);
void write_seqlock_irqsave(seqlock_t *lock, unsigned long flags);
void write_seqlock_irq(seqlock_t *lock);
void write_seqlock_bh(seqlock_t *lock);

// w释放
void write_sequnlock(seqlock_t *lock);
void write_sequnlock_irqrestore(seqlock_t *lock, unsigned long flags);
void write_sequnlock_irq(seqlock_t *lock);
void write_sequnlock_bh(seqlock_t *lock);
```

# 实现
```c++
static inline unsigned raw_read_seqcount_begin(const seqcount_t *s) {
	unsigned ret = __read_seqcount_begin(s);
	smp_rmb();
	return ret;
}
static inline unsigned __read_seqcount_begin(const seqcount_t *s) {
	unsigned ret;
repeat:
	ret = ACCESS_ONCE(s->sequence);
	if (unlikely(ret & 1)) {
		cpu_relax();
		goto repeat;
	}
	return ret;
}
```

```c++
static inline int read_seqcount_retry(const seqcount_t *s, unsigned start)
{
	smp_rmb();
	return __read_seqcount_retry(s, start);
}
static inline int __read_seqcount_retry(const seqcount_t *s, unsigned start)
{
	return unlikely(s->sequence != start);
}
```
```c++
static inline void write_seqlock(seqlock_t *sl) {
	spin_lock(&sl->lock);
	write_seqcount_begin(&sl->seqcount);
}
static inline void raw_write_seqcount_begin(seqcount_t *s) {
	s->sequence++;
	smp_wmb();
}
```
```c++
static inline void write_sequnlock(seqlock_t *sl) {
	write_seqcount_end(&sl->seqcount);
	spin_unlock(&sl->lock);
}
static inline void raw_write_seqcount_end(seqcount_t *s) {
	smp_wmb();
	s->sequence++;
}
```
1. `read_seqbegin` 的逻辑是取得 `seqcount_t` 的 `sequence` 成员变量
2. `read_seqretry` 的逻辑是比较 最新的 `sequence` 与之前取得的是否相等
3. `write_lock` 的逻辑是加锁自选锁, 然后 `sequence++`
4. `write_unlock` 的逻辑是 `sequence++`, 然后解锁自选锁
5. seqlock 自身支持多写并发, 无需额外加锁, 这个特性与 rcu 不相同
