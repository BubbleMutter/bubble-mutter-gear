/*
 * @lc app=leetcode id=507 lang=cpp
 *
 * [507] Perfect Number
 *
 * https://leetcode.com/problems/perfect-number/description/
 *
 * algorithms
 * Easy (37.46%)
 * Total Accepted:    96.7K
 * Total Submissions: 258K
 * Testcase Example:  '28'
 *
 * A perfect number is a positive integer that is equal to the sum of its
 * positive divisors, excluding the number itself. A divisor of an integer x is
 * an integer that can divide x evenly.
 *
 * Given an integer n, return true if n is a perfect number, otherwise return
 * false.
 *
 *
 * Example 1:
 *
 *
 * Input: num = 28
 * Output: true
 * Explanation: 28 = 1 + 2 + 4 + 7 + 14
 * 1, 2, 4, 7, and 14 are all divisors of 28.
 *
 *
 * Example 2:
 *
 *
 * Input: num = 7
 * Output: false
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= num <= 10^8
 *
 *
 */
// #include <prettyprint.hpp>
class Solution {
public:
    bool checkPerfectNumber(int num) {
#if 0 /* 版本1 */
        int val = 0;
        for (int i = 1; i <= num/2; ++i) {
            if (num % i == 0) {
                val += i;
            }
        }
        return num == val;
#endif
#if 1 /* 优化 */
        int val = 1;
        /**
         * 1. i 需要从2 开始; 因为 num % 1 == 0; 但 num 本身不参与加和
         * 2. 但 val 需要有 1 参与加和, 所以 val 从 1 开始
         * 3. num == 1 本身不符合要求, 悖论
         * 4. 因为 遍历 [2, sqrt(num)] 范围的数
         *    所以 需要把 num/i 捞回来
         */
        for (int i=2; i<=sqrt(num); i++) {
            if (num % i == 0) {
                val += i + (i == (num/i) ? 0 : num/i);
            }
        }
        return num == val && num != 1;
#endif
    }
};
