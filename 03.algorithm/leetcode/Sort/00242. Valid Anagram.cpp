/*
 * @lc app=leetcode id=242 lang=cpp
 *
 * [242] Valid Anagram
 *
 * https://leetcode.com/problems/valid-anagram/description/
 *
 * algorithms
 * Easy (58.05%)
 * Total Accepted:    764.7K
 * Total Submissions: 1.3M
 * Testcase Example:  '"anagram"\n"nagaram"'
 *
 * Given two strings s and t , write a function to determine if t is an anagram
 * of s.
 * 
 * Example 1:
 * 
 * 
 * Input: s = "anagram", t = "nagaram"
 * Output: true
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "rat", t = "car"
 * Output: false
 * 
 * 
 * Note:
 * You may assume the string contains only lowercase alphabets.
 * 
 * Follow up:
 * What if the inputs contain unicode characters? How would you adapt your
 * solution to such case?
 * 
 */
class Solution {
public:
    bool isAnagram(string s, string t) {
        /* 解决 unicode 的标准做法 */
        sort(s.begin(), s.end());
        sort(t.begin(), t.end());
        return s == t;
#if 0 /* hash 做法 */
        // 1. 题意: 两字符串每种字符出现次数相同
        // 2. 遍历s, 字符计数++
        // 3. 遍历s, 字符计数--
        // 4. 遍历字符表; 计数非零就不符合
        int alpha[26] = { 0 };
        for (auto c: s)
            alpha[c-'a']++;
        for (auto c: t)
            alpha[c-'a']--;
        for (int i = 0; i < 26; i++)
            if (alpha[i] != 0)
                return false;
        return true;
#endif
    }
};
