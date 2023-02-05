/*
 * @lc app=leetcode id=342 lang=cpp
 *
 * [342] Power of Four
 *
 * https://leetcode.com/problems/power-of-four/description/
 *
 * algorithms
 * Easy (41.68%)
 * Total Accepted:    223K
 * Total Submissions: 534K
 * Testcase Example:  '16'
 *
 * Given an integer n, return true if it is a power of four. Otherwise, return
 * false.
 * 
 * An integer n is a power of four, if there exists an integer x such that n ==
 * 4^x.
 * 
 * 
 * Example 1:
 * Input: n = 16
 * Output: true
 * Example 2:
 * Input: n = 5
 * Output: false
 * Example 3:
 * Input: n = 1
 * Output: true
 * 
 * 
 * Constraints:
 * 
 * 
 * -2^31 <= n <= 2^31 - 1
 * 
 * 
 * 
 * Follow up: Could you solve it without loops/recursion?
 */
class Solution {
public:
    bool isPowerOfFour(int n) {
        // 求 4 的指数
        // 1. 先求 2 的指数; (00231. Power of Two)
        // 2. 再者 4 的指数其剩余地1个1bits 在奇数位置
        //    所以 & 0101 0101 0101 ... 0101 == True
        //
        // Note: 1 是 4 的指数; 1 是0以外其他正数的指数
        return n > 0 && ((n & (n-1)) == 0) && (0x55555555 & n);

#if 0
        // 超级解法
        // 数学原理: 4 的指数 满足两个条件
        // 1. 2 的指数
        // 2. 3 的倍数 + 1
        //
        // 证明第二条:
        // 4^n = 4^(n-1) * 4 = 4^(n-1)*3 + 4^(n-1)
        //     = (4^(n-2)*3 + 4^(n-2))*3 + 4^(n-2)*3 + 4^(n-2)
        //     = 3*3*4^(n-2) + 3*4^(n-2) + 3*4^(n-2) + 4^(n-2)
        //     = 3^2 * 4^(n-2) + 2*3*4^(n-2) + 4^(n-2)
        //     = ...
        //     = 3^m * 4^(n-m) + m*3*4^(n-m) + 4^(n-m)
        // 当 n == m,
        // 4^n = 3^m  + 3*m + 1
        // 一直递推; 直到 右边第二项为 4^(n-n)
        //
        // 实践中; n % 3 的 cpu-alu 耗时 大于 位与操作
        return (n > 0) && (n & (n-1)) == 0 && n % 3 == 1;
#endif
    }
};
