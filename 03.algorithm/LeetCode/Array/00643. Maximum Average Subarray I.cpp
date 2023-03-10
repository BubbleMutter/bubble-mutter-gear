/*
 * @lc app=leetcode id=643 lang=cpp
 *
 * [643] Maximum Average Subarray I
 *
 * https://leetcode.com/problems/maximum-average-subarray-i/description/
 *
 * algorithms
 * Easy (41.94%)
 * Total Accepted:    91K
 * Total Submissions: 216.9K
 * Testcase Example:  '[1,12,-5,-6,50,3]\n4'
 *
 * Given an array consisting of n integers, find the contiguous subarray of
 * given length k that has the maximum average value. And you need to output
 * the maximum average value.
 *
 * Example 1:
 *
 *
 * Input: [1,12,-5,-6,50,3], k = 4
 * Output: 12.75
 * Explanation: Maximum average is (12-5-6+50)/4 = 51/4 = 12.75
 *
 *
 *
 *
 * Note:
 *
 *
 * 1 <= k <= n <= 30,000.
 * Elements of the given array will be in the range [-10,000, 10,000].
 *
 *
 *
 *
 */
// 滑动窗口法
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int mmax = 0, curr = 0;
        // 1. 先计算前 k 个元素的和; 即第一个窗口
        // 2. 维护窗口; 动态计算当前窗口的值 =
        //    上一个窗口值 + 当前元素值 - 上一个窗口的最左边
        // 3. 根据当前窗口值 更新结构
        for (int i = 0; i < k; ++i)
            curr += nums[i];
        mmax = curr;
        for (int i = k; i < nums.size(); ++i) {
            curr += nums[i] - nums[i-k];
            mmax = max(mmax, curr);
        }

        return 1.0 * mmax / k;
    }
};
