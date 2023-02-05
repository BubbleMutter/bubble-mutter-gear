/*
 * @lc app=leetcode id=107 lang=cpp
 *
 * [107] Binary Tree Level Order Traversal II
 *
 * https://leetcode.com/problems/binary-tree-level-order-traversal-ii/description/
 *
 * algorithms
 * Easy (55.03%)
 * Total Accepted:    408.1K
 * Total Submissions: 739.6K
 * Testcase Example:  '[3,9,20,null,null,15,7]'
 *
 * Given the root of a binary tree, return the bottom-up level order traversal
 * of its nodes' values. (i.e., from left to right, level by level from leaf to
 * root).
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: root = [3,9,20,null,null,15,7]
 * Output: [[15,7],[9,20],[3]]
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: root = [1]
 * Output: [[1]]
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: root = []
 * Output: []
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in the tree is in the range [0, 2000].
 * -1000 <= Node.val <= 1000
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
    // 层次遍历; 但是反转层次的结果
    // 经典层次遍历操作; 最后把层次遍历的 vector<vector<int>> 倒转一次即可
    // 参考 00102. Binary Tree Level Order Traversal 题
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> answer;
        if (!root) return answer;

        vector<int> level;
        queue<TreeNode *> curr, next;
        curr.push(root);
        while (!curr.empty()) {
            TreeNode *n = curr.front();
            level.push_back(n->val);

            if (n->left) next.push(n->left);
            if (n->right) next.push(n->right);

            curr.pop();
            if (curr.empty()) {
                swap(curr, next);
                answer.push_back(level);
                level.clear();
            }
        }
        reverse(answer.rbegin(), answer.rend());
        return answer;
    }
};
