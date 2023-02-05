/*
 * @lc app=leetcode id=520 lang=cpp
 *
 * [520] Detect Capital
 *
 * https://leetcode.com/problems/detect-capital/description/
 *
 * algorithms
 * Easy (53.85%)
 * Total Accepted:    181.2K
 * Total Submissions: 336.6K
 * Testcase Example:  '"USA"'
 *
 * Given a word, you need to judge whether the usage of capitals in it is right
 * or not.
 *
 * We define the usage of capitals in a word to be right when one of the
 * following cases holds:
 *
 *
 * All letters in this word are capitals, like "USA".
 * All letters in this word are not capitals, like "leetcode".
 * Only the first letter in this word is capital, like "Google".
 *
 * Otherwise, we define that this word doesn't use capitals in a right way.
 *
 *
 *
 * Example 1:
 *
 *
 * Input: "USA"
 * Output: True
 *
 *
 *
 *
 * Example 2:
 *
 *
 * Input: "FlaG"
 * Output: False
 *
 *
 *
 *
 * Note: The input will be a non-empty word consisting of uppercase and
 * lowercase latin letters.
 *
 */
class Solution {
 public:
  bool detectCapitalUse(string word) {
    // 1. 空字符串是 false
    // 2. 单字符肯定 true
    // 3. w[0] 是大写, w[2]~end 那么后续必须大小写同于 w[1]
    // 4. 全是小写
    if (word.empty()) return false;
    if (word.size() == 1) return true;
    return (isupper(word[0]) && all_of(word.begin() + 2, word.end(),
                                       [&](const char& c) {
                                         return isupper(c) == isupper(word[1]);
                                       })) ||
           (all_of(word.begin() + 1, word.end(), ::islower));
  }
};
