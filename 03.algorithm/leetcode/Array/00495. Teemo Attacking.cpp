/*
 * @lc app=leetcode id=495 lang=cpp
 *
 * [495] Teemo Attacking
 *
 * https://leetcode.com/problems/teemo-attacking/description/
 *
 * algorithms
 * Easy (56.76%)
 * Total Accepted:    92.8K
 * Total Submissions: 163.5K
 * Testcase Example:  '[1,4]\n2'
 *
 * Our hero Teemo is attacking an enemy Ashe with poison attacks! When Teemo
 * attacks Ashe, Ashe gets poisoned for a exactly duration seconds. More
 * formally, an attack at second t will mean Ashe is poisoned during the
 * inclusive time interval [t, t + duration - 1]. If Teemo attacks again before
 * the poison effect ends, the timer for it is reset, and the poison effect
 * will end duration seconds after the new attack.
 *
 * You are given a non-decreasing integer array timeSeries, where timeSeries[i]
 * denotes that Teemo attacks Ashe at second timeSeries[i], and an integer
 * duration.
 *
 * Return the total number of seconds that Ashe is poisoned.
 *
 *
 * Example 1:
 *
 *
 * Input: timeSeries = [1,4], duration = 2
 * Output: 4
 * Explanation: Teemo's attacks on Ashe go as follows:
 * - At second 1, Teemo attacks, and Ashe is poisoned for seconds 1 and 2.
 * - At second 4, Teemo attacks, and Ashe is poisoned for seconds 4 and 5.
 * Ashe is poisoned for seconds 1, 2, 4, and 5, which is 4 seconds in total.
 *
 *
 * Example 2:
 *
 *
 * Input: timeSeries = [1,2], duration = 2
 * Output: 3
 * Explanation: Teemo's attacks on Ashe go as follows:
 * - At second 1, Teemo attacks, and Ashe is poisoned for seconds 1 and 2.
 * - At second 2 however, Teemo attacks again and resets the poison timer. Ashe
 * is poisoned for seconds 2 and 3.
 * Ashe is poisoned for seconds 1, 2, and 3, which is 3 seconds in total.
 *
 *
 * Constraints:
 *
 *
 * 1 <= timeSeries.length <= 10^4
 * 0 <= timeSeries[i], duration <= 10^7
 * timeSeries is sorted in non-decreasing order.
 *
 *
 */
class Solution {
public:
    int findPoisonedDuration(vector<int>& timeSeries, int duration) {
        int res = 0;
        /* 1. 处于最后一秒
         * 2. 还没中毒结束 被 重置
         * 3. 持续到中毒结束 */
        for (size_t i = 0; i < timeSeries.size(); ++i) {
            if (i+1 == timeSeries.size() || timeSeries[i]+duration-1 < timeSeries[i+1]) {
                res += duration;
            } else {
                res += timeSeries[i+1] - timeSeries[i];
            }
        }
        return res;
    }
};
