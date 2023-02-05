/*
 * @lc app=leetcode id=501 lang=cpp
 *
 * [501] Find Mode in Binary Search Tree
 *
 * https://leetcode.com/problems/find-mode-in-binary-search-tree/description/
 *
 * algorithms
 * Easy (46.45%)
 * Total Accepted:    139.3K
 * Total Submissions: 299.8K
 * Testcase Example:  '[1,null,2,2]'
 *
 * Given the root of a binary search tree (BST) with duplicates, return all the
 * mode(s) (i.e., the most frequently occurred element) in it.
 * 
 * If the tree has more than one mode, return them in any order.
 * 
 * Assume a BST is defined as follows:
 * 
 * 
 * The left subtree of a node contains only nodes with keys less than or equal
 * to the node's key.
 * The right subtree of a node contains only nodes with keys greater than or
 * equal to the node's key.
 * Both the left and right subtrees must also be binary search trees.
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: root = [1,null,2,2]
 * Output: [2]
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: root = [0]
 * Output: [0]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in the tree is in the range [1, 10^4].
 * -10^5 <= Node.val <= 10^5
 * 
 * 
 * 
 * Follow up: Could you do that without using any extra space? (Assume that the
 * implicit stack space incurred due to recursion does not count).
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
    vector<int> findMode(TreeNode* root) {
        size_t count = 0;
        int maxfreq = 0;
        TreeNode *prev = nullptr;
        vector<int> result;
        findMode(root, &prev, count, maxfreq, result);
        return result;
    }
private:
    void findMode(TreeNode *root, TreeNode **prev, size_t &count, int &maxfreq, vector<int>& result) {
        // 空节点退出
        if (!root) return;
        // inorder traversal
        findMode(root->left, prev, count, maxfreq, result);
        if (*prev && (*prev)->val == root->val) {
            // 连续相同
            count++;
        } else {
            (*prev) = root;
            count = 1;
        }

        if (count == maxfreq) {
            result.push_back(root->val);
        } else if (count > maxfreq) {
            maxfreq = count;
            result.resize(1);
            result[0] = root->val;
        }
        findMode(root->right, prev, count, maxfreq, result);
    }
};
