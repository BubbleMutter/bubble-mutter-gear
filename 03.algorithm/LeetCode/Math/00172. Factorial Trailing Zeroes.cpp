/*
 * @lc app=leetcode id=172 lang=cpp
 *
 * [172] Factorial Trailing Zeroes
 *
 * https://leetcode.com/problems/factorial-trailing-zeroes/description/
 *
 * algorithms
 * Easy (38.47%)
 * Total Accepted:    246.6K
 * Total Submissions: 639.2K
 * Testcase Example:  '3'
 *
 * Given an integer n, return the number of trailing zeroes in n!.
 * 
 * Follow up: Could you write a solution that works in logarithmic time
 * complexity?
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 3
 * Output: 0
 * Explanation: 3! = 6, no trailing zero.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 5
 * Output: 1
 * Explanation: 5! = 120, one trailing zero.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: n = 0
 * Output: 0
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= n <= 10^4
 * 
 * 
 */

// 1. !n 中 n/5 个数值能 整除5
//    ta们分别是 5, 10, 15, 20, k*5
//    这些 5的倍数才能提供末尾的0
//
// 2. 这些数字提供末尾的0的个数 等于
//    最小因式分解中 5 个数
//
// 3. 所以 能整除 5 的提供了1个0
//         能整除 5**2 的再提供1个0
//         能整除 5**3 的再再提供1个0
//         如此类推
// 4. 复杂度是 log5(n) 符合题意

class Solution {
public:
    int trailingZeroes(int n) {
        int answer = 0;
        for (int i = 5; i <= n; i *= 5)
            answer += n / i;
        return answer;
    }
};
