/*
 * @lc app=leetcode id=993 lang=cpp
 *
 * [993] Cousins in Binary Tree
 *
 * https://leetcode.com/problems/cousins-in-binary-tree/description/
 *
 * algorithms
 * Easy (53.65%)
 * Total Accepted:    196.9K
 * Total Submissions: 366.9K
 * Testcase Example:  '[1,2,3,4]\n4\n3'
 *
 * Given the root of a binary tree with unique values and the values of two
 * different nodes of the tree x and y, return true if the nodes corresponding
 * to the values x and y in the tree are cousins, or false otherwise.
 * 
 * Two nodes of a binary tree are cousins if they have the same depth with
 * different parents.
 * 
 * Note that in a binary tree, the root node is at the depth 0, and children of
 * each depth k node are at the depth k + 1.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: root = [1,2,3,4], x = 4, y = 3
 * Output: false
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: root = [1,2,3,null,4,null,5], x = 5, y = 4
 * Output: true
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: root = [1,2,3,null,4], x = 2, y = 3
 * Output: false
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in the tree is in the range [2, 100].
 * 1 <= Node.val <= 100
 * Each node has a unique value.
 * x != y
 * x and y are exist in the tree.
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
    bool isCousins(TreeNode* root, int x, int y) {
        // 1. 层次遍历求得 x, y 的各自深度;
        //    当深度相同时; 如何判定他们是否有共同的父节点?
        // 2. 缓存3层队列; prev, curr, next,
        // 3. x, y节点先后后被找到后; 判断x, y 的深度是否相等;
        // 4. 如果不等; 则不相等
        // 5. 如果相等; 则遍历prev 层队列; 判断能否找到他们的共同父亲
        list<TreeNode *> prev, curr, next, curr_r;
        int depthOfX = -1, depthOfY = -1;
        int depth = 0;
        if (!root) return false;
        curr.push_back(root);
        while (!curr.empty()) {
            // 当前层节点出列
            TreeNode *node = curr.front();
            curr.pop_front();
            curr_r.push_back(node);

            // 记录找到的深度; x,y都找到了退出循环
            if (node->val == x)
                depthOfX = depth;
            if (node->val == y)
                depthOfY = depth;
            if (depthOfX >= 0 && depthOfY >= 0)
                break;
            // 还没找到就继续层次遍历
            if (node->left) next.push_back(node->left);
            if (node->right) next.push_back(node->right);
            // 继续下一层
            if (curr.empty()) {
                prev.clear();
                swap(prev, curr_r);

                swap(curr, next);
                depth++;
            }
        }
        // 没找到
        if (depthOfX < 0 || depthOfY < 0) return false;
        // 深度不同
        if (depthOfX != depthOfY) return false;
        // 找共同父亲
        while (!prev.empty()) {
            TreeNode * node = prev.front();
            prev.pop_front();
            if (!node->left || !node->right)
                continue; // 单枝父亲不能时共同父亲
            if (node->left->val == x && node->right->val == y)
                return false; // 共同父亲
            if (node->right->val == x && node->left->val == y)
                return false; // 共同父亲
        }
        return true;
    }
};
