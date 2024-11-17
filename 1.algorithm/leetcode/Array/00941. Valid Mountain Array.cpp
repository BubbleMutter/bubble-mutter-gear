/*
 * @lc app=leetcode id=941 lang=cpp
 *
 * [941] Valid Mountain Array
 *
 * https://leetcode.com/problems/valid-mountain-array/description/
 *
 * algorithms
 * Easy (32.34%)
 * Total Accepted:    273.2K
 * Total Submissions: 805.8K
 * Testcase Example:  '[2,1]'
 *
 * Given an array of integers arr, return true if and only if it is a valid
 * mountain array.
 * 
 * Recall that arr is a mountain array if and only if:
 * 
 * 
 * arr.length >= 3
 * There exists some i with 0 < i < arr.length - 1 such that:
 * 
 * arr[0] < arr[1] < ... < arr[i - 1] < arr[i] 
 * arr[i] > arr[i + 1] > ... > arr[arr.length - 1]
 * 
 * 
 * 
 * 
 * 
 * Example 1:
 * Input: arr = [2,1]
 * Output: false
 * Example 2:
 * Input: arr = [3,5,5]
 * Output: false
 * Example 3:
 * Input: arr = [0,3,2,1]
 * Output: true
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= arr.length <= 10^4
 * 0 <= arr[i] <= 10^4
 * 
 * 
 */
class Solution {
public:
    bool validMountainArray(vector<int>& arr) {
        if (arr.size() < 3) return false;
        int i = 0;
        // 1. 上坡, 先找山峰
        for (; i < arr.size(); ++i) {
            if (i+1 < arr.size() && arr[i] >= arr[i+1]) {
                break;
            }
        }
        // 2. 一直下坡 或者 一直上坡
        if (i == 0 || i == arr.size()) return false;
        // 3. 下坡
        for (; i < arr.size(); ++i) {
            if (i+1 < arr.size() && arr[i] <= arr[i+1]) {
                break;
            }
        }
        // 4. 没有找到第二山峰即满足条件
        return i == arr.size();
    }
};
