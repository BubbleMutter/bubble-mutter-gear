/*
 * @lc app=leetcode id=1323 lang=cpp
 *
 * [1323] Maximum 69 Number
 *
 * https://leetcode.com/problems/maximum-69-number/description/
 *
 * algorithms
 * Easy (78.64%)
 * Total Accepted:    105.7K
 * Total Submissions: 134.2K
 * Testcase Example:  '9669'
 *
 * You are given a positive integer num consisting only of digits 6 and 9.
 * 
 * Return the maximum number you can get by changing at most one digit (6
 * becomes 9, and 9 becomes 6).
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: num = 9669
 * Output: 9969
 * Explanation: 
 * Changing the first digit results in 6669.
 * Changing the second digit results in 9969.
 * Changing the third digit results in 9699.
 * Changing the fourth digit results in 9666.
 * The maximum number is 9969.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: num = 9996
 * Output: 9999
 * Explanation: Changing the last digit 6 to 9 results in the maximum number.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: num = 9999
 * Output: 9999
 * Explanation: It is better not to apply any change.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= num <= 10^4
 * num consists of only 6 and 9 digits.
 * 
 * 
 */
class Solution {
public:
    int maximum69Number (int num) {
        // 参考: 01317. Convert Integer To The Sum Of Two No Zero Integers.cpp
        // 找到最高十进制位的方法
        //
        // 1. 找到最高位 6, 记录 pow 位数
        // 2. num + 3*pow 数, 即把 6 变 9
        int val = num, pow = 0, div = 1;
        while (val) {
            if (val % 10 == 6) {
                pow = div;
            }
            val /= 10;
            div *= 10;
        }
        return num + 3*pow;
    }
};
