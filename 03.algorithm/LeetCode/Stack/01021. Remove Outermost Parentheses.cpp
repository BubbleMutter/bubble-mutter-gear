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
#if 0 /* ???????????? */
        // ??? stack ???????????????
        stack<char> stk;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == ')') {
                stk.pop();
                // stk ??????, ??????????????????')'???pop???
                if (stk.size() > 0) {
                    res += s[i];
                }
            } else {
                stk.push(s[i]);
                // ??????????????? '(', ?????? > 1
                if (stk.size() > 1) {
                    res += s[i];
                }
            }
        }
        cout << stk.size() << endl;
#else
        // ?????????, ??????'(' ?????????;
        // ???????????? s ????????????, ????????? stack ??????????????????
        uint32_t count = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == ')') {
                --count;
                // count ???0, ??????????????????')'???pop???, ?????????????????? > 0
                if (count > 0) {
                    res += s[i];
                }
            } else {
                ++count;
                // ??????????????? '(', ?????? > 1
                if (count > 1) {
                    res += s[i];
                }
            }
        }
#endif
        return res;
    }
};
