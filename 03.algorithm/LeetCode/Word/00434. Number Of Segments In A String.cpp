/*
 * @lc app=leetcode id=434 lang=cpp
 *
 * [434] Number of Segments in a String
 *
 * https://leetcode.com/problems/number-of-segments-in-a-string/description/
 *
 * algorithms
 * Easy (37.94%)
 * Total Accepted:    109.3K
 * Total Submissions: 288K
 * Testcase Example:  '"Hello, my name is John"'
 *
 * Given a string s, return the number of segments in the string.
 * 
 * A segment is defined to be a contiguous sequence of non-space characters.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "Hello, my name is John"
 * Output: 5
 * Explanation: The five segments are ["Hello,", "my", "name", "is", "John"]
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "Hello"
 * Output: 1
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= s.length <= 300
 * s consists of lowercase and uppercase English letters, digits, or one of the
 * following characters "!@#$%^&*()_+-=',.:".
 * The only space character in s is ' '.
 * 
 * 
 */
class Solution {
public:
    int countSegments(string s) {
        int i = 0, count = 0;
#if 0 /* 小优化以下 */
        while (i < s.size()) {
            if (s[i] != ' ') {
                if (i+1 == s.size() || s[i+1] == ' ') {
                    count++;
                    i += 2;
                } else {
                    ++i;
                }
            } else {
                ++i;
            }
        }
#endif
        for (int i = 0; i < s.size(); ++i) {
            // NOTE: 需要避免连续空格的情况
            //       所以首先要定位到一个非空格的字符, 且其下个字符不为空格
                count++;
        }
        return count;
    }
};
