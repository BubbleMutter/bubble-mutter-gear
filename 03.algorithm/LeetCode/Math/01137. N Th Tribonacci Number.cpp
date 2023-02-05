/*
 * @lc app=leetcode id=1137 lang=cpp
 *
 * [1137] N-th Tribonacci Number
 *
 * https://leetcode.com/problems/n-th-tribonacci-number/description/
 *
 * algorithms
 * Easy (62.15%)
 * Total Accepted:    234K
 * Total Submissions: 373.9K
 * Testcase Example:  '4'
 *
 * The Tribonacci sequence Tn is defined as follows: 
 * 
 * T0 = 0, T1 = 1, T2 = 1, and Tn+3 = Tn + Tn+1 + Tn+2 for n >= 0.
 * 
 * Given n, return the value of Tn.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 4
 * Output: 4
 * Explanation:
 * T_3 = 0 + 1 + 1 = 2
 * T_4 = 1 + 1 + 2 = 4
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 25
 * Output: 1389537
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= n <= 37
 * The answer is guaranteed to fit within a 32-bit integer, ie. answer <= 2^31
 * - 1.
 * 
 */
class Solution {
public:
    int tribonacci(int n) {
        // 经典 00509. Fibonacci Number.cpp 的拓展
        vector<int> memo { 0, 1, 1, };
        if (n < 3) return memo[n];
        for (int i = 3; i <= n; ++i) {
            int next = accumulate(memo.begin(), memo.end(), 0);
            memo[0] = memo[1];
            memo[1] = memo[2];
            memo[2] = next;
        }
        return memo.back();
    }
};
