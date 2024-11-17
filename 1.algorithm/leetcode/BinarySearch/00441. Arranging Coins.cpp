/*
 * @lc app=leetcode id=441 lang=cpp
 *
 * [441] Arranging Coins
 *
 * https://leetcode.com/problems/arranging-coins/description/
 *
 * algorithms
 * Easy (44.89%)
 * Total Accepted:    256.8K
 * Total Submissions: 571.7K
 * Testcase Example:  '5'
 *
 * You have n coins and you want to build a staircase with these coins. The
 * staircase consists of k rows where the i^th row has exactly i coins. The
 * last row of the staircase may be incomplete.
 * 
 * Given the integer n, return the number of complete rows of the staircase you
 * will build.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 5
 * Output: 2
 * Explanation: Because the 3^rd row is incomplete, we return 2.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 8
 * Output: 3
 * Explanation: Because the 4^th row is incomplete, we return 3.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= n <= 2^31 - 1
 * 
 * 
 */
class Solution {
public:
    int arrangeCoins(int n) {
        uint64_t l = 0, r = n, res = 0;
        /**
         * 因为, m*(m+1)/2  <= n; 是允许闭合区间的
         * 所以, l <= r 也得遍历 相等的场景
         */
        while (l <= r) {
            uint64_t m = (l + r)/2;
            if (m*(m+1)/2 <= n) {
                res = m;
                l = m+1;
            } else {
                r = m-1;
            }
        }
        return res;
    }
};
