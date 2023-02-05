/*
 * @lc app=leetcode id=1572 lang=cpp
 *
 * [1572] Matrix Diagonal Sum
 *
 * https://leetcode.com/problems/matrix-diagonal-sum/description/
 *
 * algorithms
 * Easy (78.13%)
 * Total Accepted:    38.3K
 * Total Submissions: 49K
 * Testcase Example:  '[[1,2,3],[4,5,6],[7,8,9]]'
 *
 * Given a square matrix mat, return the sum of the matrix diagonals.
 * 
 * Only include the sum of all the elements on the primary diagonal and all the
 * elements on the secondary diagonal that are not part of the primary
 * diagonal.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: mat = [[1,2,3],
 * [4,5,6],
 * [7,8,9]]
 * Output: 25
 * Explanation: Diagonals sum: 1 + 5 + 9 + 3 + 7 = 25
 * Notice that element mat[1][1] = 5 is counted only once.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: mat = [[1,1,1,1],
 * [1,1,1,1],
 * [1,1,1,1],
 * [1,1,1,1]]
 * Output: 8
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: mat = [[5]]
 * Output: 5
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * n == mat.length == mat[i].length
 * 1 <= n <= 100
 * 1 <= mat[i][j] <= 100
 * 
 * 
 */
class Solution {
public:
    int diagonalSum(vector<vector<int>>& mat) {
        // 1. 遍历左上角到右下角 和 "右上角到左下角"
        //    使用 trick 一次遍历完成
        // 2. 如果边长是奇数; 那么干掉中间那个数一次
        int sum = 0;
        for (int i = 0; i < mat.size(); ++i)
            sum += mat[i][i] + mat[mat.size()-i-1][i];
        return mat.size() & 0x1 ? sum - mat[(mat.size()-1)/2][(mat.size()-1)/2] : sum;
    }
};

// 方法2: 每行左右法
class Solution {
public:
    int diagonalSum(vector<vector<int>>& mat) {
        int sum = 0;
        for (int i = 0; i < mat.size(); ++i) {
            int left = i, right = mat[0].size()-1-i;
            sum += mat[i][left];
            if (left != right)
                sum += mat[i][right];
        }
        return sum;
    }
};
