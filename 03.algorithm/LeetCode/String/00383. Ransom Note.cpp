/*
 * @lc app=leetcode id=383 lang=cpp
 *
 * [383] Ransom Note
 *
 * https://leetcode.com/problems/ransom-note/description/
 *
 * algorithms
 * Easy (53.28%)
 * Total Accepted:    254K
 * Total Submissions: 476.4K
 * Testcase Example:  '"a"\n"b"'
 *
 * Given an arbitrary ransom note string and another string containing letters
 * from all the magazines, write a function that will return true if the ransom
 * note can be constructed from the magazines ; otherwise, it will return
 * false.
 * 
 * Each letter in the magazine string can only be used once in your ransom
 * note.
 * 
 * 
 * Example 1:
 * Input: ransomNote = "a", magazine = "b"
 * Output: false
 * Example 2:
 * Input: ransomNote = "aa", magazine = "ab"
 * Output: false
 * Example 3:
 * Input: ransomNote = "aa", magazine = "aab"
 * Output: true
 * 
 * 
 * Constraints:
 * 
 * 
 * You may assume that both strings contain only lowercase letters.
 * 
 * 
 */
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        // 1. hash magazine; 计算每个字母出现的次数
        // 2. loop ransomNote; 消耗 hash 中节点的计数;
        //                     当次数小于0; return false
        // 3. 如果 loop ransomNote 结束; 即满足条件

        int tbl[26] = { 0 };
        for (auto c : magazine) {
            tbl[c-'a']++;
        }
        for (auto c : ransomNote) {
            if (--tbl[c-'a'] < 0)
                return false;
        }
        return true;
    }
};
