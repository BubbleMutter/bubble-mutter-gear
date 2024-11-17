/*
 * @lc app=leetcode id=1021 lang=cpp
 *
 * [1021] Remove Outermost Parentheses
 *
 * https://leetcode.com/problems/remove-outermost-parentheses/description/
 *
 * algorithms
 * Easy (79.63%)
 * Total Accepted:    164.4K
 * Total Submissions: 206.2K
 * Testcase Example:  '"(()())(())"'
 *
 * A valid parentheses string is either empty "", "(" + A + ")", or A + B,
 * where A and B are valid parentheses strings, and + represents string
 * concatenation.
 * 
 * 
 * For example, "", "()", "(())()", and "(()(()))" are all valid parentheses
 * strings.
 * 
 * 
 * A valid parentheses string s is primitive if it is nonempty, and there does
 * not exist a way to split it into s = A + B, with A and B nonempty valid
 * parentheses strings.
 * 
 * Given a valid parentheses string s, consider its primitive decomposition: s
 * = P1 + P2 + ... + Pk, where Pi are primitive valid parentheses strings.
 * 
 * Return s after removing the outermost parentheses of every primitive string
 * in the primitive decomposition of s.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "(()())(())"
 * Output: "()()()"
 * Explanation: 
 * The input string is "(()())(())", with primitive decomposition "(()())" +
 * "(())".
 * After removing outer parentheses of each part, this is "()()" + "()" =
 * "()()()".
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "(()())(())(()(()))"
 * Output: "()()()()(())"
 * Explanation: 
 * The input string is "(()())(())(()(()))", with primitive decomposition
 * "(()())" + "(())" + "(()(()))".
 * After removing outer parentheses of each part, this is "()()" + "()" +
 * "()(())" = "()()()()(())".
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "()()"
 * Output: ""
 * Explanation: 
 * The input string is "()()", with primitive decomposition "()" + "()".
 * After removing outer parentheses of each part, this is "" + "" = "".
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 10^5
 * s[i] is either '(' or ')'.
 * s is a valid parentheses string.
 * 
 * 
 */
class Solution {
public:
    string removeOuterParentheses(string s) {
        string res;
#if 0 /* 直观方法 */
        // 用 stack 维护对括号
        stack<char> stk;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == ')') {
                stk.pop();
                // stk 为空, 说明是最外层')'被pop了
                if (stk.size() > 0) {
                    res += s[i];
                }
            } else {
                stk.push(s[i]);
                // 遗留最外层 '(', 必须 > 1
                if (stk.size() > 1) {
                    res += s[i];
                }
            }
        }
        cout << stk.size() << endl;
#else
        // 计数器, 记录'(' 的个数;
        // 因为题意 s 必然成对, 不需要 stack 强校验合法性
        uint32_t count = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == ')') {
                --count;
                // count 为0, 说明是最外层')'被pop了, 所以这里需要 > 0
                if (count > 0) {
                    res += s[i];
                }
            } else {
                ++count;
                // 遗留最外层 '(', 必须 > 1
                if (count > 1) {
                    res += s[i];
                }
            }
        }
#endif
        return res;
    }
};
