/*
 * @lc app=leetcode id=365 lang=cpp
 *
 * [365] Water and Jug Problem
 *
 * https://leetcode.com/problems/water-and-jug-problem/description/
 *
 * algorithms
 * Medium (33.45%)
 * Total Accepted:    58.6K
 * Total Submissions: 173.6K
 * Testcase Example:  '3\n5\n4'
 *
 * You are given two jugs with capacities jug1Capacity and jug2Capacity liters.
 * There is an infinite amount of water supply available. Determine whether it
 * is possible to measure exactly targetCapacity liters using these two jugs.
 *
 * If targetCapacity liters of water are measurable, you must have
 * targetCapacity liters of water contained within one or both buckets by the
 * end.
 * 
 * Operations allowed:
 * 
 * 
 * Fill any of the jugs with water.
 * Empty any of the jugs.
 * Pour water from one jug into another till the other jug is completely full,
 * or the first jug itself is empty.
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: jug1Capacity = 3, jug2Capacity = 5, targetCapacity = 4
 * Output: true
 * Explanation: The famous Die Hard example 
 *
 *
 * Example 2:
 *
 *
 * Input: jug1Capacity = 2, jug2Capacity = 6, targetCapacity = 5
 * Output: false
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: jug1Capacity = 1, jug2Capacity = 2, targetCapacity = 3
 * Output: true
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= jug1Capacity, jug2Capacity, targetCapacity <= 10^6
 * 
 * 
 */
class Solution {
public:
    int fnMaxCommonDivisor(const int x, const int y) {
        /* x 必须由外层保证 x > y */
        int remain = x % y;
        return remain == 0 ? y : fnMaxCommonDivisor(y, remain);
    }
    bool canMeasureWater(int jug1Capacity, int jug2Capacity, int targetCapacity) {
        // 1. 递归求最大公因数
        // 2. jug1 和 jug2 的最大公因数 可以被 targetCapacity 整除
        // 3. 还需要 jug1 和 jug2 的和大于等于 targetCapacity
        if (jug1Capacity < jug2Capacity) {
            swap(jug1Capacity, jug2Capacity);
        }
        return targetCapacity % (fnMaxCommonDivisor(jug1Capacity, jug2Capacity)) == 0 && jug1Capacity+jug2Capacity >= targetCapacity;
    }
};
