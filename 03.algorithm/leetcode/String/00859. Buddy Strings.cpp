/*
 * @lc app=leetcode id=859 lang=cpp
 *
 * [859] Buddy Strings
 *
 * https://leetcode.com/problems/buddy-strings/description/
 *
 * algorithms
 * Easy (28.75%)
 * Total Accepted:    111.9K
 * Total Submissions: 389K
 * Testcase Example:  '"ab"\n"ba"'
 *
 * Given two strings s and goal, return true if you can swap two letters in s
 * so the result is equal to goal, otherwise, return false.
 * 
 * Swapping letters is defined as taking two indices i and j (0-indexed) such
 * that i != j and swapping the characters at s[i] and s[j].
 * 
 * 
 * For example, swapping at indices 0 and 2 in "abcd" results in "cbad".
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "ab", goal = "ba"
 * Output: true
 * Explanation: You can swap s[0] = 'a' and s[1] = 'b' to get "ba", which is
 * equal to goal.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "ab", goal = "ab"
 * Output: false
 * Explanation: The only letters you can swap are s[0] = 'a' and s[1] = 'b',
 * which results in "ba" != goal.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "aa", goal = "aa"
 * Output: true
 * Explanation: You can swap s[0] = 'a' and s[1] = 'a' to get "aa", which is
 * equal to goal.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length, goal.length <= 2 * 10^4
 * s and goal consist of lowercase letters.
 * 
 * 
 */
// #include<prettyprint.hpp>
class Solution {
public:
    bool buddyStrings(string s, string goal) {
        if (s.size() != goal.size()) return false;
        vector<int> alpha(26, 0);
        vector<int> swapIndex;
        int i = 0;
        for (; i < s.size(); ++i) {
            alpha[s[i]-'a']++;
            if (s[i] == goal[i]) {
                continue;
            }
            if (swapIndex.size() == 2) {
                return false;
            }
            swapIndex.push_back(i);
        }
        // 场景1: 两字符串完全相同; 则需要至少一个字母出现两次才能swap
        // 场景2: 两字符串两个位置不同; 则两个位置不同的字母交叉相同
        //
        // 遍历过程中: 记录字母出现的次数 和 不同的位置索引
        return (swapIndex.size() == 0 &&
                any_of(alpha.begin(), alpha.end(), [](const int i) { return i > 1; })) ||
               (swapIndex.size() == 2 &&
                s[swapIndex[0]] == goal[swapIndex[1]] &&
                s[swapIndex[1]] == goal[swapIndex[0]]);
    }
};
