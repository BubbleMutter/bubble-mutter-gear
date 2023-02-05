/*
 * @lc app=leetcode id=1013 lang=cpp
 *
 * [1013] Partition Array Into Three Parts With Equal Sum
 *
 * https://leetcode.com/problems/partition-array-into-three-parts-with-equal-sum/description/
 *
 * algorithms
 * Easy (49.53%)
 * Total Accepted:    42.4K
 * Total Submissions: 85.7K
 * Testcase Example:  '[0,2,1,-6,6,-7,9,1,2,0,1]'
 *
 * Given an array of integers arr, return true if we can partition the array
 * into three non-empty parts with equal sums.
 * 
 * Formally, we can partition the array if we can find indexes i + 1 < j with
 * (arr[0] + arr[1] + ... + arr[i] == arr[i + 1] + arr[i + 2] + ... + arr[j -
 * 1] == arr[j] + arr[j + 1] + ... + arr[arr.length - 1])
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [0,2,1,-6,6,-7,9,1,2,0,1]
 * Output: true
 * Explanation: 0 + 2 + 1 = -6 + 6 - 7 + 9 + 1 = 2 + 0 + 1
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [0,2,1,-6,6,7,9,-1,2,0,1]
 * Output: false
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: arr = [3,3,6,5,-2,2,5,1,-9,4]
 * Output: true
 * Explanation: 3 + 3 = 6 = 5 - 2 + 2 + 5 + 1 - 9 + 4
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 3 <= arr.length <= 5 * 10^4
 * -10^4 <= arr[i] <= 10^4
 *
 *
 */
class Solution {
public:
    bool canThreePartsEqualSum(vector<int>& arr) {
        unsigned long sum = accumulate(arr.begin(), arr.end(), 0);
        // 1. 计算和
        // 2. 计算窗口部分和; 部分和*3 == 数组和; 则记录边界
        // 3. 当出现两个边界时, 返回
        // 4. 不可以遍历最后一个元素
        bool found_first = false;
        for (size_t i = 0, win = 0; i < arr.size()-1; ++i) {
            win += arr[i];
            if (3*win == sum) {
                if (found_first) { // 第一边界已经找到
                    return true;
                }
                found_first = true;
                win = 0;
            }
        }
        return false;
    }
};
