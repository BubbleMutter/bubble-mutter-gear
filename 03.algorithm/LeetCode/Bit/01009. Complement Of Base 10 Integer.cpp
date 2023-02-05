/*
 * @lc app=leetcode id=1009 lang=cpp
 *
 * [1009] Complement of Base 10 Integer
 *
 * https://leetcode.com/problems/complement-of-base-10-integer/description/
 *
 * algorithms
 * Easy (62.44%)
 * Total Accepted:    120.3K
 * Total Submissions: 192.9K
 * Testcase Example:  '5'
 *
 * The complement of an integer is the integer you get when you flip all the
 * 0's to 1's and all the 1's to 0's in its binary representation.
 * 
 * 
 * For example, The integer 5 is "101" in binary and its complement is "010"
 * which is the integer 2.
 * 
 * 
 * Given an integer n, return its complement.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 5
 * Output: 2
 * Explanation: 5 is "101" in binary, with complement "010" in binary, which is
 * 2 in base-10.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 7
 * Output: 0
 * Explanation: 7 is "111" in binary, with complement "000" in binary, which is
 * 0 in base-10.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: n = 10
 * Output: 5
 * Explanation: 10 is "1010" in binary, with complement "0101" in binary, which
 * is 5 in base-10.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= n < 10^9
 * 
 * 
 * 
 * Note: This question is the same as 476:
 * https://leetcode.com/problems/number-complement/
 * 
 */
class Solution {
public:
    int bitwiseComplement(int n) {
        if (n == 0) return 1;
        int m = n, mask = n;
        // 如题: 5 -> b'101, complement b'10; 而实际上 5 xor b'111 = b'10
        //       7 -> b'101, complement b'0;  而实际上 7 xor b'111 = b'10
        // 所以实际上求 n 的所有位的掩码, 取异或
        //
        // 1. 求得最高位的 1 bits
        // 2. 转化位所有位 1 bits
        // 3. n xor 步骤2
        while (m) {
            mask = m;
            m &= (m - 1);
        }
        return n ^ (mask | (mask - 1));
    }
};
