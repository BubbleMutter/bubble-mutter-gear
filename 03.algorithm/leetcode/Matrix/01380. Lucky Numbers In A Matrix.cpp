/*
 * @lc app=leetcode id=1380 lang=cpp
 *
 * [1380] Lucky Numbers in a Matrix
 *
 * https://leetcode.com/problems/lucky-numbers-in-a-matrix/description/
 *
 * algorithms
 * Easy (70.90%)
 * Total Accepted:    59.2K
 * Total Submissions: 83.5K
 * Testcase Example:  '[[3,7,8],[9,11,13],[15,16,17]]'
 *
 * Given an m x n matrix of distinct numbers, return all lucky numbers in the
 * matrix in any order.
 * 
 * A lucky number is an element of the matrix such that it is the minimum
 * element in its row and maximum in its column.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: matrix = [[3,7,8],[9,11,13],[15,16,17]]
 * Output: [15]
 * Explanation: 15 is the only lucky number since it is the minimum in its row
 * and the maximum in its column.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: matrix = [[1,10,4,2],[9,3,8,7],[15,16,17,12]]
 * Output: [12]
 * Explanation: 12 is the only lucky number since it is the minimum in its row
 * and the maximum in its column.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: matrix = [[7,8],[1,2]]
 * Output: [7]
 * Explanation: 7 is the only lucky number since it is the minimum in its row
 * and the maximum in its column.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * m == mat.length
 * n == mat[i].length
 * 1 <= n, m <= 50
 * 1 <= matrix[i][j] <= 10^5.
 * All elements in the matrix are distinct.
 * 
 * 
 */
class Solution {
public:
    vector<int> luckyNumbers (vector<vector<int>>& matrix) {
        vector<int> col_max(matrix[0].size(), 0);
        // 遍历每列
        for (size_t i = 0; i < matrix[0].size(); ++i) {
            // 求出每列的最大值
            col_max[i] = matrix[0][i];
            for (size_t j = 1; j < matrix.size(); ++j) {
                col_max[i] = max(col_max[i], matrix[j][i]);
            }
        }
        vector<int> res;
        // 遍历每行
        for (size_t i = 0; i < matrix.size(); ++i) {
            // 求出每行的最小值 所在的列
            auto row_min_iter = min_element(matrix[i].begin(), matrix[i].end());
            // 最小值所在列的最大值 == 最小值本身
            if (*row_min_iter == col_max[distance(matrix[i].begin(), row_min_iter)]) {
                res.push_back(*row_min_iter);
            }
        }
        return res;
    }
};
