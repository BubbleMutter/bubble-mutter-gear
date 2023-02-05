/*
 * @lc app=leetcode id=617 lang=cpp
 *
 * [617] Merge Two Binary Trees
 *
 * https://leetcode.com/problems/merge-two-binary-trees/description/
 *
 * algorithms
 * Easy (77.37%)
 * Total Accepted:    510K
 * Total Submissions: 658.9K
 * Testcase Example:  '[1,3,2,5]\n[2,1,3,null,4,null,7]'
 *
 * You are given two binary trees root1 and root2.
 * 
 * Imagine that when you put one of them to cover the other, some nodes of the
 * two trees are overlapped while the others are not. You need to merge the two
 * trees into a new binary tree. The merge rule is that if two nodes overlap,
 * then sum node values up as the new value of the merged node. Otherwise, the
 * NOT null node will be used as the node of the new tree.
 * 
 * Return the merged tree.
 * 
 * Note: The merging process must start from the root nodes of both trees.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: root1 = [1,3,2,5], root2 = [2,1,3,null,4,null,7]
 * Output: [3,4,5,5,4,null,7]
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: root1 = [1], root2 = [1,2]
 * Output: [2,2]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in both trees is in the range [0, 2000].
 * -10^4 <= Node.val <= 10^4
 * 
 * 
 */
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
#if 0
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        if (!root1 && !root2) return nullptr;

        TreeNode *node = new TreeNode;
        if (root1)
            node->val += root1->val;
        if (root2)
            node->val += root2->val;
        node->left = mergeTrees(root1 ? root1->left : nullptr,
                                root2 ? root2->left : nullptr);
        node->right = mergeTrees(root1 ? root1->right: nullptr,
                                 root2 ? root2->right: nullptr);
        return node;
    }

#else  /* 会修改输入的优化算法 */
    TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
        // 1. 两树为空; 返回空
        // 2.  t1 不空; t2为空; 返回t1
        // 3.  t1 为空；t2不空; 返回t2
        // 3. t1 t2 都不空; 分别合并两者的左右子树; 合并节点本身的值
        if (!t2) return t1;
        if (!t1) return t2;
        t1->val += t2->val;
        t1->left = mergeTrees(t1->left, t2->left);
        t1->right = mergeTrees(t1->right, t2->right);
        return t1;
    }
#endif
};
