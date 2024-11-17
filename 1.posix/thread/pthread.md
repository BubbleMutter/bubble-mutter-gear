## pthread_create
```c++
clone(child_stack=0x7fb0966e8fb0, flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID, parent_tidptr=0x7fb0966e99d0, tls=0x7fb0966e9700, child_tidptr=0x7fb0966e99d0);
```
1. pthread_create(); 使用了 clone 调用; 共享了 "VM / FS / FILES / SIGHAND"  等资源
    + 但是每个线程有其独立的 信号队列 以及 多线程共享的信号队列

## pthread_mutex
```c++
int pthread_mutex_lock (pthread_mutex_t *__mutex) {
    atomic_dec(mutex.value);
    if (mutex.value != 0)
        futex(&mutex.value, FUTEX_WAIT, ...)
    else
        return 0;
}
int pthread_mutex_unlock (pthread_mutex_t *__mutex) {
    if (mutex.value != 1)
        futex(&mutex.value, FUTEX_WAKE, ...)
    else
        return 0;
```

# 线程退出
## 1. 方法1: pthread_cancel pthread_testcancel pthread_join
1. pthread_cancel 是异步; 而且要在
   带有"线程取消点"的系统调用中, 才会退出
2. 所以, 为了更好地保证退出
   协线程插装 `pthread_testcancel();`
   然后主线程 `pthread_join()`;
## 2. 方法2: socketpair() 二次挥手
1. 两个线程都使用 epoll 机制, 
   初始化线程时创建 `socketpair(AF_UNIX, SOCK_STREAM, 0, pairfd[2]);`
   pairfd 要设置为 nonblock
   ```c++
   int flags = fcntl(pairfd[0], F_GETFL, 0);
   fcntl(pairfd[0], F_SETFL, flags|O_NONBLOCK);

   int flags = fcntl(pairfd[1], F_GETFL, 0);
   fcntl(pairfd[1], F_SETFL, flags|O_NONBLOCK);
   ```
2. 主线程当需要取消 协线程时, 往 `fd = pairfd[0];` 发消息
   主线程开始等待 `fd = pairfd[0];`消息, 加入 epoll, POLLIN 事件
3. 协线程一直等待 `fd = pairfd[1];`消息, 加入 epoll, POLLOUT, 
   有消息则退出, 协线程销毁资源完成后, 往 `fd = pairfd[1];` 发消息
4. 主线程等待 `fd = pairfd[0];`有消息, 表明协线程已经完成退出了,
   销毁协线程相关的上下文资源
> 实际中: socketpair() 会丢包吗? 
> 参考 af_unix 丢包原因一般是接收缓存满了
> 因此 pairfd[2] 保证只用于退出信息
5. Note: 需要注意的是, 协线程如果使用了 pthread_cleanup_push 注册清理函数
   这种方法如果线程函数直接 return 是不是会调用的
   除非现在线程函数结束使用 pthread_exit.
   或者, return 前手动销毁.
   开发经验开来, 保持线程的一般性, 调一下 pthread_exit 合理一点.
   避免其他不知情的开发者, 使用了 pthread_cleanup_push 导致资源销毁不当.

# 多线程队列问题 (京东2019校招问题)
```c++
#include <stdio.h>
#include <pthread.h>

#define array_count(a) (sizeof(a)/sizeof(*a))
#define TEST_SIZE (3)
pthread_mutex_t mutex_array[TEST_SIZE];
int thread_uuid[TEST_SIZE];

const int times = 10;

void *thr(void *data) {
    int id = *(int *)data;
    int count = times;
    while (count--) {
        pthread_mutex_lock(&mutex_array[id]);
        printf("thr %d\n", id);
        pthread_mutex_unlock(&mutex_array[(id+1) % array_count(mutex_array)]);
    }
    return NULL;
}

int main( int argc, char **argv ) {
    for (int i = 0; i < array_count(mutex_array); ++i) {
        pthread_t tid;
        thread_uuid[i] = i;
        pthread_mutex_init(&mutex_array[i], NULL);
        if (i > 0)
            pthread_mutex_lock(&mutex_array[i]);
        pthread_create(&tid, NULL, thr, &thread_uuid[i]);
    }
    sleep(2);
    return 0;
}
```