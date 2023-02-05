/*
 * @lc app=leetcode id=125 lang=cpp
 *
 * [125] Valid Palindrome
 *
 * https://leetcode.com/problems/valid-palindrome/description/
 *
 * algorithms
 * Easy (38.07%)
 * Total Accepted:    798.1K
 * Total Submissions: 2.1M
 * Testcase Example:  '"A man, a plan, a canal: Panama"'
 *
 * Given a string s, determine if it is a palindrome, considering only
 * alphanumeric characters and ignoring cases.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "A man, a plan, a canal: Panama"
 * Output: true
 * Explanation: "amanaplanacanalpanama" is a palindrome.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "race a car"
 * Output: false
 * Explanation: "raceacar" is not a palindrome.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 2 * 10^5
 * s consists only of printable ASCII characters.
 * 
 * 
 */
class Solution {
public:
    bool isPalindrome(string s) {
        // 1. 左右往中间遍历;
        // 2. 先获取 alnum 即 alpha or digit
        // 3. 如果 左右两边不相等 且 同时变为小写也不相等
        //    不满足条件; 提前退出
        // 4. 能接结束循环说明符合条件
        int i = 0, j = s.size()-1;
        while (i < j) {
            while (!isalnum(s[i]) && i < j)
                i++;
            while (!isalnum(s[j]) && i < j)
                j--;

            if (s[i] != s[j] && tolower(s[i]) != tolower(s[j]))
                    return false;

            i++;
            j--;
        }
        return true;
    }
};
