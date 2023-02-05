/*
 * @lc app=leetcode id=1221 lang=cpp
 *
 * [1221] Split a String in Balanced Strings
 *
 * https://leetcode.com/problems/split-a-string-in-balanced-strings/description/
 *
 * algorithms
 * Easy (84.76%)
 * Total Accepted:    191.6K
 * Total Submissions: 226.2K
 * Testcase Example:  '"RLRRLLRLRL"'
 *
 * Balanced strings are those that have an equal quantity of 'L' and 'R'
 * characters.
 * 
 * Given a balanced string s, split it in the maximum amount of balanced
 * strings.
 * 
 * Return the maximum amount of split balanced strings.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "RLRRLLRLRL"
 * Output: 4
 * Explanation: s can be split into "RL", "RRLL", "RL", "RL", each substring
 * contains same number of 'L' and 'R'.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "RLLLLRRRLR"
 * Output: 3
 * Explanation: s can be split into "RL", "LLLRRR", "LR", each substring
 * contains same number of 'L' and 'R'.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "LLLLRRRR"
 * Output: 1
 * Explanation: s can be split into "LLLLRRRR".
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 1000
 * s[i] is either 'L' or 'R'.
 * s is a balanced string.
 * 
 * 
 */
class Solution {
public:
    int balancedStringSplit(string s) {
        // 投票法:
        // 1. 遍历字符串
        // 2. 'L' 投票++; 'R'投票--
        // 3. 当投票为0, 则分割数++
        size_t vote = 0, count = 0;
        for (const char c: s) {
            if (c == 'L') {
                vote++;
            } else {
                vote--;
            }
            if (vote == 0) {
                count++;
            }
        }
        return count;
    }
};
