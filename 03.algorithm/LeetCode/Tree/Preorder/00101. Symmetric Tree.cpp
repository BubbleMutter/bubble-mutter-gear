/*
 * @lc app=leetcode id=101 lang=cpp
 *
 * [101] Symmetric Tree
 *
 * https://leetcode.com/problems/symmetric-tree/description/
 *
 * algorithms
 * Easy (47.99%)
 * Total Accepted:    830.7K
 * Total Submissions: 1.7M
 * Testcase Example:  '[1,2,2,3,4,4,3]'
 *
 * Given the root of a binary tree, check whether it is a mirror of itself
 * (i.e., symmetric around its center).
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: root = [1,2,2,3,4,4,3]
 * Output: true
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: root = [1,2,2,null,3,null,3]
 * Output: false
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in the tree is in the range [1, 1000].
 * -100 <= Node.val <= 100
 * 
 * 
 * 
 * Follow up: Could you solve it both recursively and iteratively?
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

/* 递归法 */
class Solution {
    // 1. 空树是对称的
    // 2. 两树对称的数学本质 满足以下条件:
    //    a) 两个空树对称
    //    b) 根节点相等
    //    c) x左子树 和 y右子树 对称
    //    d) x右子树 和 y左子树 对称
    bool isSymmetric(TreeNode* x, TreeNode* y) {
        return (!x && !y) || (x && y &&
                x->val == y->val &&
                isSymmetric(x->left, y->right) &&
                isSymmetric(x->right, y->left));
    }
public:
    bool isSymmetric(TreeNode* root) {
        return !root || isSymmetric(root->left, root->right);
    }
};
