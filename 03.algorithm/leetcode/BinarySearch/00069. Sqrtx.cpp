/*
 * @lc app=leetcode id=69 lang=cpp
 *
 * [69] Sqrt(x)
 *
 * https://leetcode.com/problems/sqrtx/description/
 *
 * algorithms
 * Easy (34.95%)
 * Total Accepted:    679.4K
 * Total Submissions: 1.9M
 * Testcase Example:  '4'
 *
 * Given a non-negative integer x, compute and return the square root of x.
 * 
 * Since the return type is an integer, the decimal digits are truncated, and
 * only the integer part of the result is returned.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: x = 4
 * Output: 2
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: x = 8
 * Output: 2
 * Explanation: The square root of 8 is 2.82842..., and since the decimal part
 * is truncated, 2 is returned.
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= x <= 2^31 - 1
 * 
 * 
 */
class Solution {
public:
    int mySqrt(int x) {
        // 二分查找 [0, x/2]
        // 找到 m*m 小于 x 的最大 m 值
        // 这个算法不能处理1这个情况
        if (x == 1) return 1;
        int answer = 0;
        int i = 0, j = x/2;
        while (i <= j) {
            long m = i + (j-i)/2;
            if (m*m == x)
                return m;
            else if (m*m < x) {
                answer = m;
                i = m+1;
            } else
                j = m-1;
        }
        return answer;
    }
};
