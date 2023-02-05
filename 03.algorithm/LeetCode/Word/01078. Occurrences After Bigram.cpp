/*
 * @lc app=leetcode id=1078 lang=cpp
 *
 * [1078] Occurrences After Bigram
 *
 * https://leetcode.com/problems/occurrences-after-bigram/description/
 *
 * algorithms
 * Easy (64.31%)
 * Total Accepted:    49.6K
 * Total Submissions: 77.1K
 * Testcase Example:  '"alice is a good girl she is a good student"\n"a"\n"good"'
 *
 * Given two strings first and second, consider occurrences in some text of the
 * form "first second third", where second comes immediately after first, and
 * third comes immediately after second.
 * 
 * Return an array of all the words third for each occurrence of "first second
 * third".
 * 
 * 
 * Example 1:
 * Input: text = "alice is a good girl she is a good student", first = "a",
 * second = "good"
 * Output: ["girl","student"]
 * Example 2:
 * Input: text = "we will we will rock you", first = "we", second = "will"
 * Output: ["we","rock"]
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= text.length <= 1000
 * text consists of lowercase English letters and spaces.
 * All the words in text a separated by a single space.
 * 1 <= first.length, second.length <= 10
 * first and second consist of lowercase English letters.
 * 
 * 
 */
class Solution {
public:
    vector<string> findOcurrences(string text, string first, string second) {
        // NOTE: 禁忌, 使用 找到 first, second 的标记位方法,
        //             a) 不好处理 first second first 的场景,
        //             b) 不好处理 first == second 的场景
        // 1. 窗口法: 维护前两个 word 的窗口 (正常维护即可)
        // 2. 当 前两个 word 等同 first second 那么当前 word 入列结果
        //    无论是否找到 结果, 都把 window 后移
        //
        // 取得第一个词
        auto find_word = [](const string& s, size_t pos) {
            for (size_t i = pos; i < s.size(); ++i) {
                if (i+1 == s.size() || s[i+1] == ' ') {
                    return s.substr(pos, i+1-pos);
                }
            }
            return string("");
        };
        // 构造 substr
        vector<string> res;
        size_t cursor = 0;
        vector<string> window;
        while (cursor < text.size()) {
            string word = find_word(text, cursor);
            if (word.empty()) {
                break;
            }
            if (window.size() < 2) {
                window.push_back(word);
            } else {
                // 满足条件
                if (window[0] == first && window[1] == second) {
                    res.push_back(word);
                }
                // window 后移
                window[0] = window[1];
                window[1] = word;
            }
            cursor += word.size() + 1 /* one space */;
        }
        return res;
    }
};
