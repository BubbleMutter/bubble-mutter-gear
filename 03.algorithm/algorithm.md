算法的速度指的并非是时间 而是操作数的增速
即随着输入元素的增加 运行时间的增加速度

递归算法 = 基线条件(终止条件) + 递归操作(比如说print) + 递归条件
尾递归
分而治之 = 基线条件 + 不断问题分解直到符合基线条件
hash 的填装因子 = 散列表包含的元素个数 / 位置总数

O表示法 虽然忽略的常量的影响
算法复杂度 形式 不同时确实不用考虑
O(n*log2n) 一定远快于 O(n**2)
但是当两个算法的 复杂度形式相同时, 则需要考虑,
比如说 尽管快速排序算法时间不稳定`O(n**2) ~ O(n*log2n)` 而 归并排序稳定`O(n*log2n)`;
       但是从遍历的技术层面 归并排序耗时更长

# resources
https://github.com/labuladong/fucking-algorithm
https://www.geeksforgeeks.org/data-structures/?ref=ghm

# pending question
如何判断一个图是否成环
如何判断一个单链是否成环
如何判断两个单链是否相交

什么是拓扑排序算法

zero-one 背包问题用动态规划
普通背包问题用贪心算法
哈密顿回路

MapReduce 算法
LRU 算法
LFU 算法
钢条切割问题
小朋友过桥问题
01背包问题
购物单有依赖的背包问题
最多路径数问题
4键键盘问题
编辑距离

多路归并问题

数据结构 = 跳表 b+ b树 trietree
