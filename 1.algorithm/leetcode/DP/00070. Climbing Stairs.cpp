/*
 * @lc app=leetcode id=70 lang=cpp
 *
 * [70] Climbing Stairs
 *
 * https://leetcode.com/problems/climbing-stairs/description/
 *
 * algorithms
 * Easy (48.58%)
 * Total Accepted:    893.2K
 * Total Submissions: 1.8M
 * Testcase Example:  '2'
 *
 * You are climbing a staircase. It takes n steps to reach the top.
 * 
 * Each time you can either climb 1 or 2 steps. In how many distinct ways can
 * you climb to the top?
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 2
 * Output: 2
 * Explanation: There are two ways to climb to the top.
 * 1. 1 step + 1 step
 * 2. 2 steps
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 3
 * Output: 3
 * Explanation: There are three ways to climb to the top.
 * 1. 1 step + 1 step + 1 step
 * 2. 1 step + 2 steps
 * 3. 2 steps + 1 step
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= n <= 45
 * 
 * 
 */
class Solution {
public:
    int climbStairs(int n) {
        // f(n) 表示走当前阶的走法
        // 要么 f(n-2) 上上阶再走一步
        // 要么 f(n-1) 　上阶再走两步
        // 所以 f(n) = f(n-2) + f(n-1)
        //
        // f(1) = 1;
        // f(2) = 2;

        if (n == 1) return 1;

        // 用斐波那 数列实现
        int memo[2] = { 1, 2 };
        for (int i = 3; i <= n; i++) {
            int curr = memo[0] + memo[1];
            memo[0] = memo[1];
            memo[1] = curr;
        }
        return memo[1];
    }
};
