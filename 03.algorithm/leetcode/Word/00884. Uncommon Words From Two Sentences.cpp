/*
 * @lc app=leetcode id=884 lang=cpp
 *
 * [884] Uncommon Words from Two Sentences
 *
 * https://leetcode.com/problems/uncommon-words-from-two-sentences/description/
 *
 * algorithms
 * Easy (65.24%)
 * Total Accepted:    92.5K
 * Total Submissions: 141.6K
 * Testcase Example:  '"this apple is sweet"\n"this apple is sour"'
 *
 * A sentence is a string of single-space separated words where each word
 * consists only of lowercase letters.
 *
 * A word is uncommon if it appears exactly once in one of the sentences, and
 * does not appear in the other sentence.
 *
 * Given two sentences s1 and s2, return a list of all the uncommon words. You
 * may return the answer in any order.
 *
 *
 * Example 1:
 * Input: s1 = "this apple is sweet", s2 = "this apple is sour"
 * Output: ["sweet","sour"]
 * Example 2:
 * Input: s1 = "apple apple", s2 = "banana"
 * Output: ["banana"]
 *
 *
 * Constraints:
 *
 *
 * 1 <= s1.length, s2.length <= 200
 * s1 and s2 consist of lowercase English letters and spaces.
 * s1 and s2 do not have leading or trailing spaces.
 * All the words in s1 and s2 are separated by a single space.
 *
 *
 */
class Solution {
public:
    vector<string> uncommonFromSentences(string s1, string s2) {
        // 1. 把两个 sentence 的 word 构造为可计数 set
        // 2. 分别遍历两个 set
        multiset<string> set1, set2;
        for (size_t i = 0, j = 0; i < s1.size(); ++i) {
            if (isalpha(s1[i])) {
                // 取词
                if (i+1 == s1.size() || s1[i+1] == ' ') {
                    set1.insert(s1.substr(j, i+1-j));
                    j = i+2;
                }
            }
        }
        for (size_t i = 0, j = 0; i < s2.size(); ++i) {
            if (isalpha(s2[i])) {
                // 取词
                if ((i+1 == s2.size() || s2[i+1] == ' ')) {
                    set2.insert(s2.substr(j, i+1-j));
                    j = i+2;
                }
            }
        }
        vector<string> res;
        for (const auto& word: set1) {
            if (set2.count(word) == 0 && set1.count(word) == 1) {
                res.push_back(word);
            }
        }
        for (const auto& word: set2) {
            if (set1.count(word) == 0 && set2.count(word) == 1) {
                res.push_back(word);
            }
        }
        return res;
    }
};
