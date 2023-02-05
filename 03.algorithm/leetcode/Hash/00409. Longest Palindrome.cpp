/*
 * @lc app=leetcode id=409 lang=cpp
 *
 * [409] Longest Palindrome
 *
 * https://leetcode.com/problems/longest-palindrome/description/
 *
 * algorithms
 * Easy (53.15%)
 * Total Accepted:    248.3K
 * Total Submissions: 466.8K
 * Testcase Example:  '"abccccdd"'
 *
 * Given a string s which consists of lowercase or uppercase letters, return
 * the length of the longest palindrome that can be built with those letters.
 * 
 * Letters are case sensitive, for example, "Aa" is not considered a palindrome
 * here.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "abccccdd"
 * Output: 7
 * Explanation:
 * One longest palindrome that can be built is "dccaccd", whose length is 7.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "a"
 * Output: 1
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "bb"
 * Output: 2
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 2000
 * s consists of lowercase and/or uppercase English letters only.
 * 
 * 
 */
class Solution {
public:
    int longestPalindrome(string s) {
        // 最大奇数频次 + {其他每个计数频次-1} + {每个偶数频次}
        // 即 { 所有奇数频次-1} + 1 + {所有偶数频次}
        int count[0xffff] = { 0 };
        for (const auto& c: s) {
            count[c]++;
        }
        int res = 0;
        bool odd = false;
        for (const auto& c: count) {
            if (c & 0x1) {
                res += c-1;
                odd = true;
            } else {
                res += c;
            }
        }
        if (odd) {
            res += 1;
        }
        return res;
    }
};
