/*
 * @lc app=leetcode id=594 lang=cpp
 *
 * [594] Longest Harmonious Subsequence
 *
 * https://leetcode.com/problems/longest-harmonious-subsequence/description/
 *
 * algorithms
 * Easy (52.36%)
 * Total Accepted:    110.9K
 * Total Submissions: 211.7K
 * Testcase Example:  '[1,3,2,2,5,2,3,7]'
 *
 * We define a harmonious array as an array where the difference between its
 * maximum value and its minimum value is exactly 1.
 * 
 * Given an integer array nums, return the length of its longest harmonious
 * subsequence among all its possible subsequences.
 * 
 * A subsequence of array is a sequence that can be derived from the array by
 * deleting some or no elements without changing the order of the remaining
 * elements.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [1,3,2,2,5,2,3,7]
 * Output: 5
 * Explanation: The longest harmonious subsequence is [3,2,2,2,3].
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [1,2,3,4]
 * Output: 2
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: nums = [1,1,1,1]
 * Output: 0
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= nums.length <= 2 * 10^4
 * -10^9 <= nums[i] <= 10^9
 * 
 */
class Solution {
public:
    int findLHS(vector<int>& nums) {
        map<int, int> count;
        // 计算相同数值的个数
        for (const auto& n: nums) {
            count[n]++;
        }
        int res = 0;
        // 求 "数值+1的个数" 与 "数值的个数" 的 加和
        // 并求较大值
        // NOTE: map 本身是升序的; 底层是红黑树
        for (const auto &[val, num]: count) {
            const auto iter = count.find(val+1);
            if (iter != count.end()) {
                res = max(res, iter->second + num);
            }
        }
        return res;
    }
};
