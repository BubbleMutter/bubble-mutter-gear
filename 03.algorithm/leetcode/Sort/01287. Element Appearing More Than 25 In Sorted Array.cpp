/*
 * @lc app=leetcode id=1287 lang=cpp
 *
 * [1287] Element Appearing More Than 25% In Sorted Array
 *
 * https://leetcode.com/problems/element-appearing-more-than-25-in-sorted-array/description/
 *
 * algorithms
 * Easy (59.54%)
 * Total Accepted:    60.1K
 * Total Submissions: 101K
 * Testcase Example:  '[1,2,2,6,6,6,6,7,10]'
 *
 * Given an integer array sorted in non-decreasing order, there is exactly one
 * integer in the array that occurs more than 25% of the time, return that
 * integer.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [1,2,2,6,6,6,6,7,10]
 * Output: 6
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [1,1]
 * Output: 1
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= arr.length <= 10^4
 * 0 <= arr[i] <= 10^5
 * 
 * 
 */
class Solution {
public:
    int findSpecialInteger(vector<int>& arr) {
        // 1. 从 arr[0] ~ arr[长度3/4] 遍历
        // 2. 如果 arr[i + 长度1/4] == arr[i] 即 arr[i] 为结果
        int last = 0, n = arr.size() / 4;
        for (size_t i = 0; i <= arr.size()*3/4; ++i) {
            if (arr[i] == arr[i+arr.size()/4]) {
                return arr[i];
            }
        }
        return arr[0];
    }
};
