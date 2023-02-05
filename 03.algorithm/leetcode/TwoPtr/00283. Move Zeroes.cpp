/*
 * @lc app=leetcode id=283 lang=cpp
 *
 * [283] Move Zeroes
 *
 * https://leetcode.com/problems/move-zeroes/description/
 *
 * algorithms
 * Easy (58.50%)
 * Total Accepted:    1.1M
 * Total Submissions: 1.8M
 * Testcase Example:  '[0,1,0,3,12]'
 *
 * Given an array nums, write a function to move all 0's to the end of it while
 * maintaining the relative order of the non-zero elements.
 * 
 * Example:
 * 
 * 
 * Input: [0,1,0,3,12]
 * Output: [1,3,12,0,0]
 * 
 * Note:
 * 
 * 
 * You must do this in-place without making a copy of the array.
 * Minimize the total number of operations.
 * 
 */
class Solution {
public:
    vector<int> moveZeroes(vector<int>& nums) {
        // 双指针窗口法; 维护 0 窗口
        // [0, zero)  非零
        // [zero, i)  零
        // [i, size)  unknown
        for (int i = 0, zero = 0; i < nums.size(); ++i) {
            if (nums[i] != 0)
                swap(nums[zero++], nums[i]);
        }
        return nums;
    }
};
