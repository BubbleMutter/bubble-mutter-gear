[TOC]
## Q: 数组排序已经完成; 哪种算法排序最快? 哪些最慢?
1. 最快 = 插入排序 `O(n)`
2. 最慢 = 快速排序 `O(n**2)`
3. 另外 堆排序 归并排序都需要重新排列的, 达到平均复杂度
4. 误区 冒泡排序 和 选择排序不是最快的

# Q: 排序算法 哪些是稳定的? 
1. 选择/快速/堆/希尔

# Q: 排序算法 哪些是不稳定的?
2. 插入/冒泡/归并/计数排序/基数排序/桶排序

# Q: 选择排序 不稳定原因
1. 不稳定性发生在选择 本次排列最小的数值置换到数组首部
2. 置换后 数组首部放到了数值与自己相同元素 的后面
```log
2a 2b 1 | 排序前
1 2b 2a | 排序后
2a = 2b = 2 (数值)
```

# Q: 快速排序 不稳定原因
1. 不稳定发生在 `a[pivot]` 和 `a[i]` 交换的时刻
2. 存在未排序元素 `a, b, c1, ..., pivot, ..., c2`
   a, c1, c2, 小于 pivot; b 大于 pivot,
   那么排序后元素 `a, c2, c1, ..., pivot, ..., b`

## Q: 希尔排序 不稳定原因
直接插入排序中，每次插入排序是稳定的
不会改变相同元素的相对顺序
但希尔排序在分组插入排序过程中
相同的元素可能在各自的插入排序中移动
最后其稳定性就会被打乱

# Q: 快速排序在序列已经有序的场景下 复杂度是多少?
1. `O(n**2)`

# Q: 快速排序 pivot 选取的3个方法 (first / second / median)
1. first  每次只选择子数组的首部数字作为pivot
2. second 每次只选择子数组的末尾数字作为pivot
3. median 选择 "末尾 / 首部 / 数组中间" 3者的中位数

# Q: 大数据小内存如何使用归并排序进行排序
```log
假设有 1G 内存; 8G数据
1. 分别10G数据 拆分为 8 份; 归并排序
2. 对 0~0.5 G数据 + 1.0~1.5 G 数据进行归并排序
3. 此时, 有生于 xG 数据；
  可能属于 0~0.5里面 也可能属于 1.0~1.5G里面
4. 然后 0.5~1.0 G, 1.5~2.0 G 中 读取 (1-x)G, 剩余 y G
5. 取得的 (1-x)G 与 xG 进行归并排序
6. 剩余 zG 与 之前剩余的 yG 进行归并排序
7. 从而获得 2G 排序好的数据; 
8. 重复步骤 2 ~ 5, 使得8G数据 每2G是排序完成的
9. 思想类似, 重复之前步骤; 直到8G排序完成
```

## Q: 归并排序如何进行并行; 假设线程数为N ?

## Q: 希尔排序比插入排序快在哪里
希尔排序是按照不同步长对元素进行插入排序，
当刚开始元素很无序的时候，步长最大
所以插入排序的元素个数很少(小组内个数少)，速度快
当元素基本有序，步进较小, 相当于使用原始插入排序
对于有序的序列, 插入排序效率是最高的算法之一

# 8 基数排序 (外排序) (稳定)
## Q: 原理描述 [标准术语]
1. 将所有待比较数值统一为同样的数位长度，
    + 数位较短的数前面补零。
2. 从最低位开始，依次进行一次排序
3. 最低位排序一直到最高位排序完成
## Q: 时间复杂度描述
1. `O(D*(N + R))`
    1. D 是最大值的位数
    2. N 是元素个数
    3. R 是基数, 一般为10 (发生在 变换`buckets[]`为 位置数组)
## Q: 空间复杂度描述
1. `O(N+R)`
    1. N 是元素个数 (每个指数 进行桶排序时; 临时存储数组)
    2. R 是基数, 一般为10 (桶的大小  buckets)
## Q: 分配与回收 次数?
1. 分配与回收次数即 最大值的位数

## Q: 是否适合字符串为关键字的情况?
1. 适合;
    1. 把字母位置当作是位数;
    2. 分配`buckets[26]`;
    3. 从首字母开始遍历

# 9 计数排序 (外排序) (稳定)
适用于一定范围的整数排序。在取值范围不是很大的情况下，它的性能在某些情况甚至快过那些O(nlogn)的排序
## Q: 原理描述 [标准术语]
根据元素本身的值，
将每个元素出现的次数记录到辅助空间，
通过对辅助空间内数据的计算
即可确定每一个元素最终的位置
## Q: 时间复杂度描述
1. 第一个循环用于在额外空间中记录每一个元素出现的次数，复杂度为 O(N)
2. 第二个循环用于计算每一个元素的最终位置，复杂度为 O(K)
    + K 为申请的额外空间大小; 即最大值与最小值的差
3. 第三个循环用于移动待排序集合中元素到已排序集合的正确位置上，复杂度为 `O(N)`
## Q: 空间复杂度描述
1. `O(N + K)`
    + K 为申请的额外空间大小; 即最大值与最小值的差
    + N 用于临时存储结果

# Q: 为什么说 快速排序 绝大多数情况下优于 归并排序
1. 因为平均复杂度 快速排序 同与 归并排序
2. 但是 快速排序的 `O(nlog2n)` 的常数因子 比起 归并排序要小得多

# Q: 在大量数据中选取最小 或 最大的若干个; 最快的算法是? 为什么?
堆排序 = O(n) 复杂度初始化堆, 遍历若干次即可获取

# Q: 在大量数据中选取最小 或 最大的若干个; 使用归并排序算法如何实现?

# Q: 在大量数据中选取最小 或 最大的若干个; 使用快速排序算法如何实现?

# Q: 可以并行的排序算法有哪些? (归并排序 基数排序)

# Q: 最适合对双向链表进行排序的算法?（快速排序)

# Q: 对中文词条的拼音进行排序; 要求保留拼音串相同的中文词条相对位置; 哪些算法合适
1. 这个问题本质上是问稳定的排序算法, 归并排序

# Q: 一个班有100个人; 分数满分是100分; 对ta们进行排序; 哪种排序方式比较好?
桶排序最好, 在数值范围较少的场景中, 外排序使用桶排序
时间复杂度O(n+m), 空间复杂度O(m)