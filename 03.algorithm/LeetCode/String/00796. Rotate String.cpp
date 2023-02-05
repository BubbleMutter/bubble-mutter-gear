/*
 * @lc app=leetcode id=796 lang=cpp
 *
 * [796] Rotate String
 *
 * https://leetcode.com/problems/rotate-string/description/
 *
 * algorithms
 * Easy (51.12%)
 * Total Accepted:    134.2K
 * Total Submissions: 261.7K
 * Testcase Example:  '"abcde"\n"cdeab"'
 *
 * Given two strings s and goal, return true if and only if s can become goal
 * after some number of shifts on s.
 * 
 * A shift on s consists of moving the leftmost character of s to the rightmost
 * position.
 * 
 * 
 * For example, if s = "abcde", then it will be "bcdea" after one shift.
 * 
 * 
 * 
 * Example 1:
 * Input: s = "abcde", goal = "cdeab"
 * Output: true
 * Example 2:
 * Input: s = "abcde", goal = "abced"
 * Output: false
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length, goal.length <= 100
 * s and goal consist of lowercase English letters.
 * 
 * 
 */
class Solution {
public:
    bool rotateString(string s, string goal) {
        if (s.size() != goal.size()) return false;
        for (size_t i = 0; i < goal.size(); ++i) {
            if (goal[i] == s[0]) {
                if (goal.substr(i) == s.substr(0, goal.size()-i) &&
                    goal.substr(0, i) == s.substr(goal.size()-i)) {
                    return true;
                }
            }
        }
        return false;
    }
};
