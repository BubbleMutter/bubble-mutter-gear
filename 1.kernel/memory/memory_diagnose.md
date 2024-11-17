[TOC]
# /proc/slabinfo; slabtop

# /proc/meminfo; vmstat
```txt
MemFree:           62028 kB
Cached:            55616 kB
Buffers:            5240 kB
Slab:              <---- is the total number of /proc/slabinfo
```
inode_cache shmem_inode_cache 一般来说是有内核泄露; 需要配合 kmemleak; 排查

## buffer vs cached (命令行工具 free)
1. Buffer is something that has yet to be "written" to disk.
2. Cached is something that has been "read" from the disk and stored for later use.
3. Running Description (https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=34e431b0ae398fc54ea69ff85ec700722c9da773)
    1. **Cached** will be consumed more and more when it is available. (for high performance)
    2. **Cached** rises up then MemFree goes down ordinarily.
    3. **Cached** will be free when allocation of memory has been stricted.
    4. **Cached** goes down but MemFree keep rising up at a long time, when **something** wrong(like out-of-big-page),
4. 运行描述
   1. 正常情况下, Cached 会尽可能地使用内存, 如果 MemFree 不够用, Cached才会一点一点释放.
   2. Cached 释放得少, 且 MemFree 上涨得慢, 因为释放出来的被使用了。
   3. 如果 Cached 大幅下降, 可能是因为需要大量内存, 这个时候MemFree按照理论不会大幅上涨。
   4. 如果 Cached 大幅上升, 且 MemFree 大幅上升, 即 Cached 释放后 MemFree 没有被使用, 这种场景中, 很有可能是 out-of-big-page (查看 /proc/buddyinfo 确认)

# /proc/buddyinfo
1. 对应 page-size: 1、2、4、8、16、32、64、128、256、512、1024
2. 越往右, page-size 越大;
3. 内核内存合并选项 `CONFIG_COMPACTION=y`