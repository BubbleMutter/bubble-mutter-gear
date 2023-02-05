/*
 * @lc app=leetcode id=326 lang=cpp
 *
 * [326] Power of Three
 *
 * https://leetcode.com/problems/power-of-three/description/
 *
 * algorithms
 * Easy (43.17%)
 * Total Accepted:    427.5K
 * Total Submissions: 990.3K
 * Testcase Example:  '27'
 *
 * Given an integer n, return true if it is a power of three. Otherwise, return
 * false.
 * 
 * An integer n is a power of three, if there exists an integer x such that n
 * == 3^x.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 27
 * Output: true
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 0
 * Output: false
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: n = 9
 * Output: true
 * 
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
    bool isPowerOfThree(int n) {
#if 0
        // 不安全实现
        // 对数方法: 求 log3n 的对数是否为整数; log2 精度不够
        return n > 0 && fmod(log10(n)/log10(3), 1) == 0.0;
#endif
        // 任何有限质数的 指数判定法;
        // 必须用 int() 往下取整; to avoid overflow
        static const int limit = int(pow(3, int(log(INT_MAX)/log(3))));
        return n > 0 && limit % n == 0;
    }
};
