/*
 * @lc app=leetcode id=653 lang=cpp
 *
 * [653] Two Sum IV - Input is a BST
 *
 * https://leetcode.com/problems/two-sum-iv-input-is-a-bst/description/
 *
 * algorithms
 * Easy (58.42%)
 * Total Accepted:    280.6K
 * Total Submissions: 480K
 * Testcase Example:  '[5,3,6,2,4,null,7]\n9'
 *
 * Given the root of a Binary Search Tree and a target number k, return true if
 * there exist two elements in the BST such that their sum is equal to the
 * given target.
 *
 *
 * Example 1:
 *
 *
 * Input: root = [5,3,6,2,4,null,7], k = 9
 * Output: true
 *
 *
 * Example 2:
 *
 *
 * Input: root = [5,3,6,2,4,null,7], k = 28
 * Output: false
 *
 *
 *
 * Constraints:
 *
 *
 * The number of nodes in the tree is in the range [1, 10^4].
 * -10^4 <= Node.val <= 10^4
 * root is guaranteed to be a valid binary search tree.
 * -10^5 <= k <= 10^5
 *
 *
 */
class Solution {
public:
    void findTargetEx(TreeNode* root, int k, map<int, bool> &visited, bool &res) {
        // BST 树找到两节点的和 为给定的数
        // 能找到返回true
        // 暴力求解; 中序遍历; 用hash缓存每一个已访问元素的值
        if (res) return; // 提前结束
        if (!root) return;
        findTargetEx(root->left, k, visited, res);
        if (visited.find(k - root->val) != visited.end()) {
            res = true;
            return;
        }
        visited[root->val] = true;
        findTargetEx(root->right, k, visited, res);
    }
    bool findTarget(TreeNode* root, int k) {
        bool res = false;
        map<int, bool> visited;
        findTargetEx(root, k, visited, res);
        return res;
    }
};
