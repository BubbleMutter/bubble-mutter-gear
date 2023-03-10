[TOC]
# api
pthread_create pthread_exit pthread_once
pthread_self pthread_equal
pthread_join pthread_detach
pthread_key_create pthread_setspecific pthread_getspecific
pthread_cancel pthread_testcancel
pthread_setcancelstate pthread_setcanceltype
pthread_cleanup_push pthread_cleanup_pop
pthread_kill pthread_sigqueue

pthread_mutex_init pthread_mutex_destroy
thread_mutex_lock pthread_mutex_unlock
phtread_mutex_trylock pthread_mutex_timedlock

pthread_cond_init pthread_cond_destroy
pthread_cond_signal pthread_cond_broadcast
pthread_cond_wait pthread_cond_timedwait

pthread_barrier_init pthread_barrier_destroy pthread_barrier_wait

pthread_rwlock_init phtread_rwlock_destroy
pthread_rwlock_rdlock pthread_rwlock_wrlock pthread_rwlock_unlock
pthread_rwlock_tryrdlock pthread_rwlock_trywrlock
pthread_rwlock_timedrdlock pthread_rwlock_timedwrlock

### 基本控制
+ 子线程 pthread_cancel主线程会导致 seg-fault
+ pthread_t/pthread_equal/pthread_self
+ pthread_create/pthread_cancel/pthread_detach
    + detach的线程 不能被join
    + 可以通过attr使得线程 创建时detach 这是原子性的
    + 常见操作 `pthread_detach(pthread_self());`
+ pthread_exit/pthread_join
    + exit传递数据给join
    + 同一个线程 只能被join一次, 多次join的行为undefined
    + 调用join的thread被异步cancel后 被动join线程回到joinable状态
+ pthread_cleanup_push/pthread_cleanup_pop
    + 必须是exit和cancel后才会执行
### 锁应用
+ barrier (二分算法: merge排序 qsort排序)~

# pthread Catalog
Resources
control: add/del/get (create/terminate/identificate)
mutex, spin, rwlock, cond, barrier
### Resources
+ shared resources in a single process
    + text-section of the executalbe program
    + global and heap mem
    + file descriptors
+ uni-resources
    + a set of regs values
    + a stack
    + a scheduling priority and policy
    + a signal mask
    + an errno variable
    + thread-specific data
### get/add/del
```c
// -----------------
// get:
pthread_t tid;
pthread_equal(pthread_t, pthread_t); // return 0 or 1
pthread_self(void);     // return tid

// -----------------
// add
pthread_create(pthread_t *output_param-tid_pt,
        const pthread_attr_t *input_param-attr,
        void *(* new_thread_start_routine)(void *),
        void *arg);
/* 多次调用 pthread_once传同样的 once_control/init_routine
 * 保证init_routine只会被执行一次
 *
 * 如 线程函数t 和 初始化线程u
 * t会被调用n次 但是不知道哪个线程先调用
 * t必须保证 初始化线程u 跑完一次
 */
 pthread_once_t once_control = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *once_control,
    void (*init_routine)(void));

// -------------------
// del
pthread_exit(void *retval);
pthread_join(pthread_t thread, void **retval);
pthread_cancel(pthread_t tid);

/* only pthread_exit() will invole the pushed cleanup_rontines.
 * we have to match calls to _pop() with the calls to _push()
 * after the pthread_exit() */
pthread_cleanup_push(void (* routine)(void *),
        void *arg);
pthread_cleanup_pop(int execute); 
// cleanup-routines will not be called when threads `return`
// execute != 0, force to exec cleanup-routine at stack-top

/* detach the thread, which will be un-joinable */
pthread_detach(pthread_t tid);
```

## Interface common behavior
+ any trylock() interface will return the errno EBUSY if failed.
+ any timed\*() interface will return the errno ETIMEOUT if failed.

###### mutex dead lock
+ a thread itself tries to lock the same mutex twice.
+ one thread hold a mutex and block while trying to lock a second mutex,
meanwhile another thread holding the second mutex tries to lock the first mutex.

### Spin-Lock
```c
/* like a mutex, except that instead of blocking a process by sleeping,
 * blocking by busy-waiting(spinning).
 * used in situations where locks are held for short periods of times
 * and threads don't want to incur the cost being descheduled.
 * It's the low-level primitives to implement other types of locks.
 * It's useful in a non-preemptive kernel.
 *
 * @pshared is one of PTHREAD_PROCESS_PRIVATE or PTHREAD_PROCESS_SHARED
 */
pthread_spin_init(pthread_spinlock_t *lock,
        int pshared);
pthread_destroy(pthread_spinlock_t *lock);

pthread_spin_lock(pthread_spinlock_t *lock);
pthread_spin_trylock(pthread_spinlock_t *lock);
pthread_spin_unlock(pthread_spinlock_t *lock);
```

### RW-Lock:
```c
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int pthread_rwlock_init(pthread_rwlock_t *output_param-rwlock,
        const pthread_rwlockattr_t *attr);
int phtread_rwlock_destroy(pthread_rwlock_t *input_param-rwlock);
// 有线程在写 其他线程不能读 不能写
// 有线程在读 其他线程能读 但不能写
/*
 * When a rwlock is write-locked with #_wrlock(),
 * all threads attempting to lock it with #_wr/rdlock()
 * will block until it is unlocked.
 * When a rwlock is read-locked with #_rdlock(),
 * all threads attempting to lock it in read mode with #_rdlock()
 * will be given access,
 * but any threads attempting to lock it in write-mode with #_wrlock()
 * will block until all the threads have released thier read-lock.
 */
pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
pthread_rwlock_unlock(phtread_rwlock_t *rwlock);

pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);

pthread_rwlock_timedrdlock(pthread_rwlock_t *rwlock,
        const struct timespec *abstime);
pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock,
        const struct timespec *abstime);
```

### Condition-Lock
```c
/* It's complicated to use, see my_cond_implement.c */
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_init(pthread_cond_t *output_param-cond,
        const pthread_condattr_t *input_param-attr);
phtread_destroy_(pthread cond_t *input_param-cond);

pthread_cond_wait(pthread_cond_t *cond),
    pthread_mutex_t *mutex);
pthread_cond_timedwait(pthread_cond_t *cond,
        pthread_mutex_t *mutex,
        const struct timespec *abstime);

pthread_cond_signal(pthread_cond_t *cond); /* wake up at least one */
pthread_cond_broadcast(pthread_cond_t *cond); /* wake up all */
```

### Barriers
```c
/* allow each thread to wait until all cooperating threads
 * have reached the same point, and the continue to exec,
 * the implement-theory is simple, just a mutex with a @count. */
```