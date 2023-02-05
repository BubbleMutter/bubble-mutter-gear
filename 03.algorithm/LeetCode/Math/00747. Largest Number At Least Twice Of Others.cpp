/*
 * @lc app=leetcode id=747 lang=cpp
 *
 * [747] Largest Number At Least Twice of Others
 *
 * https://leetcode.com/problems/largest-number-at-least-twice-of-others/description/
 *
 * algorithms
 * Easy (44.72%)
 * Total Accepted:    144.9K
 * Total Submissions: 323.4K
 * Testcase Example:  '[3,6,1,0]'
 *
 * You are given an integer array nums where the largest integer is unique.
 *
 * Determine whether the largest element in the array is at least twice as much
 * as every other number in the array. If it is, return the index of the
 * largest element, or return -1 otherwise.
 *
 *
 * Example 1:
 *
 *
 * Input: nums = [3,6,1,0]
 * Output: 1
 * Explanation: 6 is the largest integer.
 * For every other number in the array x, 6 is at least twice as big as x.
 * The index of value 6 is 1, so we return 1.
 *
 *
 * Example 2:
 *
 *
 * Input: nums = [1,2,3,4]
 * Output: -1
 * Explanation: 4 is less than twice the value of 3, so we return -1.
 *
 * Example 3:
 *
 *
 * Input: nums = [1]
 * Output: 0
 * Explanation: 1 is trivially at least twice the value as any other number
 * because there are no other numbers.
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= nums.length <= 50
 * 0 <= nums[i] <= 100
 * The largest element in nums is unique.
 *
 *
 */
class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        // 1. 求得最大两个数值的索引
        // 2. 最大值 >= 第二最大值的两倍
        if (nums.size() == 1) return 0;
        vector<int> max2 { 0, 1 };
        if (nums[0] > nums[1]) swap(max2[0], max2[1]);
        // 动态维护; 参考
        //     00628. Maximum Product Of Three Numbers
        for (size_t i = 1; i < nums.size(); ++i) {
            if (nums[max2[1]] < nums[i]) {
                max2[0] = max2[1];
                max2[1] = i;
            } else if (nums[max2[0]] < nums[i] && nums[i] < nums[max2[1]]) {
                max2[0] = i;
            }
        }
        return nums[max2[1]] >= nums[max2[0]]*2 ? max2[1] : -1;
    }
};
