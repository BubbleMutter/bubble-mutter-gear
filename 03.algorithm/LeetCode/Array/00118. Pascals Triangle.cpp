/*
 * @lc app=leetcode id=118 lang=cpp
 *
 * [118] Pascal's Triangle
 *
 * https://leetcode.com/problems/pascals-triangle/description/
 *
 * algorithms
 * Easy (54.59%)
 * Total Accepted:    462.6K
 * Total Submissions: 843.8K
 * Testcase Example:  '5'
 *
 * Given an integer numRows, return the first numRows of Pascal's triangle.
 * 
 * In Pascal's triangle, each number is the sum of the two numbers directly
 * above it as shown:
 * 
 * 
 * Example 1:
 * Input: numRows = 5
 * Output: [[1],[1,1],[1,2,1],[1,3,3,1],[1,4,6,4,1]]
 * Example 2:
 * Input: numRows = 1
 * Output: [[1]]
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= numRows <= 30
 * 
 * 
 */
class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        // 1. 第一行为 [1]
        // 2. 中间行 首尾元素为1
        // 3. 中间元素对称
        // 4. 左边元素 == 上一行同位元素 + 上一行前一元素
        // 5. 奇数行的中间元素需要 额外补充 (因为对称遍历; 遍历不到中间元素)
        vector<vector<int>> answer;
        answer.push_back(vector<int>(1, 1));
        for (int i = 2; i <= numRows; i++) {
            vector<int> row(i);
            *row.begin() = *row.rbegin() = 1;
            for (int j = 1; j < i/2; j++)
                row[j] = row[i-j-1] = answer.back()[j-1] + answer.back()[j];
            if (row.size() & 1) {
                row[row.size() / 2] = answer.back()[row.size()/2-1] + answer.back()[row.size()/2];
            }
            answer.push_back(row);
        }
        return answer;
    }
};
