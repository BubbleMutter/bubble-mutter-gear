/*
 * @lc app=leetcode id=872 lang=cpp
 *
 * [872] Leaf-Similar Trees
 *
 * https://leetcode.com/problems/leaf-similar-trees/description/
 *
 * algorithms
 * Easy (64.82%)
 * Total Accepted:    154.9K
 * Total Submissions: 238.9K
 * Testcase Example:  '[3,5,1,6,2,9,8,null,null,7,4]\n[3,5,1,6,7,4,2,null,null,null,null,null,null,9,8]'
 *
 * Consider all the leaves of a binary tree, from left to right order, the
 * values of those leaves form a leaf value sequence.
 * 
 * 
 * 
 * For example, in the given tree above, the leaf value sequence is (6, 7, 4,
 * 9, 8).
 * 
 * Two binary trees are considered leaf-similar if their leaf value sequence is
 * the same.
 * 
 * Return true if and only if the two given trees with head nodes root1 and
 * root2 are leaf-similar.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: root1 = [3,5,1,6,2,9,8,null,null,7,4], root2 =
 * [3,5,1,6,7,4,2,null,null,null,null,null,null,9,8]
 * Output: true
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: root1 = [1,2,3], root2 = [1,3,2]
 * Output: false
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in each tree will be in the range [1, 200].
 * Both of the given trees will have values in the range [0, 200].
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
// 后序遍历得到叶序列
// 方法1: 分别用计算两个树的叶序列; 再判断两个树的叶序列是否相等
class Solution {
public:
    void leavesCaches(TreeNode* root, vector<int> &res) {
        if (!root) return;
        if (!root->left && !root->right) {
            res.push_back(root->val);
            return;
        }
        leavesCaches(root->left, res);
        leavesCaches(root->right, res);
    }
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> res1, res2;
        leavesCaches(root1, res1);
        leavesCaches(root2, res2);
        return res1.size() == res2.size() && res1 == res2;
    }
};

#if 0
// 方法2: 先后序遍历(左右根)缓存一个树的叶序列;
//        再"反后序遍历(右左根)"另一个树; 把相等 叶节点相同的 res.back() 销毁调
// 如果 res 最后被清空; 说明两树的叶节点序列相同
class Solution {
public:
    void leavesPush(TreeNode* root, vector<int> &leaves) {
        if (!root) return;
        leavesPush(root->left, leaves);
        leavesPush(root->right, leaves);
        if (!root->left && !root->right) {
            leaves.push_back(root->val);
            return;
        }
    }
    void leavesPop(TreeNode* root, vector<int> &leaves, bool & res) {
        if (!root) return;
        if (!res) return;
        leavesPop(root->right, leaves, res);
        leavesPop(root->left, leaves, res);
        if (!root->left && !root->right) {
            if (root->val == leaves[leaves.size()-1]) {
                leaves.pop_back();
            } else {
                res = false;
            }
            return;
        }
    }
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> leaves;
        bool res = true;
        leavesPush(root1, leaves);
        leavesPop(root2, leaves, res);
        return leaves.empty() ? true : false;
    }
};
#endif
