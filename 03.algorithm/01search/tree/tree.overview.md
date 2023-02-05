[TOC]
# 原理总览
总结秘籍 https://www.jianshu.com/p/ed7935fd510f
总结秘籍 https://zhuanlan.zhihu.com/p/27700617
平衡实现 https://zhuanlan.zhihu.com/p/94130997  (动画的旋转逻辑是有漏洞的)
红黑树   https://www.jianshu.com/p/e136ec79235c (超级详细; 认真看)

# btree
## 遍历  (X序 X表示根的位置)
1. 前序遍历: 根左右
2. 后续遍历: 左右根
3. 中序遍历: 左根右
4. 层次遍历: 广度优先的队列方法
5. 栈遍历:   
## 已知 前/中/后 两种遍历方式; 求另一种
1. 已知前列 中序 求 后序
2. 已知后序 中序 求 前列
3. 已知前序 后序 无法确认一棵树? 无法求得中序遍历

# avl tree
https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
https://www.geeksforgeeks.org/avl-tree-set-2-deletion/

# rbtree
https://www.geeksforgeeks.org/red-black-tree-set-1-introduction-2/
https://www.geeksforgeeks.org/red-black-tree-set-2-insert/
https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/

# b-tree
https://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
https://www.geeksforgeeks.org/b-tree-set-1-insert-2/
https://www.geeksforgeeks.org/b-tree-set-3delete/
https://www.tutorialspoint.com/cplusplus-program-to-implement-b-tree

# b+tree
https://www.geeksforgeeks.org/introduction-of-b-tree/?ref=rp
https://www.geeksforgeeks.org/difference-between-b-tree-and-b-tree/?ref=rp
https://www.tutorialspoint.com/cplusplus-program-to-implement-bplus-tree

# trie tree
https://www.geeksforgeeks.org/trie-insert-and-search/
https://www.geeksforgeeks.org/trie-delete/
https://www.geeksforgeeks.org/implement-reverse-dns-look-cache/
https://www.geeksforgeeks.org/implement-forward-dns-look-cache/

# r-tree
https://www.geeksforgeeks.org/introduction-to-r-tree/?ref=rp

# splay tree
https://www.geeksforgeeks.org/splay-tree-set-1-insert/
https://www.geeksforgeeks.org/splay-tree-set-2-insert-delete/

# segment tree
https://www.geeksforgeeks.org/segment-tree-set-1-sum-of-given-range/
https://www.geeksforgeeks.org/segment-tree-set-1-range-minimum-query/
https://www.geeksforgeeks.org/lazy-propagation-in-segment-tree/
https://www.geeksforgeeks.org/persistent-segment-tree-set-1-introduction/