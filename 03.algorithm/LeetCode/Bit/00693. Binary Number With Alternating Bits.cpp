/*
 * @lc app=leetcode id=693 lang=cpp
 *
 * [693] Binary Number with Alternating Bits
 *
 * https://leetcode.com/problems/binary-number-with-alternating-bits/description/
 *
 * algorithms
 * Easy (60.62%)
 * Total Accepted:    87.8K
 * Total Submissions: 144.8K
 * Testcase Example:  '5'
 *
 * Given a positive integer, check whether it has alternating bits: namely, if
 * two adjacent bits will always have different values.
 *
 *
 * Example 1:
 *
 *
 * Input: n = 5
 * Output: true
 * Explanation: The binary representation of 5 is: 101
 *
 *
 * Example 2:
 *
 *
 * Input: n = 7
 * Output: false
 * Explanation: The binary representation of 7 is: 111.
 *
 * Example 3:
 *
 *
 * Input: n = 11
 * Output: false
 * Explanation: The binary representation of 11 is: 1011.
 *
 *
 * Constraints:
 *
 *
 * 1 <= n <= 2^31 - 1
 *
 *
 */
class Solution {
public:
    bool hasAlternatingBits(int n) {
        // 1. 右移一位再xor;
        //   如果n满足 10101... ; 那么错位xor 必然右边都是1
        // 2. 把 错位xor 结果 +1; 求得 1000000 这样的2指数幂
        // 3. 用 00231. Power of Two 的方法; 判断是否是2指数幂
        unsigned m = n;
        m = (m^(m >> 1)) + 1;
        return (m & (m - 1)) == 0;
    }
};
