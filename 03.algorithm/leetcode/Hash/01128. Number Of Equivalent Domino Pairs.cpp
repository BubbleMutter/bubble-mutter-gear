/*
 * @lc app=leetcode id=1128 lang=cpp
 *
 * [1128] Number of Equivalent Domino Pairs
 *
 * https://leetcode.com/problems/number-of-equivalent-domino-pairs/description/
 *
 * algorithms
 * Easy (46.06%)
 * Total Accepted:    45.3K
 * Total Submissions: 97.9K
 * Testcase Example:  '[[1,2],[2,1],[3,4],[5,6]]'
 *
 * Given a list of dominoes, dominoes[i] = [a, b] is equivalent to dominoes[j]
 * = [c, d] if and only if either (a == c and b == d), or (a == d and b == c) -
 * that is, one domino can be rotated to be equal to another domino.
 *
 * Return the number of pairs (i, j) for which 0 <= i < j < dominoes.length,
 * and dominoes[i] is equivalent to dominoes[j].
 *
 *
 * Example 1:
 *
 *
 * Input: dominoes = [[1,2],[2,1],[3,4],[5,6]]
 * Output: 1
 *
 *
 * Example 2:
 *
 *
 * Input: dominoes = [[1,2],[1,2],[1,1],[1,2],[2,2]]
 * Output: 3
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= dominoes.length <= 4 * 10^4
 * dominoes[i].length == 2
 * 1 <= dominoes[i][j] <= 9
 *
 *
 */
class Solution {
public:
    int numEquivDominoPairs(vector<vector<int>>& dominoes) {
        // 1. 把每个 dominoe 排序
        // 2. 把每个 dominoe 放到 hash 中
        // 3. 相同 dominoe 的个数按照组合公式计算 表示 pair 数
        // 4. 优化: 不需要重新遍历 hash
        map<vector<int>, int> hash;
        size_t count = 0;
        for (size_t i = 0; i < dominoes.size(); ++i) {
            if (dominoes[i][0] > dominoes[i][1]) {
                swap(dominoes[i][0], dominoes[i][1]);
            }
            // 4. 优化
            //    当 1个时, pair 数为 0
            //    当 2个时, pair 数为 1, delta = 1
            //    当 3个时, pair 数为 3, delta = 2
            //    当 4个时, pair 数为 6, delta = 3
            //    每增加一个相同 dominoe; 则 pair 数增加 n-1;
            count += hash[dominoes[i]]++;
        }
#if 0 // 3. 相同 dominoe 的个数按照组合公式计算 表示 pair 数
        for (const auto& p: hash) {
            count += p.second * (p.second - 1) / 2;
        }
#endif
        return count;
    }
};
