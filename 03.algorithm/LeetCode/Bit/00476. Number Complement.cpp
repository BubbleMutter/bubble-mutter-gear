/*
 * @lc app=leetcode id=476 lang=cpp
 *
 * [476] Number Complement
 *
 * https://leetcode.com/problems/number-complement/description/
 *
 * algorithms
 * Easy (66.51%)
 * Total Accepted:    256K
 * Total Submissions: 384.8K
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
 * Given an integer num, return its complement.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: num = 5
 * Output: 2
 * Explanation: The binary representation of 5 is 101 (no leading zero bits),
 * and its complement is 010. So you need to output 2.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: num = 1
 * Output: 0
 * Explanation: The binary representation of 1 is 1 (no leading zero bits), and
 * its complement is 0. So you need to output 0.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= num < 2^31
 * 
 * 
 * 
 * Note: This question is the same as 1009:
 * https://leetcode.com/problems/complement-of-base-10-integer/
 * 
 */
class Solution {
public:
    int findComplement(int num) {
        int val = 0, mask = num;
        /* 找到最高位的1 */
        while ((val = mask & (mask - 1)) != 0) {
            mask = val;
        }
        /* mask-1 为除去最高为的后续位都为 1的数 */
        return (~num) & (mask-1);
    }
};
