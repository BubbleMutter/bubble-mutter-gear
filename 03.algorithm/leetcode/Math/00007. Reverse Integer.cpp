/*
 * @lc app=leetcode id=7 lang=cpp
 *
 * [7] Reverse Integer
 *
 * https://leetcode.com/problems/reverse-integer/description/
 *
 * algorithms
 * Easy (25.89%)
 * Total Accepted:    1.4M
 * Total Submissions: 5.4M
 * Testcase Example:  '123'
 *
 * Given a signed 32-bit integer x, return x with its digits reversed. If
 * reversing x causes the value to go outside the signed 32-bit integer range
 * [-2^31, 2^31 - 1], then return 0.
 * 
 * Assume the environment does not allow you to store 64-bit integers (signed
 * or unsigned).
 * 
 * 
 * Example 1:
 * Input: x = 123
 * Output: 321
 * Example 2:
 * Input: x = -123
 * Output: -321
 * Example 3:
 * Input: x = 120
 * Output: 21
 * Example 4:
 * Input: x = 0
 * Output: 0
 * 
 * 
 * Constraints:
 * 
 * 
 * -2^31 <= x <= 2^31 - 1
 * 
 * 
 */
class Solution {
public:
    int reverse(int x) {
        long answer = 0;
        while (x != 0) {
            // 这个做法不需要考虑正负数问题
            // 负数 取模 还是负数
            answer = answer*10 + x % 10;
            x /= 10;
        }
        if (answer >= numeric_limits<int>::max() ||
            answer <= numeric_limits<int>::min()) {
            answer = 0;
        }
        return answer;
    }
};
