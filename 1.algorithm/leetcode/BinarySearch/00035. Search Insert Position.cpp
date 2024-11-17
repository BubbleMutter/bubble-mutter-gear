/*
 * @lc app=leetcode id=35 lang=cpp
 *
 * [35] Search Insert Position
 *
 * https://leetcode.com/problems/search-insert-position/description/
 *
 * algorithms
 * Easy (42.79%)
 * Total Accepted:    771.9K
 * Total Submissions: 1.8M
 * Testcase Example:  '[1,3,5,6]\n5'
 *
 * Given a sorted array of distinct integers and a target value, return the
 * index if the target is found. If not, return the index where it would be if
 * it were inserted in order.
 * 
 * 
 * Example 1:
 * Input: nums = [1,3,5,6], target = 5
 * Output: 2
 * Example 2:
 * Input: nums = [1,3,5,6], target = 2
 * Output: 1
 * Example 3:
 * Input: nums = [1,3,5,6], target = 7
 * Output: 4
 * Example 4:
 * Input: nums = [1,3,5,6], target = 0
 * Output: 0
 * Example 5:
 * Input: nums = [1], target = 0
 * Output: 0
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= nums.length <= 10^4
 * -10^4 <= nums[i] <= 10^4
 * nums contains distinct values sorted in ascending order.
 * -10^4 <= target <= 10^4
 * 
 * 
 */
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        // 二分查找:
        // i = 0, j = nums.size()-1;
        // while (i <= j) m = i + (j-i)/2;
        //
        // find most large but less than target index
        // init: all num are larger than target, index = -1
        int i = 0, j = nums.size()-1, k = -1;
        while (i <= j) {
            int m = i + (j-i)/2;
            if (nums[m] == target) {
                return m;
            } else if (nums[m] < target) {
                k = m;
                i = m+1;
            } else {
                j = m-1;
            }
        }
        return k+1;
    }
};
