/*
 * @lc app=leetcode id=205 lang=cpp
 *
 * [205] Isomorphic Strings
 *
 * https://leetcode.com/problems/isomorphic-strings/description/
 *
 * algorithms
 * Easy (40.42%)
 * Total Accepted:    345.7K
 * Total Submissions: 853.9K
 * Testcase Example:  '"egg"\n"add"'
 *
 * Given two strings s and t, determine if they are isomorphic.
 * 
 * Two strings s and t are isomorphic if the characters in s can be replaced to
 * get t.
 * 
 * All occurrences of a character must be replaced with another character while
 * preserving the order of characters. No two characters may map to the same
 * character, but a character may map to itself.
 * 
 * 
 * Example 1:
 * Input: s = "egg", t = "add"
 * Output: true
 * Example 2:
 * Input: s = "foo", t = "bar"
 * Output: false
 * Example 3:
 * Input: s = "paper", t = "title"
 * Output: true
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 5 * 10^4
 * t.length == s.length
 * s and t consist of any valid ascii character.
 * 
 * 
 */
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        // 1. 维护 s => t 的映射表
        //    维护 t => s 的映射表
        // 2. 遍历 s, t
        // 3. 如果没有建立映射那就建立映射
        // 4. 如果无法互相映射;
        //    那么返回false
        map<char, char> maps;
        map<char, char> mapt;
        for (int i = 0; i < s.size(); i++) {
            if (!maps[s[i]])
                maps[s[i]] = t[i];
            if (!mapt[t[i]])
                mapt[t[i]] = s[i];
            if (maps[s[i]] != t[i] ||
                mapt[t[i]] != s[i])
                return false;
        }
        return true;
    }
};
