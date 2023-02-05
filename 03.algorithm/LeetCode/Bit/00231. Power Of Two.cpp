/*
 * @lc app=leetcode id=231 lang=cpp
 *
 * [231] Power of Two
 *
 * https://leetcode.com/problems/power-of-two/description/
 *
 * algorithms
 * Easy (43.81%)
 * Total Accepted:    400.7K
 * Total Submissions: 914.5K
 * Testcase Example:  '1'
 *
 * Given an integer n, return true if it is a power of two. Otherwise, return
 * false.
 * 
 * An integer n is a power of two, if there exists an integer x such that n ==
 * 2^x.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 1
 * Output: true
 * Explanation: 2^0 = 1
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 16
 * Output: true
 * Explanation: 2^4 = 16
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: n = 3
 * Output: false
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: n = 4
 * Output: true
 * 
 * 
 * Example 5:
 * 
 * 
 * Input: n = 5
 * Output: false
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
#if 0
    bool isPowerOfTwo(int n) {
        if (n == 0) return false;
        // 定律1: x & (-x) 求得最右边的1
        // 因为负数的表示法 x == ~x + 1
        // x取反后 +1; 会往前进位; 直到变成x右边第一个1
        // 所以，如果 x 有且只有一个1, 那么 x&(-x) == x
        long x = n;
        return (x & (-x)) == x;
    }
#endif
    bool isPowerOfTwo(int n) {
        // 1. 大于0
        // 2. 干掉1个1 后还是0
        return n > 0 && (n & (n-1)) == 0;
    }
};
