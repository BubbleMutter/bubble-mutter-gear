/*
 * @lc app=leetcode id=1160 lang=cpp
 *
 * [1160] Find Words That Can Be Formed by Characters
 *
 * https://leetcode.com/problems/find-words-that-can-be-formed-by-characters/description/
 *
 * algorithms
 * Easy (67.72%)
 * Total Accepted:    107.7K
 * Total Submissions: 159.2K
 * Testcase Example:  '["cat","bt","hat","tree"]\n"atach"'
 *
 * You are given an array of strings words and a string chars.
 *
 * A string is good if it can be formed by characters from chars (each
 * character can only be used once).
 *
 * Return the sum of lengths of all good strings in words.
 *
 *
 * Example 1:
 *
 *
 * Input: words = ["cat","bt","hat","tree"], chars = "atach"
 * Output: 6
 * Explanation: The strings that can be formed are "cat" and "hat" so the
 * answer is 3 + 3 = 6.
 *
 *
 * Example 2:
 *
 *
 * Input: words = ["hello","world","leetcode"], chars = "welldonehoneyr"
 * Output: 10
 * Explanation: The strings that can be formed are "hello" and "world" so the
 * answer is 5 + 5 = 10.
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= words.length <= 1000
 * 1 <= words[i].length, chars.length <= 100
 * words[i] and chars consist of lowercase English letters.
 *
 *
 */
class Solution {
public:
    int countCharacters(vector<string>& words, string chars) {
        // 1. 根据 chars 构造字符池
        // 2. 遍历每个 word, 先复制一个字符池, 判断字符池中是否够用
        // 3. 够用时结果++
        vector<int> pool(26, 0);
        for (const char c: chars) {
            pool[(int)(c-'a')]++;
        }
        size_t count = 0;
        for (const auto& word: words) {
            vector<int> temp(pool);
            size_t i = 0;
            for (; i < word.size(); ++i) {
                if (--temp[word[i]-'a'] < 0) {
                    break;
                }
            }
            count += (i == word.size()) ? word.size() : 0;
        }
        return count;
    }
};
