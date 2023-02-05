/*
 * @lc app=leetcode id=20 lang=cpp
 *
 * [20] Valid Parentheses
 *
 * https://leetcode.com/problems/valid-parentheses/description/
 *
 * algorithms
 * Easy (39.81%)
 * Total Accepted:    1.3M
 * Total Submissions: 3.3M
 * Testcase Example:  '"()"'
 *
 * Given a string s containing just the characters '(', ')', '{', '}', '[' and
 * ']', determine if the input string is valid.
 * 
 * An input string is valid if:
 * 
 * 
 * Open brackets must be closed by the same type of brackets.
 * Open brackets must be closed in the correct order.
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "()"
 * Output: true
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "()[]{}"
 * Output: true
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "(]"
 * Output: false
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: s = "([)]"
 * Output: false
 * 
 * 
 * Example 5:
 * 
 * 
 * Input: s = "{[]}"
 * Output: true
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 10^4
 * s consists of parentheses only '()[]{}'.
 * 
 * 
 */
class Solution {
public:
    inline char getParenthese(char c) {
        if (c == ')') return '(';
        if (c == ']') return '[';
        if (c == '}') return '{';
        return '\0';
    }
    bool isValid(string s) {
        vector<char> pending;
        for (auto c : s) {
            if (c == '(' || c == '{' || c == '[')
                pending.push_back(c);
            else if (!pending.empty() && pending.back() == getParenthese(c))
                pending.pop_back();
            else
                return false;
        }
        return pending.empty();
    }
};
