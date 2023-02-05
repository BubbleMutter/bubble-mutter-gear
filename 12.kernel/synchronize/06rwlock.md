# rwlock
```c++
#define rwlock_init(lock)

#define read_trylock(lock)
#define write_trylock(lock)
#define write_lock(lock)
#define read_lock(lock)
#define read_unlock(lock)
#define write_unlock(lock)

#define write_trylock_irqsave(lock, flags)
#define read_lock_irqsave(lock, flags)
#define write_lock_irqsave(lock, flags)
#define read_unlock_irqrestore(lock, flags)
#define write_unlock_irqrestore(lock, flags)

#define read_lock_irq(lock)
#define write_lock_irq(lock)
#define read_unlock_irq(lock)
#define write_unlock_irq(lock)

#define read_lock_bh(lock)
#define write_lock_bh(lock)
#define read_unlock_bh(lock)
#define write_unlock_bh(lock)
```
