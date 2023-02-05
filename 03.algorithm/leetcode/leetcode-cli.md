https://leetcode-cn.com/problemset/all
右边有标签; 可以选的

排序/ 
动态规划 /
贪心算法 / 
红黑树
hash
树的各种遍历
分治
字符串处理

中等题真的要做大半天的; 而且可能是错的

# leetcode-cli 
## 登录问题
1. 文件 `~/.lc/leetcode/user.json`
2. 内容
```json
{
  "login": "[username]",
  "loginCSRF": "",
  "sessionCSRF": "[copied from csrftoken]",
  "sessionId": "[copied from LEETCODE_SESSION]"
}
```
## 插件安装
1. https://github.com/skygragon/leetcode-cli-plugins/tree/master/plugins 下载插件的js
2. 把下载到的 js 文件放置到 ~/.lc/ 目录中
3. 修改 ~/.lc/plugins.json 对应插件的的值, 改 false 为 true
## 常用命令
```sh
leetcode-cli list -q e -t string
leetcode-cli show <$id> -gxl cpp
leetcode-cli submit $filename
leetcode-cli show <%id> --solution

# 搜索没有完成的算法题, 简单难度
leetcode-clit list -q eDL -t algorithms
```

# 记录
| 000039 | Combination Sum  | 做不出来 |
| 000043 | Multiply Strings | 时间太慢了; 待优化 |

# 2022 plan
+ `00094. Binary Tree Inorder Traversal`: 不会用递归方法, 即二叉树 中序遍历的循环方法
+ `00144. Binary Tree Preorder Traversal: 不会用递归方法, 即二叉树 前序遍历的循环方法
+ `00145. Binary Tree Postorder Traversal: 不会用递归方法, 即二叉树 后序遍历的循环方法
+ `00590. N Ary Tree Postorder Traversal`: N叉树后序遍历的循环方法
+ `00460. LFU Cache`:
+ `00146. LRU Cache`: lru-k 多次数入列; lru-2q, 双队列转移; lru-mq 多队列优化
+ `00028. strStr()`: 字符串子串算法
+ `00326. Power Of Three`: 使用对数方法是否符合不循环的约束? 本质问题是计算机如何实现对数?

# topic
## easy
1. DP                 (x) `leetcode-cli list -t dynamic-programming -q eDL`
2. Hash               () `leetcode-cli list -t hash-table -q eDL`
3. List               (x) `leetcode-cli list -t list -q eDL`
4. Stack              (x) `leetcode-cli list -t stack -q eDL`
5. Tree               (x) `leetcode-cli list -t tree -q eDL`
6. TwoPtr             (x) `leetcode-cli list -t two-pointers -q eDL`
7. Greedy             (x) `leetcode-cli list -t greedy -q eDL`
8. Divide and Con     (x) `leetcode-cli list -t divide-and-conquer -q eDL`
9. Bit                (x) `leetcode-cli list -t bit -q eDL`
10. Binary Search     (x) `leetcode-cli list -t binary-search -q eDL`
11. Sort              (x) `leetcode-cli list -t sort -q eDL`
12. Math              (x) `leetcode-cli list -t math -q eDL`
13. String            (x) `leetcode-cli list -t string -q eDL`
14. Array             (x) `leetcode-cli list -t array -q eDL`
21. BFS               () `leetcode-cli list -t breadth-first-search -q eDL`
22. DFS               () `leetcode-cli list -t depth-first-search -q eDL`

## level2 middle


# Array String 常用算法
## 全排列
## 前缀和
## 窗口法 (双指针)
## 双向遍历
00821. Shortest Distance To A Character.cpp
00941. Valid Mountain Array.cpp
