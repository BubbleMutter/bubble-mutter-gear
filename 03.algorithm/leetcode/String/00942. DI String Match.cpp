/*
 * @lc app=leetcode id=942 lang=cpp
 *
 * [942] DI String Match
 *
 * https://leetcode.com/problems/di-string-match/description/
 *
 * algorithms
 * Easy (75.34%)
 * Total Accepted:    100.9K
 * Total Submissions: 133.8K
 * Testcase Example:  '"IDID"'
 *
 * A permutation perm of n + 1 integers of all the integers in the range [0, n]
 * can be represented as a string s of length n where:
 * 
 * 
 * s[i] == 'I' if perm[i] < perm[i + 1], and
 * s[i] == 'D' if perm[i] > perm[i + 1].
 * 
 * 
 * Given a string s, reconstruct the permutation perm and return it. If there
 * are multiple valid permutations perm, return any of them.
 * 
 * 
 * Example 1:
 * Input: s = "IDID"
 * Output: [0,4,1,3,2]
 * Example 2:
 * Input: s = "III"
 * Output: [0,1,2,3]
 * Example 3:
 * Input: s = "DDI"
 * Output: [3,2,0,1]
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 10^5
 * s[i] is either 'I' or 'D'.
 * 
 * 
 */
class Solution {
public:
    vector<int> diStringMatch(string s) {
        // 已知是全排列;
        // 原始数组 [0,1,2,3,...,n] (n = s.size())
        //           ^           ^
        // 使用双指针法
        // 分别指向原始数组的 首尾
        //
        // 如果 s[i] 是 I, 那么选取 首指针指向的数
        // 如果 s[i] 是 D, 那么选取 尾指针指向的数
        vector<int> res(s.size()+1, 0);
        int head = 0, tail = s.size();
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == 'I') {
                res[i] = head++;
            } else { /* 'D' */
                res[i] = tail--;
            }
        }
        res[s.size()] = head;
        return res;
    }
};
