/*
 * @lc app=leetcode id=482 lang=cpp
 *
 * [482] License Key Formatting
 *
 * https://leetcode.com/problems/license-key-formatting/description/
 *
 * algorithms
 * Easy (43.09%)
 * Total Accepted:    200.6K
 * Total Submissions: 465.1K
 * Testcase Example:  '"5F3Z-2e-9-w"\n4'
 *
 * You are given a license key represented as a string s that consists of only
 * alphanumeric characters and dashes. The string is separated into n + 1
 * groups by n dashes. You are also given an integer k.
 * 
 * We want to reformat the string s such that each group contains exactly k
 * characters, except for the first group, which could be shorter than k but
 * still must contain at least one character. Furthermore, there must be a dash
 * inserted between two groups, and you should convert all lowercase letters to
 * uppercase.
 * 
 * Return the reformatted license key.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "5F3Z-2e-9-w", k = 4
 * Output: "5F3Z-2E9W"
 * Explanation: The string s has been split into two parts, each part has 4
 * characters.
 * Note that the two extra dashes are not needed and can be removed.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "2-5g-3-J", k = 2
 * Output: "2-5G-3J"
 * Explanation: The string s has been split into three parts, each part has 2
 * characters except the first part as it could be shorter as mentioned
 * above.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 10^5
 * s consists of English letters, digits, and dashes '-'.
 * 1 <= k <= 10^4
 * 
 * 
 */
class Solution {
public:
    string licenseKeyFormatting(string s, int k) {
        string res;
        for (auto iter = s.crbegin(); iter != s.crend(); ++iter) {
            // 忽略 '-'
            if (*iter == '-') continue;
            // 满足 A...A-A...A-A...A 时添加'-'
            if ((res.size() % (k+1)) == k) {
                res.push_back('-');
            }
            // 转换大写
            res.push_back(toupper(*iter));
        }
        // 翻转
        reverse(res.begin(), res.end());
        return res;
    }
};
