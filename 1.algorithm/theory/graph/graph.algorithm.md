[TOC]
无向图 深度优先搜索 得到的序列不唯一 

# 理论
+ 同一个图的 **BFS搜索树的深度** 小于等于 **DFS搜索树的深度**  
## 广度优先搜索 (BFS) (Breadth-First Search)
1. 辅助数据
    + 待访问队列
    + 节点深度数组 depth[] 记录第i个节点第几次被访问
    + 节点前驱数组 prev[]  记录第i个节点的前继index (无向图 前驱唯一)
2. 节点第一个节点 depth为0 prev为-1
3. while 队列尺寸大于0
    1. 队列首x 出列
    2. 获取 队列首的后继节点集 y
    3. 对于每一个y 如果未被访问(depth为-1) 则进入队列
    4. `depth[y] = depth[x]; pre[y] = x`
+ 经常是最短最少的问题

## 深度优先搜索 (DFS) (Deep First Search)
+ 常常是递归
+ 有时是堆栈
+ 常见问题 时间戳 / 颜色 / 父子关系 / LCA / TwoSum / KSum
+ 注意DFS 只能穷举全部后才能得到最优解 (NP 问题?)
+ 相较于BFS常常是找最短/最少

# 经典问题
## Q: 最短路径 广度优先法 (只适无权图)
```c++
// 1. 创建一个队列候选; 一个hash记录已检索
// 2. 起点入列
// 3. 循环: 非空队列
// 4. 出列一个元素;
// 5. 如果元素在hash中; continue; 表示已经检索
// 6. 如果图节点(node)是目的地 结束循环
// 7. 否则把该节点的下一跳入列
```
## Q: 最短路径 Dijkstra's Algorithm (可用于有权图; 仅用于 有向无环图 权值必须为正)
构造已处理hash; key: 节点 value: { 最近父节点, 开销 }
构造待处理队列
### 算法思路
1. **起点** 插入待处理队列
2. 待处理队列中 找到 开销最小的节点 出列
3. 遍历节点的邻居 (邻居的开销 = 当前节点在hash的开销 + 图-节点-邻居开销)
   1. 如果 邻居不在hash中则插入到hash 和 待处理队列
   2. 否则 检查是否有前往邻居的更短路径
   3. 如果 有更短路径 则更新到该邻居节点在hash的开销 和 最近父节点
4. 重复 2. 3.; 直到对图中的每个节点都处理过了 (待处理队列为空)
5. 回溯最终路径; 终点->父节点->父节点->...->起点

### 算法思想
1. 缺陷
    1. 该算法建立在一个假设上: 已处理的节点; 没有前往该节点更短的路径
    2. 这种假设建立在没有 "负数权边" 的前提
    3. "负数权边" 可以使得已处理的节点 更新后有更少开销; 
    4. 但是该算法只对单个节点的处理一次, 最终"负数权边"的路径被忽略
2. hash 表的本质是 从起点到达该节点的最小开销 的表

### 代码实现
```c++
// Map<> 二维hash表示图
// Map<> 结果

```

## Q: 最短路径 (贝尔曼-福德 Algorithm) (可用于有权图; 仅用于 有向无环图; 权值可为负)

## Q: MST 最小生成树 (Prim算法 Kruskal算法)
+ 带权无向图中生成一棵树 树包含全部节点 权值最小

## Q: 给定数字字符串 返回可能的IP地址组合 (DFS)
+ 穷举插入3小数点的情况 `O(n^3)`

## Q: 八皇后   (DFS)

## Q: 数独穷举 (DFS) 

## Q: 单词变换
+ 给定 词典 + 起点单词 + 终点单词
    1. 每次只变换一个字母 变换后单词要在词典中
    2. 能否 通过起点单词 到达 终点单词 ?
    3. 最短路径是怎么样的
+ 例子
    + 起点 hit 终点 cog
    + 词典 hot dot dog lot log
    + 结论 hit > hot > dot > dog > cog
+ 构造 词典n*n矩阵, 能相连的两个单词`[i][j]`设为1
+ 遍历词典 找到可以成为 起点后继的单词集U
+ 遍历词典 找到可以成为 终点前驱的单词集V
+ 最终问题 变为最短路径生成算法
    + 每对 U中元素和V中元素 的路径
    + Dijkstra 和 Floyd 算法 (BFS)

## Q: 周围区域问题 (连通域问题 围棋问题) (BFS)
+ 找到图中 被黑子围起来的 所有白子
    + 边缘区域 是否起到包围作用 (此处是两种情况)
    + 八连通还是四连通
+ BFS 算法
+ 运行中 每个格子有 5种状态
    1. 黑子
    2. 空格
    3. 未决白子
    4. 已决白子 被包围
    5. 已决白子 不连通
+ 算法
```py
def bfs_white(chess, i, j):
    pend_queue # 待决白子队列 bfs搜搜队列
               # 记录　坐标点 和 方向 0, 1, 2, 3, 4
               # 方向0 仅仅是第一个点
    visited_queue # 已决白字队列
    isConnected = True # 区域连通 (开始假设全连通)
    pend_queue.push((i, j, 0)) # 入列 
    while len(pend_queue) > 0:
        x = pend_queue.pop()  # 出列
        visited_queue.push(x) # 入列visited
        # 对每个 坐标三个方向的每个格子 (首个坐标有4个方向)
        # 如果格子是白子 则pend_queue.push入列 记录方向
        # 如果格子是空格 则isConnected = False
        pass
    for x in visited_queue:
        # 根据 isConnected True设置为 '已决白子被包围'
        # 否则 设置为 '已决白子不连通'
        pass
def visit_chess(chess, height, width):
    for i in height:
        for j in width:
            if chess[i][j] == '未决白子':
                bfs_white(chess, i, j)
```

## Q: 判断有向图是否有环
1. 拓扑排序法: 如果成环, 拓扑排序法失败
2. 深度优先遍历: 给节点打上 **已遍历标记**; 有环则可以重新访问 **已遍历标记** 为 true 的节点
3. 广度优先遍历: 给节点打上 **已遍历标记**; 有环则可以重新访问 **已遍历标记** 为 true 的节点