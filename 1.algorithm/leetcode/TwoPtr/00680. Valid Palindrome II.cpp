/*
 * @lc app=leetcode id=680 lang=cpp
 *
 * [680] Valid Palindrome II
 *
 * https://leetcode.com/problems/valid-palindrome-ii/description/
 *
 * algorithms
 * Easy (38.35%)
 * Total Accepted:    390.8K
 * Total Submissions: 1M
 * Testcase Example:  '"aba"'
 *
 * Given a string s, return true if the s can be palindrome after deleting at
 * most one character from it.
 *
 *
 * Example 1:
 *
 *
 * Input: s = "aba"
 * Output: true
 *
 *
 * Example 2:
 *
 *
 * Input: s = "abca"
 * Output: true
 * Explanation: You could delete the character 'c'.
 *
 *
 * Example 3:
 *
 *
 * Input: s = "abc"
 * Output: false
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= s.length <= 10^5
 * s consists of lowercase English letters.
 *
 *
 */
class Solution {
public:
    bool validPalindrome(string s) {
        int i = 0, j = s.size()-1;
        while (i < j) {
            if (s[i] != s[j]) {
                return validPalindrome(s, i, j-1) || validPalindrome(s, i+1, j);
            }
            i++;
            j--;
        }
        return true;
    }
private:
    bool validPalindrome(const string& s, int head, int tail) {
        int i = head, j = tail;
        while (i < j) {
            if (s[i] != s[j]) {
                return false;
            }
            i++;
            j--;
        }
        return true;
    }
};
