/*
 * @lc app=leetcode id=1252 lang=cpp
 *
 * [1252] Cells with Odd Values in a Matrix
 *
 * https://leetcode.com/problems/cells-with-odd-values-in-a-matrix/description/
 *
 * algorithms
 * Easy (78.51%)
 * Total Accepted:    75.4K
 * Total Submissions: 96K
 * Testcase Example:  '2\n3\n[[0,1],[1,1]]'
 *
 * There is an m x n matrix that is initialized to all 0's. There is also a 2D
 * array indices where each indices[i] = [ri, ci] represents a 0-indexed
 * location to perform some increment operations on the matrix.
 *
 * For each location indices[i], do both of the following:
 *
 *
 * Increment all the cells on row ri.
 * Increment all the cells on column ci.
 *
 *
 * Given m, n, and indices, return the number of odd-valued cells in the matrix
 * after applying the increment to all locations in indices.
 *
 *
 * Example 1:
 *
 *
 * Input: m = 2, n = 3, indices = [[0,1],[1,1]]
 * Output: 6
 * Explanation: Initial matrix = [[0,0,0],[0,0,0]].
 * After applying first increment it becomes [[1,2,1],[0,1,0]].
 * The final matrix is [[1,3,1],[1,3,1]], which contains 6 odd numbers.
 *
 *
 * Example 2:
 *
 *
 * Input: m = 2, n = 2, indices = [[1,1],[0,0]]
 * Output: 0
 * Explanation: Final matrix = [[2,2],[2,2]]. There are no odd numbers in the
 * final matrix.
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= m, n <= 50
 * 1 <= indices.length <= 100
 * 0 <= ri < m
 * 0 <= ci < n
 *
 *
 *
 * Follow up: Could you solve this in O(n + m + indices.length) time with only
 * O(n + m) extra space?
 *
 */
class Solution {
public:
    int oddCells(int m, int n, vector<vector<int>>& indices) {
        /* cell 为奇数; 即对应的 行+列 incre 总共奇数次
         *
         * 1. 计算行的 incre 次数
         * 2. 计数列的 incre 次数
         * 3. incre奇数次的行 * incre 偶数次的列
         * 4. incre奇数次的列 * incre 偶数次的行
         */
        vector<int> row(m, 0), col(n, 0);
        for (const auto& indice: indices) {
            row[indice[0]]++;
            col[indice[1]]++;
        }
        auto is_odd = [](const int n) { return n & 0x1; };
        int odd_row = count_if(row.begin(), row.end(), is_odd);
        int odd_col = count_if(col.begin(), col.end(), is_odd);
        return odd_row*(col.size()-odd_col) + (row.size()-odd_row)*odd_col;
    }
};
