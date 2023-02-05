/*
 * @lc app=leetcode id=844 lang=cpp
 *
 * [844] Backspace String Compare
 *
 * https://leetcode.com/problems/backspace-string-compare/description/
 *
 * algorithms
 * Easy (47.45%)
 * Total Accepted:    378K
 * Total Submissions: 797.4K
 * Testcase Example:  '"ab#c"\n"ad#c"'
 *
 * Given two strings s and t, return true if they are equal when both are typed
 * into empty text editors. '#' means a backspace character.
 * 
 * Note that after backspacing an empty text, the text will continue empty.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "ab#c", t = "ad#c"
 * Output: true
 * Explanation: Both s and t become "ac".
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "ab##", t = "c#d#"
 * Output: true
 * Explanation: Both s and t become "".
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "a#c", t = "b"
 * Output: false
 * Explanation: s becomes "c" while t becomes "b".
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length, t.length <= 200
 * s and t only contain lowercase letters and '#' characters.
 * 
 * 
 * 
 * Follow up: Can you solve it in O(n) time and O(1) space?
 * 
 */
class Solution {
public:
    bool backspaceCompare(string s, string t) {
        vector<char> rs, rt;
        removeBackspace(s, rs);
        removeBackspace(t, rt);
        return rs == rt;
    }
private:
    void removeBackspace(string s, vector<char>& res) {
        for (size_t i = 0; i < s.size(); i++) {
            if (s[i] == '#') {
                if (!res.empty()) {
                    res.pop_back();
                }
            } else {
                res.push_back(s[i]);
            }
        }
    }
};
