/*
 * @lc app=leetcode id=637 lang=cpp
 *
 * [637] Average of Levels in Binary Tree
 *
 * https://leetcode.com/problems/average-of-levels-in-binary-tree/description/
 *
 * algorithms
 * Easy (67.99%)
 * Total Accepted:    246.5K
 * Total Submissions: 362.3K
 * Testcase Example:  '[3,9,20,null,null,15,7]'
 *
 * Given the root of a binary tree, return the average value of the nodes on
 * each level in the form of an array. Answers within 10^-5 of the actual
 * answer will be accepted.
 * 
 * Example 1:
 * 
 * 
 * Input: root = [3,9,20,null,null,15,7]
 * Output: [3.00000,14.50000,11.00000]
 * Explanation: The average value of nodes on level 0 is 3, on level 1 is 14.5,
 * and on level 2 is 11.
 * Hence return [3, 14.5, 11].
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: root = [3,9,20,15,7]
 * Output: [3.00000,14.50000,11.00000]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in the tree is in the range [1, 10^4].
 * -2^31 <= Node.val <= 2^31 - 1
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
    vector<double> averageOfLevels(TreeNode* root) {
        // 层次遍历; 计算每层的节点平均值
        vector<double> res;
        if (!root) return res;
        list<TreeNode *> curr_layer;
        list<TreeNode *> next_layer;
        curr_layer.push_back(root);
        double resOfLayer = 0.0;
        int countOfLayer = 0;
        while (!curr_layer.empty()) {
            TreeNode *node = curr_layer.front();
            curr_layer.pop_front();
            resOfLayer += node->val;
            countOfLayer++;
            if (node->left) next_layer.push_back(node->left);
            if (node->right) next_layer.push_back(node->right);
            if (curr_layer.empty()) {
                res.push_back(resOfLayer / countOfLayer);
                swap(curr_layer, next_layer);
                countOfLayer = 0;
                resOfLayer = 0.0;
            }
        }
        return res;
    }
};
