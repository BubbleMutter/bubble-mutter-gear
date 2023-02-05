# 原子操作
```c++
atomic_t v = ATOMIC_INIT(0);
void atomic_set(atomic_t *v, int i);

// 原子写
int atomic_read(atomic_t *v);

void atomic_inc(atomic_t *v);
void atomic_dec(atomic_t *v);
void atomic_add(int i, atomic_t *v);
void atomic_sub(int i, atomic_t *v);
void atomic_and(int f, atomic_t *v);
void atomic_xor(int f, atomic_t *v);
void atomic_or(int f, atomic_t *v);

// 原子操作: 写+读, 仅返回 true/false
int atomic_inc_and_test(atomic_t *v);
int atomic_dec_and_test(atomic_t *v);
int atomic_sub_and_test(int i, atomic_t *v); // no atomic_add_and_test

// 原子操作: 写+读, 返回 具体的值
int atomic_add_return(int i, atomic_t *v);
int atomic_sub_return(int i, atomic_t *v);
int atomic_inc_return(atomic_t *v);
int atomic_dec_return(atomic_t *v);

// 原子操作: 写+读, 返回 具体的值的负数
int atomic_add_negative(int i, atomic_t *v);
```
1. Note: atomic_inc + atomic_read 两个操作顺序调用不能保证原子性
   需要调用 atomic_inc_return 一个操作保证原子性

# 原子操作 bit
```c++
set_bit(bit, addr)                 // *addr |= bit
clear_bit(bit, addr)             // *addr &= ~bit
change_bit(bit, addr)             // *addr ^= bit
test_bit(bit, addr)                 // Is bit set in *addr?
test_and_set_bit(bit, addr)         // Set bit and return old value
test_and_clear_bit(bit, addr)     // Clear bit and return old value
test_and_change_bit(bit, addr)     // Change bit and return old value
```
+ 加上前缀 `__` 是非原子化版本

# 使用原子操作bit 替代锁; 以提高性能
```c++
/* try to set lock */
while (test_and_set_bit(nr, addr) != 0)
    wait_for_a_while( );

/* do your work */

/* release lock, and check... */
if (test_and_clear_bit(nr, addr) == 0)
    something_went_wrong( ); /* already released: error */
```
+ 书上这么写; 但实在没找到例子

# 查找 0 或 1 的 bit 的位置
```c++
find_first_zero_bit(addr, nbits) //    Position first zero bit in *addr
find_first_bit(addr, nbits)         // Position first set bit in *addr
find_next_zero_bit(addr, nbits, bit) // Position next zero bit in *addr >= bit
find_next_bit(addr, nbits, bit)     // Position next set bit in *addr >= bit
```
