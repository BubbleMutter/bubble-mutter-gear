/*
 * @lc app=leetcode id=500 lang=cpp
 *
 * [500] Keyboard Row
 *
 * https://leetcode.com/problems/keyboard-row/description/
 *
 * algorithms
 * Easy (67.48%)
 * Total Accepted:    144.5K
 * Total Submissions: 213.9K
 * Testcase Example:  '["Hello","Alaska","Dad","Peace"]'
 *
 * Given an array of strings words, return the words that can be typed using
 * letters of the alphabet on only one row of American keyboard like the image
 * below.
 * 
 * In the American keyboard:
 * 
 * 
 * the first row consists of the characters "qwertyuiop",
 * the second row consists of the characters "asdfghjkl", and
 * the third row consists of the characters "zxcvbnm".
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: words = ["Hello","Alaska","Dad","Peace"]
 * Output: ["Alaska","Dad"]
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: words = ["omk"]
 * Output: []
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: words = ["adsdf","sfd"]
 * Output: ["adsdf","sfd"]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= words.length <= 20
 * 1 <= words[i].length <= 100
 * words[i] consists of English letters (both lowercase and uppercase). 
 * 
 * 
 */
class Solution {
public:
    vector<string> findWords(vector<string>& words) {
        static vector<set<char>> keyboard {
            { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
              'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' },
            { 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
              'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L' },
            { 'z', 'x', 'c', 'v', 'b', 'n', 'm',
              'Z', 'X', 'C', 'V', 'B', 'N', 'M' },
        };
        vector<string> res;
        for (const auto& word: words) {
            if (any_of(keyboard.begin(), keyboard.end(), [&](const set<char>& row) {
                        return all_of(word.begin(), word.end(), [&](const char& c) {
                                return row.count(c) > 0;
                                });
                        })) {
                res.push_back(word);
            }
        }
        return res;
    }
};
