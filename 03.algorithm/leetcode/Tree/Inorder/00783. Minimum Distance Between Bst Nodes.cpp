/*
 * @lc app=leetcode id=783 lang=cpp
 *
 * [783] Minimum Distance Between BST Nodes
 *
 * https://leetcode.com/problems/minimum-distance-between-bst-nodes/description/
 *
 * algorithms
 * Easy (55.64%)
 * Total Accepted:    113K
 * Total Submissions: 202.9K
 * Testcase Example:  '[4,2,6,1,3]'
 *
 * Given the root of a Binary Search Tree (BST), return the minimum difference
 * between the values of any two different nodes in the tree.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: root = [4,2,6,1,3]
 * Output: 1
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: root = [1,0,48,null,null,12,49]
 * Output: 1
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in the tree is in the range [2, 100].
 * 0 <= Node.val <= 10^5
 * 
 * 
 * 
 * Note: This question is the same as 530:
 * https://leetcode.com/problems/minimum-absolute-difference-in-bst/
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
    int minDiffInBST(TreeNode* root) {
        TreeNode * prev = nullptr;
        int res = INT_MAX;
        // 中序遍历, 缓存res和上一个节点
        minDiffInBST(root, prev, res);
        return res;
    }
private:
    void minDiffInBST(TreeNode* root, TreeNode* &prev, int& res) {
        if (!root) return;
        minDiffInBST(root->left, prev, res);
        if (prev) { // 前驱节点非空; 则更新 diff
            res = min(abs(prev->val - root->val), res);
        }
        // 无论前驱节点是否为空; 更新前驱节点
        prev = root;
        minDiffInBST(root->right, prev, res);
    }
};
