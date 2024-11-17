/*
 * @lc app=leetcode id=746 lang=cpp
 *
 * [746] Min Cost Climbing Stairs
 *
 * https://leetcode.com/problems/min-cost-climbing-stairs/description/
 *
 * algorithms
 * Easy (57.42%)
 * Total Accepted:    401.7K
 * Total Submissions: 697.1K
 * Testcase Example:  '[10,15,20]'
 *
 * You are given an integer array cost where cost[i] is the cost of i^th step
 * on a staircase. Once you pay the cost, you can either climb one or two
 * steps.
 *
 * You can either start from the step with index 0, or the step with index 1.
 *
 * Return the minimum cost to reach the top of the floor.
 *
 *
 * Example 1:
 *
 *
 * Input: cost = [10,15,20]
 * Output: 15
 * Explanation: You will start at index 1.
 * - Pay 15 and climb two steps to reach the top.
 * The total cost is 15.
 *
 *
 * Example 2:
 *
 *
 * Input: cost = [1,100,1,1,1,100,1,1,100,1]
 * Output: 6
 * Explanation: You will start at index 0.
 * - Pay 1 and climb two steps to reach index 2.
 * - Pay 1 and climb two steps to reach index 4.
 * - Pay 1 and climb two steps to reach index 6.
 * - Pay 1 and climb one step to reach index 7.
 * - Pay 1 and climb two steps to reach index 9.
 * - Pay 1 and climb one step to reach the top.
 * The total cost is 6.
 *
 *
 *
 * Constraints:
 *
 *
 * 2 <= cost.length <= 1000
 * 0 <= cost[i] <= 999
 *
 *
 */
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        // f(k) 表示如果"踏"这一阶的花费
        // 而"踏"k阶的路线: 要么"踏"k-1阶; 要么"踏"k-2阶
        // 所以 f(k) = min(f(k-1), f(k-1)) + cost[i]
        // 走完k阶的实际花费就是 "踏"k阶 or "踏"k-1阶
        vector<int> take { cost[0], cost[1] };
        for (size_t i = 2; i < cost.size(); ++i) {
            int curr = min(take[0], take[1]) + cost[i];
            take[0] = take[1];
            take[1] = curr;
        }
        return min(take[0], take[1]);
    }
};
