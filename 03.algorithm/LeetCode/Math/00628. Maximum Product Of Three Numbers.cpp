/*
 * @lc app=leetcode id=628 lang=cpp
 *
 * [628] Maximum Product of Three Numbers
 *
 * https://leetcode.com/problems/maximum-product-of-three-numbers/description/
 *
 * algorithms
 * Easy (46.77%)
 * Total Accepted:    193.6K
 * Total Submissions: 413.9K
 * Testcase Example:  '[1,2,3]'
 *
 * Given an integer array nums, find three numbers whose product is maximum and
 * return the maximum product.
 * 
 * 
 * Example 1:
 * Input: nums = [1,2,3]
 * Output: 6
 * Example 2:
 * Input: nums = [1,2,3,4]
 * Output: 24
 * Example 3:
 * Input: nums = [-1,-2,-3]
 * Output: -6
 * 
 * 
 * Constraints:
 * 
 * 
 * 3 <= nums.length <= 10^4
 * -1000 <= nums[i] <= 1000
 * 
 * 
 */
class Solution {
public:

    int maximumProduct(vector<int>& nums) {
        // 先排序; 取最大的3个数值; 这样慢了; 需要 log2n
        // dp 一次求得最大的3个数值

        auto f = [](int x, int y) { return x*y; };
        if (nums.size() < 4) {
            return accumulate(nums.begin(), nums.end(), 1, f);
        }
        vector<int> max3 { nums[0], nums[1], nums[2] };
        sort(max3.begin(), max3.end());
        vector<int> min2 { max3[0], max3[1] };
        for (int i = 3; i < nums.size(); ++i) {
            if (max3.back() < nums[i]) {
                max3[0] = max3[1];
                max3[1] = max3[2];
                max3[2] = nums[i];
            } else if (max3[1] < nums[i] && nums[i] <= max3[2]) {
                max3[0] = max3[1];
                max3[1] = nums[i];
            } else if (max3[0] < nums[i] && nums[i] <= max3[1]) {
                max3[0] = nums[i];
            }

            if (nums[i] <= min2[0]) {
                min2[1] = min2[0];
                min2[0] = nums[i];
            } else if (min2[0] < nums[i] && nums[i] < min2[1]) {
                min2[1] = nums[i];
            }
        }
        return max(accumulate(max3.begin(), max3.end(), 1, f),
                   accumulate(min2.begin(), min2.end(), 1, f)*max3.back());
        // 动态规划; 找到最大的3个值和最小的2个值 (注意大于等于 和 小于等于的场景)
        // 1. 全是正数or全负数: 最大的三个数的积
        // 2. 最小的两个负数积*最大的一个正数积
        // 上述两种场景求较大值
    }
};
