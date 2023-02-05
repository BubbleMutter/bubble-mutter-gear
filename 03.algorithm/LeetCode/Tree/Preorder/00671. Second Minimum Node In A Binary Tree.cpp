/*
 * @lc app=leetcode id=671 lang=cpp
 *
 * [671] Second Minimum Node In a Binary Tree
 *
 * https://leetcode.com/problems/second-minimum-node-in-a-binary-tree/description/
 *
 * algorithms
 * Easy (43.37%)
 * Total Accepted:    128.3K
 * Total Submissions: 295.6K
 * Testcase Example:  '[2,2,5,null,null,5,7]'
 *
 * Given a non-empty special binary tree consisting of nodes with the
 * non-negative value, where each node in this tree has exactly two or zero
 * sub-node. If the node has two sub-nodes, then this node's value is the
 * smaller value among its two sub-nodes. More formally, the property root.val
 * = min(root.left.val, root.right.val) always holds.
 * 
 * Given such a binary tree, you need to output the second minimum value in the
 * set made of all the nodes' value in the whole tree.
 * 
 * If no such second minimum value exists, output -1 instead.
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: root = [2,2,5,null,null,5,7]
 * Output: 5
 * Explanation: The smallest value is 2, the second smallest value is 5.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: root = [2,2,2]
 * Output: -1
 * Explanation: The smallest value is 2, but there isn't any second smallest
 * value.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in the tree is in the range [1, 25].
 * 1 <= Node.val <= 2^31 - 1
 * root.val == min(root.left.val, root.right.val) for each internal node of the
 * tree.
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
// 这一题有一个陷阱: 就是 INT_MAX 可以作为节点成员出现
// 如果简单地初始化 candicate(2, INT_MAX); 然后在遍历时把节点值插入到合适位置 会出错
// 所以: 正确的做法时 初始化 candicate 空数组; 在遍历过程中增加到尺寸2
//       然后在遍历时; 当发现candicate已经有两个成员时; 再把当前节点插入合适位置
public:
    int findSecondMinimumValue(TreeNode* root) {
        // 不能直接地使用 candicate(2, INT_MAX);
        // 因为 INT_MAX 可以作为树木的成员出现
        vector<int> res;
        findSecondMinimumValue(root, res);
        if (res.size() == 2) {
            return res[1];
        }
        return -1;
    }
private:
    void findSecondMinimumValue(TreeNode* root, vector<int>& res) {
        // 1. 前序遍历 (实际上任意遍历即可)
        //    遍历过程中, 维护最小的两个元素
        // 2. 候选者数组即遍历过程中最小的两个值(不相同的)
        // 2.1 候选者为空; 插入节点值即可
        // 2.2 候选者只有一个; 插入节点值后调整大小位置
        // 2.3 候选者已经有两个; 根据节点值插入到数组的合适位置
        if (!root) return;
        if (res.empty()) {
            res.push_back(root->val);
        } else if (res.size() == 1) {
            if (root->val > res[0]) {
                res.push_back(root->val);
            } else if (root->val < res[0]) {
                res.push_back(res[0]);
                res[0] = root->val;
            }
        } else if (res.size() == 2) {
            if (root->val < res[0]) {
                res.push_back(res[0]);
                res[0] = root->val;
            } else if (res[0] < root->val && root->val < res[1]) {
                res[1] = root->val;
            }
        }
        findSecondMinimumValue(root->left, res);
        findSecondMinimumValue(root->right, res);
    }
};
