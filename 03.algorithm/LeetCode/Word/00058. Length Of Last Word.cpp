/*
 * @lc app=leetcode id=58 lang=cpp
 *
 * [58] Length of Last Word
 *
 * https://leetcode.com/problems/length-of-last-word/description/
 *
 * algorithms
 * Easy (33.45%)
 * Total Accepted:    475.5K
 * Total Submissions: 1.4M
 * Testcase Example:  '"Hello World"'
 *
 * Given a string s consists of some words separated by spaces, return the
 * length of the last word in the string. If the last word does not exist,
 * return 0.
 * 
 * A word is a maximal substring consisting of non-space characters only.
 * 
 * 
 * Example 1:
 * Input: s = "Hello World"
 * Output: 5
 * Example 2:
 * Input: s = " "
 * Output: 0
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 10^4
 * s consists of only English letters and spaces ' '.
 * 
 * 
 */
class Solution {
public:
    int lengthOfLastWord(string s) {
        // 1. 从右往左找; 首个字母
        // 2. 找不到; 返回0
        // 3. 在1.的基础上从右往左找第一个空格
        // 4. 返回1. - 3. 的距离
        int i = s.size()-1;
        for (; i >= 0; i--)
            if (isalpha(s[i])) break;

        if (i < 0) return 0;

        int j = i;
        for (; j >= 0; j--)
            if (s[j] == ' ') break;

        return i-j;
    }
};
