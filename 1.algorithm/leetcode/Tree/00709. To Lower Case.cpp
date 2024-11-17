/*
 * @lc app=leetcode id=709 lang=cpp
 *
 * [709] To Lower Case
 *
 * https://leetcode.com/problems/to-lower-case/description/
 *
 * algorithms
 * Easy (79.92%)
 * Total Accepted:    242.8K
 * Total Submissions: 303.8K
 * Testcase Example:  '"Hello"'
 *
 * Implement function ToLowerCase() that has a string parameter str, and
 * returns the same string in lowercase.
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: "Hello"
 * Output: "hello"
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: "here"
 * Output: "here"
 * 
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: "LOVELY"
 * Output: "lovely"
 * 
 * 
 * 
 * 
 */
class Solution {
public:
    string toLowerCase(string str) {
        for (size_t i = 0; i < str.size(); ++i) {
            const auto& c = str[i];
            if ('A' <= c && c <= 'Z') {
                str[i] -= 'A'-'a';
            }
        }
        return str;
    }
};
