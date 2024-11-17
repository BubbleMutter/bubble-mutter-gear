/*
 * @lc app=leetcode id=1018 lang=cpp
 *
 * [1018] Binary Prefix Divisible By 5
 *
 * https://leetcode.com/problems/binary-prefix-divisible-by-5/description/
 *
 * algorithms
 * Easy (47.60%)
 * Total Accepted:    37.5K
 * Total Submissions: 78.9K
 * Testcase Example:  '[0,1,1]'
 *
 * You are given a binary array nums (0-indexed).
 * 
 * We define xi as the number whose binary representation is the subarray
 * nums[0..i] (from most-significant-bit to least-significant-bit).
 * 
 * 
 * For example, if nums = [1,0,1], then x0 = 1, x1 = 2, and x2 = 5.
 * 
 * 
 * Return an array of booleans answer where answer[i] is true if xi is
 * divisible by 5.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [0,1,1]
 * Output: [true,false,false]
 * Explanation: The input numbers in binary are 0, 01, 011; which are 0, 1, and
 * 3 in base-10.
 * Only the first number is divisible by 5, so answer[0] is true.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [1,1,1]
 * Output: [false,false,false]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= nums.length <= 10^5
 * nums[i] is either 0 or 1.
 * 
 * 
 */
class Solution {
public:
    vector<bool> prefixesDivBy5(vector<int>& nums) {
        unsigned x = 0;
        vector<bool> res(nums.size());
        for (size_t i = 0; i < nums.size(); ++i) {
            // 1. 因为 nums.length 远大于 uint64_t 的 bits
            // 2. 所以 不能直接转化为数值 再取模
            // 3. 实际上 可以只保留前一个数的模
            // 4. 那么 当前数值能否被5整除的 实际上是之前数值的模*2  累加上当前位
            //    而且当前位 是 1 或 0, 直接用 bit-or 即可
            //    然后保留当前数的模
            x = ((x << 1) | nums[i]) % 5;
            if (x) {
                res[i] = false;
            } else {
                res[i] = true;
            }
        }
        return res;
    }
};
