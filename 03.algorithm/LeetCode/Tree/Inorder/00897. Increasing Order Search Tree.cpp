/*
 * @lc app=leetcode id=897 lang=cpp
 *
 * [897] Increasing Order Search Tree
 *
 * https://leetcode.com/problems/increasing-order-search-tree/description/
 *
 * algorithms
 * Easy (75.86%)
 * Total Accepted:    158.9K
 * Total Submissions: 209.3K
 * Testcase Example:  '[5,3,6,2,4,null,8,1,null,null,null,7,9]'
 *
 * Given the root of a binary search tree, rearrange the tree in in-order so
 * that the leftmost node in the tree is now the root of the tree, and every
 * node has no left child and only one right child.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: root = [5,3,6,2,4,null,8,1,null,null,null,7,9]
 * Output: [1,null,2,null,3,null,4,null,5,null,6,null,7,null,8,null,9]
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: root = [5,1,7]
 * Output: [1,null,5,null,7]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in the given tree will be in the range [1, 100].
 * 0 <= Node.val <= 1000
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
    TreeNode* increasingBST(TreeNode* root) {
        TreeNode *res = nullptr, *step = nullptr;
        increasingBST(root, step, res);
        return res;
    }
private:
    void increasingBST(TreeNode* root, TreeNode* &step,TreeNode* &res) {
        // 中序遍历; 缓存结果res指针和res的步进指针
        if (!root) return;
        increasingBST(root->left, step, res);
        if (!res) {
            res = new TreeNode(root->val);
            step = res;
        } else {
            step->right = new TreeNode(root->val);
            step = step->right;
        }
        increasingBST(root->right, step, res);
    }
};
