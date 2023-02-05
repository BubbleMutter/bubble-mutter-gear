/*
 * @lc app=leetcode id=504 lang=cpp
 *
 * [504] Base 7
 *
 * https://leetcode.com/problems/base-7/description/
 *
 * algorithms
 * Easy (47.22%)
 * Total Accepted:    81K
 * Total Submissions: 171.3K
 * Testcase Example:  '100'
 *
 * Given an integer num, return a string of its base 7 representation.
 * 
 * 
 * Example 1:
 * Input: num = 100
 * Output: "202"
 * Example 2:
 * Input: num = -7
 * Output: "-10"
 * 
 * 
 * Constraints:
 * 
 * 
 * -10^7 <= num <= 10^7
 * 
 * 
 */
class Solution {
public:
    string convertToBase7(int num) {
        if (num == 0) return "0";
        string res;
        bool negative = false;
        if (num < 0) {
            negative = true;
            num = -num;
        }
        while (num) {
            res.push_back('0' + num % 7);
            num /= 7;
        }
        if (negative) {
            res.push_back('-');
        }
        reverse(res.begin(), res.end());
        return res;
    }
};
