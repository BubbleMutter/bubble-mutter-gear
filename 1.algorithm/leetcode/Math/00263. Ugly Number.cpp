/*
 * @lc app=leetcode id=263 lang=cpp
 *
 * [263] Ugly Number
 *
 * https://leetcode.com/problems/ugly-number/description/
 *
 * algorithms
 * Easy (41.71%)
 * Total Accepted:    232K
 * Total Submissions: 556.2K
 * Testcase Example:  '6'
 *
 * Write a program to check whether a given number is an ugly number.
 * 
 * Ugly numbers are positive numbers whose prime factors only include 2, 3, 5.
 * 
 * Example 1:
 * 
 * 
 * Input: 6
 * Output: true
 * Explanation: 6 = 2 × 3
 * 
 * Example 2:
 * 
 * 
 * Input: 8
 * Output: true
 * Explanation: 8 = 2 × 2 × 2
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: 14
 * Output: false 
 * Explanation: 14 is not ugly since it includes another prime factor 7.
 * 
 * 
 * Note:
 * 
 * 
 * 1 is typically treated as an ugly number.
 * Input is within the 32-bit signed integer range: [−2^31,  2^31 − 1].
 * 
 */
class Solution {
public:
    bool isUgly(int num) {
#if 0 // 自己的解法
        if (num == 0) return false;
        if (num == 1) return true;
        bool p2 = false, p3 = false, p5 = false;
        p2 = num % 2 == 0;
        p3 = num % 3 == 0;
        p5 = num % 5 == 0;
        return (p2 && isUgly(num/2)) ||
               (p3 && isUgly(num/3)) ||
               (p5 && isUgly(num/5));
#endif
        // 题解
        // 遍历 2,3,4,5,
        // 尽可能地让 num 整除 factor
        // 得到最后的num 就是无法 被 2/3/4/5 整除的
        // 判断最后剩下的 num 是否是 1
        // Note: 满足题意的数值一定是  (2*2*..*2) * (3*3*3*..*3) * (5*5*..*5)
        if (!num) return false;
        for (int i = 2; i <= 5; i++)
            while (num % i == 0)
                num /= i;
        return num == 1;
    }
};
