/*
 * @lc app=leetcode id=1582 lang=cpp
 *
 * [1582] Special Positions in a Binary Matrix
 *
 * https://leetcode.com/problems/special-positions-in-a-binary-matrix/description/
 *
 * algorithms
 * Easy (64.04%)
 * Total Accepted:    18.3K
 * Total Submissions: 28.5K
 * Testcase Example:  '[[1,0,0],[0,0,1],[1,0,0]]'
 *
 * Given a rows x cols matrix mat, where mat[i][j] is either 0 or 1, return the
 * number of special positions in mat.
 * 
 * A position (i,j) is called special if mat[i][j] == 1 and all other elements
 * in row i and column j are 0 (rows and columns are 0-indexed).
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: mat = [[1,0,0],
 * [0,0,1],
 * [1,0,0]]
 * Output: 1
 * Explanation: (1,2) is a special position because mat[1][2] == 1 and all
 * other elements in row 1 and column 2 are 0.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: mat = [[1,0,0],
 * [0,1,0],
 * [0,0,1]]
 * Output: 3
 * Explanation: (0,0), (1,1) and (2,2) are special positions. 
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: mat = [[0,0,0,1],
 * [1,0,0,0],
 * [0,1,1,0],
 * [0,0,0,0]]
 * Output: 2
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: mat = [[0,0,0,0,0],
 * [1,0,0,0,0],
 * [0,1,0,0,0],
 * [0,0,1,0,0],
 * [0,0,0,1,1]]
 * Output: 3
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * rows == mat.length
 * cols == mat[i].length
 * 1 <= rows, cols <= 100
 * mat[i][j] is 0 or 1.
 * 
 * 
 */
// #include "prettyprint.hpp"

class Solution {
public:
    int numSpecial(vector<vector<int>>& mat) {
        // 1. 遍历一次矩阵
        // 2. 记录只有1个1的行 所在的列
        // 3. 记录每列的和
        // 4. 遍历 2 步骤; 中只有1个1的行
        //    查询这行出现1的列的和 (3. 步骤中计算)
        //    是否等于 1
        vector<int> matchRow(mat.size(), -1); 
        vector<int> sumOfCol(mat[0].size(), 0);

        for (int i = 0; i < mat.size(); i++) {
            int sum = 0, col = -1;
            for (int j = 0; j < mat[0].size(); j++) {
                sumOfCol[j] += mat[i][j];
                sum += mat[i][j]; 
                if (mat[i][j]) col = j; // 记录本行最后出现1的列
            }
            cout << sum << endl;
            // 满足条件的行, 出现1的位置
            if (sum == 1)
                matchRow[i] = col;
        }
        int answer = 0;
        for (int i = 0; i < matchRow.size(); ++i)
            num += (matchRow[i] >= 0) && (sumOfCol[matchRow[i]] == 1);
        return answer;
    }
};
