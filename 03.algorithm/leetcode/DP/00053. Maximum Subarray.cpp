/*
 * @lc app=leetcode id=53 lang=cpp
 *
 * [53] Maximum Subarray
 *
 * https://leetcode.com/problems/maximum-subarray/description/
 *
 * algorithms
 * Easy (47.64%)
 * Total Accepted:    1.3M
 * Total Submissions: 2.8M
 * Testcase Example:  '[-2,1,-3,4,-1,2,1,-5,4]'
 *
 * Given an integer array nums, find the contiguous subarray (containing at
 * least one number) which has the largest sum and return its sum.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [-2,1,-3,4,-1,2,1,-5,4]
 * Output: 6
 * Explanation: [4,-1,2,1] has the largest sum = 6.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [1]
 * Output: 1
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: nums = [0]
 * Output: 0
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: nums = [-1]
 * Output: -1
 * 
 * 
 * Example 5:
 * 
 * 
 * Input: nums = [-100000]
 * Output: -100000
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= nums.length <= 3 * 10^4
 * -10^5 <= nums[i] <= 10^5
 * 
 * 
 * 
 * Follow up: If you have figured out the O(n) solution, try coding another
 * solution using the divide and conquer approach, which is more subtle.
 */
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
#if 0
        // 1. 求前缀和数组
        // 2. 遍历前缀和数组; 记录遍历过程中 最小的前缀和
        // 3. 针对每个前缀和元素
        //    a) 如果最小前缀和 < 0; 那么 候选为 当前前缀和 - 记录的最小前缀和
        //    b) 否则 候选就是 当前前缀和
        // 4. 拿到"候选" 更新答案

        vector<int> sums(nums.size(), 0);
        for (int i = 0, sum = 0; i < nums.size(); i++) {
            sum += nums[i];
            sums[i] = sum;
        }

        int answer = sums[0];
        for (int i = 0, mmin = 0; i < sums.size(); i++) {
            if (mmin < 0)
                answer = max(answer, sums[i] - mmin);
            else
                answer = max(answer, sums[i]);
            mmin = min(mmin, sums[i]);
        }
#endif

 #if 1 // 题解: 优化
        // 1. 针对每一个数值
        // 2. 该数值 子数组 和为负;
        //    那么新 子数组 从当前数值重新开始
        // 3. 该数值 子数组 和非负;
        //    则加入 子数组
        // 4. 在局部 子数组 更新后; 更新结果
        //
        // Note: 实际上 subarray 是一个移动窗口

        int answer = nums[0];
        for (int i = 0, window = 0; i < nums.size(); ++i) {
            if (window < 0)
                window = nums[i];
            else
                window += nums[i];
            answer = max(answer, window);
        }
#endif
        return answer;
    }
};
