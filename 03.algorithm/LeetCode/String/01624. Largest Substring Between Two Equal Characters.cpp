/*
 * @lc app=leetcode id=1624 lang=cpp
 *
 * [1624] Largest Substring Between Two Equal Characters
 *
 * https://leetcode.com/problems/largest-substring-between-two-equal-characters/description/
 *
 * algorithms
 * Easy (59.10%)
 * Total Accepted:    15.8K
 * Total Submissions: 26.7K
 * Testcase Example:  '"aa"'
 *
 * Given a string s, return the length of the longest substring between two
 * equal characters, excluding the two characters. If there is no such
 * substring return -1.
 * 
 * A substring is a contiguous sequence of characters within a string.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "aa"
 * Output: 0
 * Explanation: The optimal substring here is an empty substring between the
 * two 'a's.
 * 
 * Example 2:
 * 
 * 
 * Input: s = "abca"
 * Output: 2
 * Explanation: The optimal substring here is "bc".
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "cbzxy"
 * Output: -1
 * Explanation: There are no characters that appear twice in s.
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: s = "cabbac"
 * Output: 4
 * Explanation: The optimal substring here is "abba". Other non-optimal
 * substrings include "bb" and "".
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 300
 * s contains only lowercase English letters.
 * 
 * 
 */
class Solution {
public:
    int maxLengthBetweenEqualCharacters(string s) {
        // 记录字符首次出现的 index 即可
        vector<int>  alpha(26, -1);
        int length = -1;
        for (int i = 0; i < s.size(); ++i) {
            if (alpha[s[i]-'a'] < 0)
                alpha[s[i]-'a'] = i;
            else
                length = max(length, i-alpha[s[i]-'a']-1);
        }
        return length;
    }
};
