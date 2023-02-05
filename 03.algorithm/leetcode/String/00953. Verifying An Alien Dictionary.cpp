/*
 * @lc app=leetcode id=953 lang=cpp
 *
 * [953] Verifying an Alien Dictionary
 *
 * https://leetcode.com/problems/verifying-an-alien-dictionary/description/
 *
 * algorithms
 * Easy (52.29%)
 * Total Accepted:    328.5K
 * Total Submissions: 627.8K
 * Testcase Example:  '["hello","leetcode"]\n"hlabcdefgijkmnopqrstuvwxyz"'
 *
 * In an alien language, surprisingly, they also use English lowercase letters,
 * but possibly in a different order. The order of the alphabet is some
 * permutation of lowercase letters.
 * 
 * Given a sequence of words written in the alien language, and the order of
 * the alphabet, return true if and only if the given words are sorted
 * lexicographically in this alien language.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: words = ["hello","leetcode"], order = "hlabcdefgijkmnopqrstuvwxyz"
 * Output: true
 * Explanation: As 'h' comes before 'l' in this language, then the sequence is
 * sorted.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: words = ["word","world","row"], order = "worldabcefghijkmnpqstuvxyz"
 * Output: false
 * Explanation: As 'd' comes after 'l' in this language, then words[0] >
 * words[1], hence the sequence is unsorted.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: words = ["apple","app"], order = "abcdefghijklmnopqrstuvwxyz"
 * Output: false
 * Explanation: The first three characters "app" match, and the second string
 * is shorter (in size.) According to lexicographical rules "apple" > "app",
 * because 'l' > '∅', where '∅' is defined as the blank character which is less
 * than any other character (More info).
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= words.length <= 100
 * 1 <= words[i].length <= 20
 * order.length == 26
 * All characters in words[i] and order are English lowercase letters.
 * 
 * 
 */
class Solution {
public:
    bool isAlienSorted(vector<string>& words, string order) {
        vector<int> alpha(26, 0);
        // 构造字母索引映射表
        for (size_t i = 0; i < order.size(); ++i) {
            alpha[order[i]-'a'] = i;
        }
        // 重写 strcmp
        // 1. 找到首个不相同的字符; 比较该位置字符的大小
        // 2. 如果前置所有字符相同, 则较短的字符 较小
        auto compare = [&](const string& x, const string& y) {
            int i = 0, j = 0;
            while (i < x.size() && j < y.size()) {
                if (x[i] == y[j]) {
                    ++i;
                    ++j;
                    continue;
                }
                return alpha[x[i]-'a'] < alpha[y[j]-'a'];
            }
            return x.size() < y.size();
        };
        // is_sorted 算子
        return is_sorted(words.begin(), words.end(), compare);
    }
};
