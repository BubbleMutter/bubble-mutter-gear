[TOC]
glibc malloc 研究笔记
glibc malloc 历史上主要由由两个部分组成
    (Doug Lea 实现的 ptmalloc2)
    (Wolfram Gloger 适配的 多 threads/arenas 版本)

http://gee.cs.oswego.edu/dl/html/malloc.html 上古时期的讨论
https://sourceware.org/glibc/wiki/MallocInternals 概念介绍

内存管理的本质问题: 在 分配销毁效率 与 额外空间损耗 两者间平衡
假设极端场景a) 每次分配内存都从最后放新申请, 销毁后不能再复用,
              那么, 分配销毁效率 贼快, 但是额外空间消耗超大

# 1. heap:  一大块连续的内存
1. 头部必有一个 heap_info, 包含 arena 指针
2. 紧接着 heap_info 可能有一个 arena
    否则 heap_info 指向另一个 heap 的 arena
    因此 heap 只会从属于一个 arena
3. 最后 heap 后续剩余部分划分为一块一块的 `chunk`
4. 进程最初的 heap (即内核分配的 heap 空间) 为从属 `main_arena`
    + 通过 `brk/sbrk` 扩展
5. 后续通过 `mmap` 创建新的 heap 以及 arena
    + arena 的最大个数是 `nr_cpu * 8`; 一个 arena 有多个 heap

# 2. heap_info:
1. 包含指向 arena 的指针
2. 包含双向链表连接不同的 heap
3. 首地址必然是 2的指数次方, 通过 `mmap/brk/sbrk` 传参控制
4. 所以 `heap_info *heap_info = mchunkptr & HEAP_MAX_SIZE` 求得 chunk 的所在 heap
## 2.1 定义
```c++
typedef struct _heap_info {
  mstate ar_ptr; /* Arena for this heap. */
  struct _heap_info *prev; /* Previous heap. */
  size_t size;   /* Current size in bytes. */
  size_t mprotect_size;	/* Size in bytes that has been mprotected
			   PROT_READ|PROT_WRITE.  */
  /* Make sure the following data is properly aligned, particularly
     that sizeof (heap_info) + 2 * SIZE_SZ is a multiple of
     MALLOC_ALIGNMENT. */
  char pad[-6 * SIZE_SZ & MALLOC_ALIGN_MASK];
} heap_info;
```

# 3. arena: `struct malloc_state`
1. 包含 mutex, 由一个 arena 由多个 or 单个线程共享
2. 包含单个 or 多个 heap 引用
3. 链表指针连接单个 or 多个heap 中 free 的 `chunk`
4. 一个指向 top_chunk 的指针

# 4. chunk
1. 可以用来 alloc / free 以及 合并为更大的 chunk
2. 存在与 heap 中, heap_info 或 arena 后面
3. 实际上是 heap 中的一块一块的内存
4. chunk 有不同的大小, 是 malloc 内存管理的最小单元
5. chunk 尺度存储变量是 `size_t`, 最小尺度是 8 bytes, 低 3 bits 用作 flags (AMP)
## 4.1 定义
```c++
struct malloc_chunk {
  INTERNAL_SIZE_T      prev_size;  /* Size of previous chunk (if free).  */
  INTERNAL_SIZE_T      size;       /* Size in bytes, including overhead. */
  struct malloc_chunk* fd;         /* double links -- used only if free. */
  struct malloc_chunk* bk;
  /* Only used for large blocks: pointer to next larger size.  */
  struct malloc_chunk* fd_nextsize; /* double links -- used only if free. */
  struct malloc_chunk* bk_nextsize;
};
```
## 4.2 chunk_flag (AMP): (chunk 的size低3)
1. flag@A(0x4): 0表示 存在于进程`heap 空间` 的 `main_arena`
                1表示 存在于进程`mmap 空间` 的 `mmap arena`
2. flag@M(0x2): 1表示 `mmap()` 得到的内存
3. flag@P(0x1): 1表示 上一个(地址更低的) chunk 已经分配, 已分配的 chunk 无 prev_size
## 4.3 top_chunk: 某些 heap 的结尾
1. 一个 arena 连接着多个 heap
2. 一个 arena 连接的最后一个 heap 的结尾有一块内存 top_chunk
3. 用以指示 arena 的结尾

# 5. bins[] 回收站
1. arena 中有两个定长指针数组 `fastbins[]` 和 `bins[]` 连接 `chunk` 链表
2. 其中 bins[] 一部分链表是 `large bin`; 另一部分链表是 `small bin`
## 5.1 fast
1. chunk 的大小是固定的; 不会分割和合并
2. 由于不会分割和合并; 所以只需要单向链表即可
3. 单向链表 `struct malloc_chunk* bk;`不维护
4. 运行中, 可能会分配到其他 `bins[]` 进行分割/合并
## 5.2 unsorted
1. 刚刚 free 的 `chunk`; 还没排序到合适的链表
   首次进入一个空闲 bins[] 链表时 (即只有一个 `chunk` 的 bin)
## 5.3 small
1. `chunk` 的 size 固定, 能直接使用, 不需要分割
2. 加入到该 bin 中的 `chunk` 首先会与邻接的 `chunk` (地址连续的) 进行合并
3. 无法合并: 即大小固定, 那么视为 small bin
4. 可以合并: 即合并称为 `large chunk`
5. 由于 `small chunk` 特性, malloc / free 返回后;
   不会存在两个连续的 `small chunk`
## 5.4 large
1. `large chunk` 的 size 大于 `small chunk`, 大小不一
2. `large chunk` 不能直接使用, 可以需要分割为合适的大小
   分割后一部分返回给用户, 另一部分加入到合适 `bins[]` 中
3. 由于大小不一, 为了快速查找
    1. 一个 `bin[]` 中 `large chunk` 用降序排列
    2. 不同 `bin[]` 中 通过首个 `large chunk` 大小升序排列
    3. 所有 `large chunk` 通过 `->fd `和 `->bk` 连接
    4. 不同大小的首个 `large chunk` 通过 `fd_nextsize` 和 `bk_nextsize` 连接 (见图)
4. 首先查找 `bins[i]` 首个 `large chunk` 是否满足大小
   不满足直接寻找一个 `bin[i+1]`
5. 当找到合适大小 `large chunk` 返回用户; 如果有多后续多个相同 size 的;
   优先返回后面的 (`->fd` 指向); 从而减少 `fd_nextsize` 和 `bk_nextsize` 修改 (见图)
6. 当找到相同大小 `large chunk` 插入链表; 则优先通过 `->fd` 插入到后面
   减少 `fd_nextsize` 和 `bk_nextsize` 的修改 (见图)

# 6. tcache_bins[] thread cache 回收站
1. 一个线程只会选择一个 arena; 一个 arena 可以由多个线程共用
2. 全局 arena 有两处入口, 链表 和 主arena
    1. 链表 `static mstate free_list;`
    2. 首个 `static struct malloc_state main_arena;`
3. 线程首次选择 arena 的方式有3种: (依次选择) (`arena_get2()`)
    1. 找之前没用过的 `get_free_list()`
    2. 复用已经用过的 `reused_arena()`
    3. 尝试新创建一个 `_int_new_arena()`
4. 后续线程将会复用上一次使用的同一个 arena
   如果加锁失败; 将阻塞等待
5. 每个线程有一个 `tcache_bins[TCACHE_MAX_BINS]`, 小缓存尺寸的 `chunk`
   一个 `tcache_bins[i]` 里面 `chunk` 尺寸相同
   不同 `tcache_bins[i]` 按照 `chunk` 尺寸排序
   `tcache_bins[i]` 的 `chunk` 不会分割; 如果大小合适; 将会到 `arena` 中查找

# 7. 线程切换 arena
1. 当线程在当前 arena 无法申请内存, 即经过下列尝试都找不到
    1. `tcache_bins[]`
    2. `fastbins[]`
    3. `small bin`
    4. 合并 `fastbins[]` 后 `large bin`
    5. 合并 `fastbins[]` 后 `small bin`
    6. `top_chunk`
    7. sbrk 扩展 heap
    7. mmap 创建新的 heap 都失败
2. 那么将会进行, 这种切换只能是
    1. 由 `mmap arena` 切到 `main_arena`
    2. 由 `main_arena` 切到 `mmap arena`
3. 切了一次不行, 那就申请失败, 不会再一次申请中频繁切
4. 实际上在低内存运行环境中, 确实存在频繁切的情况
   当然是在切换后都能申请到 or 申请失败进程不自杀

# 11. malloc > public_mALLOc
```c++
#define public_mALLOc    malloc
```
1. 如果有 hook 函数, 返回 hook 函数
2. 根据字节数获取 arena, 获取不到返回失败
3. 首次调用 `_int_malloc` 传入 arena 和 字节数
4. 首次调用 `_int_malloc` 申请成功
5. 首次调用 `_int_malloc` 申请失败
    1. arena 不是 `main_arena`; 再次调用 `_int_malloc`, 传入 `main_arena`,
    2. arena 就是 `main_arena`; 再次调用 `_int_malloc`, 传入 `arena_get2()` 新找到 arena
7. 返回申请到的内存

# 12. _int_malloc (逐个步骤找 chunk 找到提前返回)
算法逻辑
1. 在 `tcache_bins[]` 找 size 完全一样的 chunk
2. 如果 size 足够大 `M_MMAP_THRESHOLD`, 那么直接 mmap 返回
3. 在 `fastbins[]` 中找合适 size 的 chunk (不一定完全相同)
   把同尺寸的 `fastbins[]` 挪到 `tcache_bin[]`
   (这是 for 循环 malloc 对象数组的优化)
4. 在 `small bin` 中找合适 size 的 chunk (不一定完全相同)
   可能会把同尺寸的 `chunk` 传入 `tcache_bin[]`
   (这是 for 循环 malloc 对象数组的优化)
5. 遍历 `fastbins[i]` 并挪到 `unsorted bin`
   遍历 `unsorted bin`, 合并邻接为 free 的 `chunk`
   把 `unsorted bin` 的 `chunk` 挪到对合适的 `bins[]`
   该过程中, 如果发现合适的大小的 `chunk` 则提前返回
   (唯一 加入到 `small bin` 和 `large bin` 的入口)
   (`unsorted bin` 中由于提前返回, 可能还保留 `chunk`)
   (一次遍历一个 `fastbins[i]`)
   (无论 size 大小; 只要步骤4. 不满足; 步骤5. 至少遍历一次)
6. size 足够大, 则在 `large bin` 中找合适 size 的 chunk, 
   可能会分割, 一部分返回用户, 另一部分加入到 `unsorted bin`
   (如果 size 足够大, 多次合并 `fastbins[i]`
    将会导致后续不容易申请小内存, 所以这里不重复)
7. size 不够大, 那么重复步骤5. 直到所有 `fastbins[i]` 为空
8. 如果 `top_chunk` 够大, 则分割一部分返回给用户
9. 申请新的 heap
10. 返回失败 

# 21. free > public_fREe
```c++
#define public_fREe      free
```
# 22. _int_free (逐个步骤找 chunk 可以放的位置)
1. 检查 `tcache_bin[]` 有没有空间放
2. 如果 `chunk` size 足够小, 放在合适的 `fastbins[]` 中 (此处不合并)
3. 如果 `chunk` size 足够大, 而且是一整块 `mmap`, 直接 `munmap`
4. 合并邻接为 free 的 `chunk` 
5. 如果 `chunk` 不是 `top_chunk` 放入 `unsorted bin`
6. 否则 `chunk` 就是 `top_chunk`
   如果 `chunk` size 足够大, 那么返回一部分内存给系统 (munmap / brk / sbrk)
   因为系统调用耗时, 这个操作在一定次数 free 满足条件后才执行

# 31. realloc > public_rEALLOc
```c++
#define public_rEALLOc   realloc
```
1. 一整个 mmap 出来的, 直接 mremap
2. 如果是缩小, 剩余 size 足够大 
   那么分割出 chunk 加入 arena `unsorted bin`
3. 如果是缩小, 剩余 size 不够大,
   返回原始指针
4. 如果是扩展, 后续邻接是 `top_chunk`, 
   `top_chunk` 足够大, 分割 `top_chunk`
   返回原始指针
5. 如果是扩展, 后续邻接是 free 的 `chunk`
   后续`chunk` 足够大, 合并后再看看是否需要分割后续`chunk`
   返回原始指针
6. 如果是扩展, 步骤4. 步骤5. 不能满足, 重新 malloc

# 91. reference urls
https://reverseengineering.stackexchange.com/questions/15033/how-does-glibc-malloc-work 
http://gee.cs.oswego.edu/dl/html/malloc.html // a longer but slightly out of date high-level description
glibc内存管理ptmalloc源代码分析
https://www.cnblogs.com/luoleqi/p/12731875.html
https://reverseengineering.stackexchange.com/questions/15033/how-does-glibc-malloc-work
ftp://ftp.cs.utexas.edu/pub/garbage/allocsrv.ps