/*
 * @lc app=leetcode id=459 lang=cpp
 *
 * [459] Repeated Substring Pattern
 *
 * https://leetcode.com/problems/repeated-substring-pattern/description/
 *
 * algorithms
 * Easy (43.52%)
 * Total Accepted:    225.8K
 * Total Submissions: 518.7K
 * Testcase Example:  '"abab"'
 *
 * Given a string s, check if it can be constructed by taking a substring of it
 * and appending multiple copies of the substring together.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "abab"
 * Output: true
 * Explanation: It is the substring "ab" twice.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "aba"
 * Output: false
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "abcabcabcabc"
 * Output: true
 * Explanation: It is the substring "abc" four times or the substring "abcabc"
 * twice.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 10^4
 * s consists of lowercase English letters.
 * 
 * 
 */
class Solution {
public:
    bool repeatedSubstringPattern(string s) {
        for (int i = 1; i <= s.size()/2 /* at least repeated twice */; ++i) {
            if (s[i] == s.front()) {
                /* pattern 的字符个数 */
                int pattern_size = i;
                /* pattern 的字符个数不能被整除 */
                if (s.size() % pattern_size) {
                    continue;
                }
                bool found  = true;
                /* 确定 pattern */
                string pattern = s.substr(0, pattern_size);
                /* 遍历剩下 pattern */
                for (int j = i; j < s.size(); j += pattern_size) {
                    if (s.substr(j, pattern_size) == pattern) {
                        continue;
                    }
                    found = false;
                    break;
                }
                if (found == true) {
                    return true;
                }
            }
        }
        return false;
    }
};
