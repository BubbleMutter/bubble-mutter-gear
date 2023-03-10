[TOC]
# 1. 排序算法
## 1.1 选择排序
### 1.1.1 算法思想
1. 在未排序元素中, 从前往后遍历找到最小值, 放到待排序元素的最左边
2. "放在"的本质 是 swap操作
### 1.1.2 算法流程
1. 每次从待排序 数值中选取最小值; 
2. 放在待排序 数值的最前面
3. 对剩下的元素重复 1. ~ 2.
4. n个元素则重复n-1次; 直到排序完成

## 1.2 插入排序
### 1.2.1 算法思想
1. 选取未排序元素; 从后往前扫描在已排序元素; 找到相应位置后插入
2. "插入"是形象化的描述; 实际上是在一个循环中把数值 `a[i]` 不断从右往左移动
### 1.2.2 算法流程
1. 假设 `a[0:i-1]` 已排序; 选取 `a[i]`
2. `a[i]` 从后往前 逐个与 `a[i-1]` ~ `a[0]` 比较
3. 如果 `a[i]` 数值相对较少; 则 `a[i]` 左移
4. 直到 `a[i]` 插入相应的位置(即 左方的数值比 `a[i]` 小)
5. 重复 1. ~ 4. n-1 次

## 1.3 冒泡排序
### 1.3.1 算法思想
1. 在未排序元素中, 从前往后遍历, 把较大值右移, 直到最大值移到最右边
### 1.3.2 算法流程
1. 从左往右依次比较相邻的元素; 把较大的元素右移 (即交换)
2. 单次循环 直到 最后一对相邻元素 `a[n-1-i-1]` 和 `a[n-1-i]` (i 为循环次数)
3. 重复 1. 2. `n-1` 次
### 1.3.3 算法特征
1. 每次遍历后, 最右边元素 为 最大值
2. 排序过程中, 待排序数值 从右边往左边 从大到小 逐渐完成排序

## 1.4 快速排序
### 1.4.1 算法思想
1. 在未排序元素中, 选取基准值 pivot,
   把小于基准值的放到左边,
   把大于基准值的放到右边
2. 把左右两边元素, 分别作为未排序元素递归重复 1. 
### 1.4.2 算法流程
1. 选取基准值 pivot
2. 将数组分成两个子数组: 小于基准值的一组 和 大于基准值的一组 partition
3. 分别对两个子数组进行快速排序
4. 重复1. ~ 3.
### 1.4.3 算法特性
1. 快速排序的主题思想上是 分治+递归
2. 递归本质:
    1. 基线条件: `子数组长度 < 2` (实际上是两个条件: 数组为空 + 数组长度为1)
    2. 递归操作: 选取基准值; 拆分两数组
    2. 递归条件: (无条件) 分别对两个子数组进行排序
3. 排序效率并不恒定, 取决于pivot的选取,
   如果pivot总是在子数组端点; 则根本没有拆分; 复杂度为 `O(n**2)`
### 1.4.4 时间复杂度
1. 理想情况下, 每次选取的 pivot 都能把未排序元素划分为数量相同的两个部分
2. 那么, 选取一个pivot, 遍历n个元素, 划分为两个部分
   然后, 选取两个pivot, 分别遍历 n/2 个元素, 划分为四个部分
   然后, 选取四个pivot, 分别遍历 n/4 个元素, 划分为八个部分
   如此类推
   最后, 选取 n个pivot, 分别遍历 1   个元素
   由此, 每个这样的步骤都需要遍历 n 个元素,
         一共需要 log2n 这种步骤, 即可 `O(n*log2n)`

## 1.5 归并排序 (迭代法 / 递归法)
### 1.5.1 算法思想
1. 先 2个 2个地排序; 再 4个 4个地排序; 然后 8个 8个地排序; ...
### 1.5.2 算法步骤 (迭代法)
1. 申请空间; 尺度同于待排序数组
2. 设定两个指针; 两个最初位置分别指向两个已排序数组的起始位置
3. 比较两个指针指向的元素; 选择相对较少的元素放入合并空间
4. 重复步骤3; 直到两个指针任意一个丞相序列末尾
5. 将另一个序列剩下的所有元素直接复制到合并序列的尾部

## 1.6.1 堆排序
### 1.6.1 算法思想
1. 将待排序数组构造成一个大顶堆
    + 此时，整个序列的最大值就是堆顶的根节点。
2. 将首元素与末元素进行交换
    + 此时末尾就为最大值。
3. 然后将剩余n-1个元素重新构造成一个堆
4. 重复2 3步骤; 直到剩余元素个数为 0
### 1.6.2 算法流程
1. 从首个非叶节点 `[n/2 - 1]` 开始到首元素`[0]` 构造大顶堆
2. 首元素与末元素交换
3. 交换后 数组尺寸减一 剩余数组重新构造大顶堆
4. 重复2 3 直到数组长度为空
5. 大顶堆构造流程
    1. 从当前节点的左右子节点中; 找到 "较大子节点"
    2. 较大子节点 与 "当前节点" 交换
    3. 重新进行 1 2; 直到 "当前节点" 都大于子节点 or "当前节点" 变为叶节点
### 1.6.3 算法特性
1. 二叉堆的性质
    1. 索引为 i 的左节点 索引是 `2*i+1`
    2. 索引为 i 的右节点 索引是 `2*i+2`
    3. 索引为 i 的父节点 索引是 `floor((i-1) / 2)`
### 1.6.4 时间复杂度
1. 首次构建堆的复杂度为 `O(n)`
2. 每次重建的复杂度 `log2(n-1) log2(n-2) ...` 近似为 log2n
   一共需要 `n - 1` 的交换并重建
4. 复杂度 = `O(n) + O(n*log2n) = O(n*log2n)`

## 1.7 希尔排序
### 1.7.1 算法思想
1. 将待排序元素按下标的一定增量分组, 分成多个子序列
2. 然后对各个子序列按照插入排序算法排序
3. 然后依次缩减增量再进行排序
4. 直到增量为1时, 进行最后一次直接插入排序, 排序结束
### 1.7.2 算法流程
1. 通过步进划分小组; 初始步进是 `a.size() / 2`; 每次循环步进除以2
2. 遍历每个小组; 每个小组从末元素开始遍历;
3. 每个小组遍历中  使用插入法排序
4. 直到步进为0, 结束算法
### 1.7.3 算法特性

### 1.7.4 时间复杂度

## 1.8 基数排序
### 1.8.1 算法思想
### 1.8.2 算法流程
1. 获取数组a中的最大值
2. 求最大指数(即最大值的位数)。
3. 从指数1开始到最大指数，根据位数对数组a中的元素进行桶排序

## 1.9 计数排序
### 1.9.1 算法思想
### 1.9.2 算法流程
1. 根据待排序集合中最大元素和最小元素的差值范围，申请额外空间；
2. 遍历待排序集合，将每一个元素出现的次数记录到元素值对应的额外空间内；
3. 变换额外空间, 计算每一个元素在排序后最后出现的索引
4. 从后边开始; 将待排序集合每一个元素移动到3步骤计算得出的正确位置上。

## 1.10 桶排序
### 1.10.1 算法思想
### 1.10.2 算法流程
1. 根据待排序集合中最大元素和最小元素的差值范围和映射规则，确定申请的桶个数；
2. 遍历待排序集合，将每一个元素移动到对应的桶中；
3. 对每一个桶中元素进行排序，并移动到已排序集合中。

# 2. 总结
## 2.1 算法比较
| 类型   | 排序算法       | 平均时间复杂度 | 最坏情况     | 最好情况     | 空间复杂度 | 稳定性 | 每次选出最终位置 | 复杂度与初始位置 |
| :-:    | :-:            | :-:            | :-:          | :-:          | :-:        | :-:    | :-:              | :-:              |
| 内排序 | 选择排序(选择) | `O(n**2)`      | `O(n**2)`    | `O(n**2)`    | `O(1)`     | 不稳定 | 能, 选出最小元素 | 无关, 必须遍历   |
| 内排序 | 插入排序(插入) | `O(n**2)`      | `O(n**2)`    | `O(n)`       | `O(1)`     | 稳定   | 否               | 有关, 能提前结束 |
| 内排序 | 冒泡排序(交换) | `O(n**2)`      | `O(n**2)`    | `O(n)`       | `O(1)`     | 稳定   | 能, 选出最大元素 | 有关, 能提前结束 |
| 内排序 | 快速排序(交换) | `O(n*log2n)`   | `O(n**2)`    | `O(n*log2n)` | `O(log2n)` | 不稳定 | 否               | 有关, 影响复杂度 |
| 内排序 | 归并排序       | `O(n*log2n)`   | `O(n*log2n)` | `O(n*log2n)` | `O(n)`     | 稳定   | 否               | 无关，都要归并   |
| 内排序 |  堆排序 (选择) | `O(n*log2n)`   | `O(n*log2n)` | `O(n*log2n)` | `O(1)`     | 不稳定 | 能, 选出最大元素 | 无关, 都是建堆   |
| 内排序 | 希尔排序(插入) | `O(n*log2n)`   | `O(n*log2n)` | `O(n*log2n)` | `O(1)`     | 不稳定 | 否               | 有关, 能提前结束 |
| 外排序 | 基数排序       | `O(n * k)`     | `O(n * k)`   | `O(n * k)`   | `O(n + k)` | 稳定   | -----            | 无关             |
| 外排序 | 计数排序       | `O(n + m)`     | `O(n + m)`   | `O(n + m)`   | `O(n + m)` | 稳定   | -----            | 无关             |
| 外排序 |  桶排序        | `O(n + m)`     | `O(n + m)`   | `O(n + m)`   | `O(m)`     | 稳定   | -----            | 无关             |

## 2.1 算法稳定性
1. 算法稳定性是指: 相同数值的元素; 经过排序后; 其相对顺序保持不变
   + 不是时间复杂度的稳定性