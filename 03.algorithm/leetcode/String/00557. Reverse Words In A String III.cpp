/*
 * @lc app=leetcode id=557 lang=cpp
 *
 * [557] Reverse Words in a String III
 *
 * https://leetcode.com/problems/reverse-words-in-a-string-iii/description/
 *
 * algorithms
 * Easy (77.17%)
 * Total Accepted:    383.4K
 * Total Submissions: 495.5K
 * Testcase Example:  '"Let\'s take LeetCode contest"'
 *
 * Given a string s, reverse the order of characters in each word within a
 * sentence while still preserving whitespace and initial word order.
 * 
 * 
 * Example 1:
 * Input: s = "Let's take LeetCode contest"
 * Output: "s'teL ekat edoCteeL tsetnoc"
 * Example 2:
 * Input: s = "God Ding"
 * Output: "doG gniD"
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 5 * 10^4
 * s contains printable ASCII characters.
 * s does not contain any leading or trailing spaces.
 * There is at least one word in s.
 * All the words in s are separated by a single space.
 * 
 * 
 */
class Solution {
public:
    string reverseWords(string s) {
        int start = 0;
        // 1. 定位 word 结尾字符
        // 2. 记录单词开头
        // 3. 翻转
        for (int i = 0; i < s.size(); i++) {
            if (s[i] != ' ' && (i+1 == s.size() || s[i+1] == ' ')) {
                reverse(s.begin()+start, s.begin()+i+1);
                start = i+2; /* word 只有一个 space 相隔 */
            }
        }
        return s;
    }
};
