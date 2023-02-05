/*
 * @lc app=leetcode id=1309 lang=cpp
 *
 * [1309] Decrypt String from Alphabet to Integer Mapping
 *
 * https://leetcode.com/problems/decrypt-string-from-alphabet-to-integer-mapping/description/
 *
 * algorithms
 * Easy (78.03%)
 * Total Accepted:    71.7K
 * Total Submissions: 91.4K
 * Testcase Example:  '"10#11#12"'
 *
 * You are given a string s formed by digits and '#'. We want to map s to
 * English lowercase characters as follows:
 *
 *
 * Characters ('a' to 'i') are represented by ('1' to '9') respectively.
 * Characters ('j' to 'z') are represented by ('10#' to '26#') respectively.
 *
 *
 * Return the string formed after mapping.
 *
 * The test cases are generated so that a unique mapping will always exist.
 *
 *
 * Example 1:
 *
 *
 * Input: s = "10#11#12"
 * Output: "jkab"
 * Explanation: "j" -> "10#" , "k" -> "11#" , "a" -> "1" , "b" -> "2".
 *
 *
 * Example 2:
 *
 *
 * Input: s = "1326#"
 * Output: "acz"
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= s.length <= 1000
 * s consists of digits and the '#' letter.
 * s will be a valid string such that mapping is always possible.
 *
 *
 */
class Solution {
public:
    string freqAlphabets(string s) {
        // 1. 贪心算法
        // 2. 检查 i+2 是否为 #
        // 3. 是: 则 substr(i, 2) 转数字
        // 4. 否: s[i]-'1' + 'a' 转单个数字
        string res;
        int i = 0;
        while (i < s.size()) {
            if (i+2 < s.size() && s[i+2] == '#') {
                res += 'a'+std::stoul(s.substr(i, 2))-1;
                i += 3;
            } else {
                res += 'a' + (s[i]-'1');
                ++i;
            }
        }
        return res;
    }
};
