/*
 * @lc app=leetcode id=997 lang=cpp
 *
 * [997] Find the Town Judge
 *
 * https://leetcode.com/problems/find-the-town-judge/description/
 *
 * algorithms
 * Easy (50.07%)
 * Total Accepted:    258.3K
 * Total Submissions: 516.6K
 * Testcase Example:  '2\n[[1,2]]'
 *
 * In a town, there are n people labeled from 1 to n. There is a rumor that one
 * of these people is secretly the town judge.
 * 
 * If the town judge exists, then:
 * 
 * 
 * The town judge trusts nobody.
 * Everybody (except for the town judge) trusts the town judge.
 * There is exactly one person that satisfies properties 1 and 2.
 * 
 * 
 * You are given an array trust where trust[i] = [ai, bi] representing that the
 * person labeled ai trusts the person labeled bi.
 * 
 * Return the label of the town judge if the town judge exists and can be
 * identified, or return -1 otherwise.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 2, trust = [[1,2]]
 * Output: 2
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 3, trust = [[1,3],[2,3]]
 * Output: 3
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: n = 3, trust = [[1,3],[2,3],[3,1]]
 * Output: -1
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= n <= 1000
 * 0 <= trust.length <= 10^4
 * trust[i].length == 2
 * All the pairs of trust are unique.
 * ai != bi
 * 1 <= ai, bi <= n
 * 
 * 
 */
class Solution {
public:
    int findJudge(int n, vector<vector<int>>& trust) {
        // 竞选法官
        map<int, int> candicates;
        // 投票
        for (const auto& p: trust)
            candicates[p[1]]++;
        // 检票; 获得所有人投票的, 且只有一个人 则为法官
        // 题意: 1) ai != bi 不能自投
        //       2) (ai, bi) unique 不能刷票
        vector<int> judge;
        for (const auto& p: candicates) {
            if (p.second == n-1) {
                if (judge.empty()) {
                    judge.push_back(p.first);
                } else { // 两个法官
                    return -1;
                }
            }
        }
        // 没有竞选人
        // 则判断村里是否只有一个人, 自己当法官
        if (judge.size() != 1) return n == 1 ? 1 : -1;
        // 法官不能相信别人
        if (any_of(trust.begin(), trust.end(), [&](const vector<int> p) {
                    return p[0] == judge[0];
                    })) {
            return -1;
        }
        return judge[0];
    }
};
