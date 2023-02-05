[TOC]
# shm (posix)
## 接口
```c++
// posix futex 接口
int shm_open(const char *name, int oflag, mode_t mode); // add / get 命名 shm_fd 的方法
int shm_unlink(const char *name); // del 方法; 只del当前进程; 内核中的对象知道没有进程引用后才销毁
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t length);
```
1. mmap 参数
    1. prot: 可以是 PROT_READ PROT_WRITE PROT_EXEC PROT_NONE
    2. flags: MAP_SHARED 表示多进程共享; MAP_PRIVATE 表示其他进程 copy-on-write;

## 实现 (glibc)
1. /dev/shm/ 创建目录; 设置 1777 权限
2. `shm_open` 在该目录创建一个文件, 命名为参数 name
3. `mmap` 映射文件到进程内存空间,
   因为 /dev/shm/ 的特性, 文件本身也存在与内存中
   从而实现共享内存, 共享内存内容修改最终写入到文件中
   拥有权限的用户可以直接编辑文件 `/dev/shm/$name`
4. `shm_unlink` 删除文件

# shm (xsi)
## 接口
```c++
#include <sys/mman.h>
int shmget(key_t key, size_t size, int shmflg);          // 创建匿名shm; 返回 shmid;
int shmctl(int shmid, int cmd, struct shmid_ds *buf);    // 删除匿名shm;
void *shmat(int shmid, const void *shmaddr, int shmflg); // 映射到用户空间
int shmdt(const void *shmaddr); // 反映射

shmget shmctl shmop shmat shmdt
```
1. `shmctl(cmd = IPC_RMID)` 删除shm;
    + 最后的销毁逻辑, 需要等所有 shmat的进程 都执行 shmdt 后才会执行

## 实现 (kernel)
1. xsi 统一的 ipc 对象封装; 其中 shm 实现创建回调
   `int newseg(struct ipc_namespace *ns, struct ipc_params *params)`;
2. `newseg` 的核心逻辑为 `shmem_file_setup`
3. `shmem_file_setup` 创建一个 tmpfs 的文件节点; fops 为 `shmem_file_operations`
    ```c++
    static const struct file_operations shmem_file_operations = {
        .mmap		= shmem_mmap,
        .llseek		= shmem_file_llseek,
        .read_iter	= shmem_file_read_iter,
        .write_iter	= generic_file_write_iter,
        .fsync		= noop_fsync,
        // ...
    };
    ```
    1. 其中, 写方法是内核通用的,
