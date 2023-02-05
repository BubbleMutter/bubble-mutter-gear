/*
 * @lc app=leetcode id=290 lang=cpp
 *
 * [290] Word Pattern
 *
 * https://leetcode.com/problems/word-pattern/description/
 *
 * algorithms
 * Easy (38.30%)
 * Total Accepted:    246.8K
 * Total Submissions: 643.3K
 * Testcase Example:  '"abba"\n"dog cat cat dog"'
 *
 * Given a pattern and a string s, find if s follows the same pattern.
 *
 * Here follow means a full match, such that there is a bijection between a
 * letter in pattern and a non-empty word in s.
 *
 *
 * Example 1:
 *
 *
 * Input: pattern = "abba", s = "dog cat cat dog"
 * Output: true
 *
 *
 * Example 2:
 *
 *
 * Input: pattern = "abba", s = "dog cat cat fish"
 * Output: false
 *
 *
 * Example 3:
 *
 *
 * Input: pattern = "aaaa", s = "dog cat cat dog"
 * Output: false
 *
 *
 * Example 4:
 *
 *
 * Input: pattern = "abba", s = "dog dog dog dog"
 * Output: false
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= pattern.length <= 300
 * pattern contains only lower-case English letters.
 * 1 <= s.length <= 3000
 * s contains only lower-case English letters and spaces ' '.
 * s does not contain any leading or trailing spaces.
 * All the words in s are separated by a single space.
 *
 *
 */
class Solution {
 public:
  bool wordPattern(string pattern, string s) {
    // 1. 解析 s 构造 word 表
    // 2. word 表 和 pattern 长度不同 return false
    // 3. 遍历 word表 和 pattern 构造相互映射
    // 4. 一旦不满足映射; 则 return false
    map<char, string> p2w;
    map<string, char> w2p;
    vector<string> words;
    for (int i = 0, head = 0; i < s.size(); i++) {
      if (i + 1 == s.size() || s[i + 1] == ' ') {
        words.push_back(move(string(s.begin() + head, s.begin() + i + 1)));
        head = i + 2;  // 不能 ++; 要+2; 干掉空格
      }
    }
    if (words.size() != pattern.size()) {
      return false;
    }
    for (size_t i = 0; i < words.size(); ++i) {
      if (p2w.find(pattern[i]) == p2w.end()) {  // first occurs
        p2w[pattern[i]] = words[i];
      }
      if (w2p.find(words[i]) == w2p.end()) {
        w2p[words[i]] = pattern[i];
      }
      if (w2p[words[i]] != pattern[i] || p2w[pattern[i]] != words[i]) {
        return false;
      }
    }
    return true;
  }
};
