/*
 * @lc app=leetcode id=1189 lang=cpp
 *
 * [1189] Maximum Number of Balloons
 *
 * https://leetcode.com/problems/maximum-number-of-balloons/description/
 *
 * algorithms
 * Easy (62.54%)
 * Total Accepted:    107.7K
 * Total Submissions: 172.1K
 * Testcase Example:  '"nlaebolko"'
 *
 * Given a string text, you want to use the characters of text to form as many
 * instances of the word "balloon" as possible.
 * 
 * You can use each character in text at most once. Return the maximum number
 * of instances that can be formed.
 * 
 * 
 * Example 1:
 * 
 * 
 * 
 * 
 * Input: text = "nlaebolko"
 * Output: 1
 * 
 * 
 * Example 2:
 * 
 * 
 * 
 * 
 * Input: text = "loonbalxballpoon"
 * Output: 2
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: text = "leetcode"
 * Output: 0
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= text.length <= 10^4
 * text consists of lower case English letters only.
 * 
 * 
 */
class Solution {
public:
    int maxNumberOfBalloons(string text) {
        // hash 法记录字符出现频次
        vector<int> alpha(26, 0);
        for (const auto c: text) {
            alpha[c-'a']++;
        }
        // balloon
        int count = text.size();
        count = min(alpha['a'-'a'], count);
        count = min(alpha['b'-'a'], count);
        count = min(alpha['l'-'a']/2, count);
        count = min(alpha['o'-'a']/2, count);
        count = min(alpha['n'-'a'], count);
        return count;
    }
};
