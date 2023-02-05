/*
 * @lc app=leetcode id=1351 lang=cpp
 *
 * [1351] Count Negative Numbers in a Sorted Matrix
 *
 * https://leetcode.com/problems/count-negative-numbers-in-a-sorted-matrix/description/
 *
 * algorithms
 * Easy (75.20%)
 * Total Accepted:    154.4K
 * Total Submissions: 205.2K
 * Testcase Example:  '[[4,3,2,-1],[3,2,1,-1],[1,1,-1,-2],[-1,-1,-2,-3]]'
 *
 * Given a m x n matrix grid which is sorted in non-increasing order both
 * row-wise and column-wise, return the number of negative numbers in grid.
 *
 *
 * Example 1:
 *
 *
 * Input: grid = [[4,3,2,-1],[3,2,1,-1],[1,1,-1,-2],[-1,-1,-2,-3]]
 * Output: 8
 * Explanation: There are 8 negatives number in the matrix.
 *
 *
 * Example 2:
 *
 *
 * Input: grid = [[3,2],[1,0]]
 * Output: 0
 *
 *
 *
 * Constraints:
 *
 *
 * m == grid.length
 * n == grid[i].length
 * 1 <= m, n <= 100
 * -100 <= grid[i][j] <= 100
 *
 *
 *
 * Follow up: Could you find an O(n + m) solution?
 */

// 在降序数组中 找到第一个非负数的索引, 即可求得负数的数量
// 初始值设置为 grid[0].size() 因为刚开始没找到
class Solution {
 public:
  int countNegatives(vector<vector<int>>& grid) {
    int index = grid[0].size() - 1;
    int count = 0;
    for (size_t i = 0; i < grid.size(); ++i) {
      while (index >= 0) {
        // 找到该行右往左数 第一个非负数的索引
        if (grid[i][index] >= 0)
          break;
        else
          index--;
      }
      count += (grid[i].size() - 1) - index;
    }
    return count;
  }
};
