/*
 * @lc app=leetcode id=119 lang=cpp
 *
 * [119] Pascal's Triangle II
 *
 * https://leetcode.com/problems/pascals-triangle-ii/description/
 *
 * algorithms
 * Easy (52.01%)
 * Total Accepted:    356.4K
 * Total Submissions: 683.2K
 * Testcase Example:  '3'
 *
 * Given an integer rowIndex, return the rowIndex^th (0-indexed) row of the
 * Pascal's triangle.
 * 
 * In Pascal's triangle, each number is the sum of the two numbers directly
 * above it as shown:
 * 
 * 
 * Example 1:
 * Input: rowIndex = 3
 * Output: [1,3,3,1]
 * Example 2:
 * Input: rowIndex = 0
 * Output: [1]
 * Example 3:
 * Input: rowIndex = 1
 * Output: [1,1]
 *
 *
 * Constraints:
 *
 *
 * 0 <= rowIndex <= 33
 *
 *
 *
 * Follow up: Could you optimize your algorithm to use only O(rowIndex) extra
 * space?
 *
 */
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        // 帕斯卡三角形 本质:
        // n+1 行   m 个元素值就是 组合公式  n! / (n-m)!m!
        // n+1 行 m-1 个元素值由此可得       n! / (n-(m-1))!(m-1)! = n! / (n-m+1)!(m-1)!
        //
        // 所以 第m元素 = 第m-1元素 *  ((n-m+1)/m)
        //
        // 然后根据对称性质用 O(n) 得出结果
        vector<int> answer(rowIndex+1, 1);
        for (int i = 1; i <= (rowIndex+1)/2; i++) {
            answer[i] = answer[rowIndex-i] = (long)answer[i-1]*(rowIndex-i+1)/i;
        }
        return answer;
    }
};
