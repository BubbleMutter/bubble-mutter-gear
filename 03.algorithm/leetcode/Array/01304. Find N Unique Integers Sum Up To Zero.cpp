/*
 * @lc app=leetcode id=1304 lang=cpp
 *
 * [1304] Find N Unique Integers Sum up to Zero
 *
 * https://leetcode.com/problems/find-n-unique-integers-sum-up-to-zero/description/
 *
 * algorithms
 * Easy (76.86%)
 * Total Accepted:    129.4K
 * Total Submissions: 168.3K
 * Testcase Example:  '5'
 *
 * Given an integer n, return any array containing n unique integers such that
 * they add up to 0.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 5
 * Output: [-7,-1,1,3,4]
 * Explanation: These arrays also are accepted [-5,-1,1,2,3] ,
 * [-3,-1,2,-2,4].
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 3
 * Output: [-1,0,1]
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: n = 1
 * Output: [0]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= n <= 1000
 * 
 * 
 */
class Solution {
public:
    vector<int> sumZero(int n) {
        // 1. -1, +1, -2, +2, ..., -m, +m
        // 2. 如果 n 是奇数, 则最后补一个零
        // 3. note 遍历时, i 应该小于 n-1
        //    从而避免 n = 1 时, res[0] 进入循环
        vector<int> res(n, 0);
        for (size_t i = 0, k = 1; i < n-1; i += 2, k++) {
            res[i] = k;
            res[i+1] = -k;
        }
        return res;
    }
};
