/*
 * @lc app=leetcode id=1886 lang=cpp
 *
 * [1886] Determine Whether Matrix Can Be Obtained By Rotation
 *
 * https://leetcode.com/problems/determine-whether-matrix-can-be-obtained-by-rotation/description/
 *
 * algorithms
 * Easy (54.66%)
 * Total Accepted:    24.3K
 * Total Submissions: 44.3K
 * Testcase Example:  '[[0,1],[1,0]]\n[[1,0],[0,1]]'
 *
 * Given two n x n binary matrices mat and target, return true if it is
 * possible to make mat equal to target by rotating mat in 90-degree
 * increments, or false otherwise.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: mat = [[0,1],[1,0]], target = [[1,0],[0,1]]
 * Output: true
 * Explanation: We can rotate mat 90 degrees clockwise to make mat equal
 * target.
 * 
 * 
 * Example 2:
 *
 *
 * Input: mat = [[0,1],[1,1]], target = [[1,0],[0,1]]
 * Output: false
 * Explanation: It is impossible to make mat equal to target by rotating mat.
 *
 *
 * Example 3:
 *
 *
 * Input: mat = [[0,0,0],[0,1,0],[1,1,1]], target = [[1,1,1],[0,1,0],[0,0,0]]
 * Output: true
 * Explanation: We can rotate mat 90 degrees clockwise two times to make mat
 * equal target.
 *
 *
 *
 * Constraints:
 *
 *
 * n == mat.length == target.length
 * n == mat[i].length == target[i].length
 * 1 <= n <= 10
 * mat[i][j] and target[i][j] are either 0 or 1.
 *
 *
 */
// #include <prettyprint.hpp>
class Solution {
public:
    bool findRotation(vector<vector<int>>& mat, vector<vector<int>>& target) {
        // 不旋转
        if (mat == target) return true;

        /* 方形矩阵旋转操作 */
        vector<vector<int>> tmp(mat.size(), vector<int>(mat[0].size(), 0));
        /* 旋转3次 */
        for (int rotate = 0; rotate < 3; ++rotate) {
            for (size_t i = 0; i < mat.size(); ++i) {
                for (size_t j = 0; j < mat[i].size(); ++j) {
                    tmp[j /* 列 -> 行 */ ][mat.size()-1-i /* 行的补数 -> 列 */] = mat[i][j];
                }
            }
            if (tmp == target) return true;
            mat = tmp;
        }
        return false;
    }
};
