/*
 * @lc app=leetcode id=405 lang=cpp
 *
 * [405] Convert a Number to Hexadecimal
 *
 * https://leetcode.com/problems/convert-a-number-to-hexadecimal/description/
 *
 * algorithms
 * Easy (45.54%)
 * Total Accepted:    93.7K
 * Total Submissions: 205.7K
 * Testcase Example:  '26'
 *
 * Given an integer num, return a string representing its hexadecimal
 * representation. For negative integers, two’s complement method is used.
 * 
 * All the letters in the answer string should be lowercase characters, and
 * there should not be any leading zeros in the answer except for the zero
 * itself.
 * 
 * Note: You are not allowed to use any built-in library method to directly
 * solve this problem.
 * 
 * 
 * Example 1:
 * Input: num = 26
 * Output: "1a"
 * Example 2:
 * Input: num = -1
 * Output: "ffffffff"
 * 
 * 
 * Constraints:
 * 
 * 
 * -2^31 <= num <= 2^31 - 1
 * 
 * 
 */
class Solution {
public:
    string toHex(int num) {
        string res;
        static char mapping[] = {
            '0', '1', '2', '3',
            '4', '5', '6', '7',
            '8', '9', 'a', 'b',
            'c', 'd', 'e', 'f',
        };
        // 负数的有符号右移; 高位会补1
        unsigned n = num;
        if (n == 0) return string("0");
        while (n != 0) {
            res.push_back(mapping[n & 0xf]);
            n >>= 4;
        }
        reverse(res.begin(), res.end());
        return res;
    }
};
