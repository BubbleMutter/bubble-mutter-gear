/*
 * @lc app=leetcode id=830 lang=cpp
 *
 * [830] Positions of Large Groups
 *
 * https://leetcode.com/problems/positions-of-large-groups/description/
 *
 * algorithms
 * Easy (50.17%)
 * Total Accepted:    51.6K
 * Total Submissions: 102.8K
 * Testcase Example:  '"abbxxxxzzy"'
 *
 * In a string s of lowercase letters, these letters form consecutive groups of
 * the same character.
 *
 * For example, a string like s = "abbxxxxzyy" has the groups "a", "bb",
 * "xxxx", "z", and "yy".
 *
 * A group is identified by an interval [start, end], where start and end
 * denote the start and end indices (inclusive) of the group. In the above
 * example, "xxxx" has the interval [3,6].
 *
 * A group is considered large if it has 3 or more characters.
 *
 * Return the intervals of every large group sorted in increasing order by
 * start index.
 *
 *
 * Example 1:
 *
 *
 * Input: s = "abbxxxxzzy"
 * Output: [[3,6]]
 * Explanation: "xxxx" is the only large group with start index 3 and end index
 * 6.
 *
 *
 * Example 2:
 *
 *
 * Input: s = "abc"
 * Output: []
 * Explanation: We have groups "a", "b", and "c", none of which are large
 * groups.
 *
 *
 * Example 3:
 *
 *
 * Input: s = "abcdddeeeeaabbbcd"
 * Output: [[3,5],[6,9],[12,14]]
 * Explanation: The large groups are "ddd", "eeee", and "bbb".
 *
 *
 * Example 4:
 * 
 * 
 * Input: s = "aba"
 * Output: []
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 1000
 * s contains lower-case English letters only.
 * 
 * 
 */
class Solution {
public:
    vector<vector<int>> largeGroupPositions(string s) {
        // 1. 本质上是取词操作, 连续相同的字母作为词语
        // 2. 取得词语后, 根据长度 > 3 作为条件
        vector<vector<int>> res;
        int k = 0;
        for (int i = 0; i < s.size(); ++i) {
            if (i+1 == s.size() || s[i] != s[i+1]) {
                if (i - k >= 2) {
                    vector<int> group(2);
                    group[0] = k;
                    group[1] = i;
                    res.push_back(std::move(group));
                }
                // 新的组, 的开始字符
                k = i+1;
            }
        }
        return res;
    }
};
