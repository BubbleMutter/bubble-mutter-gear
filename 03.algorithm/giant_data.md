高并发 https://bbs.huaweicloud.com/blogs/194361?utm_source=pc_p&utm_medium=oschina&utm_campaign=bowen&utm_content=cyuyan 

## Q: 1亿数据在有限内存上如何排序
https://blog.csdn.net/qq_39521554/article/details/79546854

## 大数据小内存TOPK,排序问题
https://blog.csdn.net/imudges_Zy/article/details/89735977?utm_medium=distribute.pc_aggpage_search_result.none-task-blog-2~all~first_rank_v2~rank_v25-2-89735977.nonecase&utm_term=%E5%A4%A7%E6%95%B0%E6%8D%AE%E5%B0%8F%E5%86%85%E5%AD%98%E6%8E%92%E5%BA%8F

## Q: 亿万 url 查找问题
十万 url 查找问题

## Q: 十亿 账号, 查找其中一个是否存在 (bitmap)
1. 量级概念
    1. 1bytes, 可以保存 8个账号
    2. 1k,     可以保存 8千账号
    3. 1M,     可以保存 8百万账号
    3. 64M,    可以保存 6千万账号  (大概)
    4. 128M,   可以保存 1.3亿账号  (大概)
    5. 256M,   可以保存 2.6亿账号  (大概)
    6. 526M,   可以保存 5.5亿账号  (大概)
    7. 1G,     可以保存  10亿账号  (大概)
2. 因此, 10亿账号, 需要1G内存的bitmap 表示存在与否
   可以一次性存入内存中, 不需要额外磁盘空间