/*
 * @lc app=leetcode id=1636 lang=cpp
 *
 * [1636] Sort Array by Increasing Frequency
 *
 * https://leetcode.com/problems/sort-array-by-increasing-frequency/description/
 *
 * algorithms
 * Easy (66.30%)
 * Total Accepted:    13.6K
 * Total Submissions: 20.4K
 * Testcase Example:  '[1,1,2,2,2,3]'
 *
 * Given an array of integers nums, sort the array in increasing order based on
 * the frequency of the values. If multiple values have the same frequency,
 * sort them in decreasing order.
 * 
 * Return the sorted array.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [1,1,2,2,2,3]
 * Output: [3,1,1,2,2,2]
 * Explanation: '3' has a frequency of 1, '1' has a frequency of 2, and '2' has
 * a frequency of 3.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [2,3,1,3,2]
 * Output: [1,3,3,2,2]
 * Explanation: '2' and '3' both have a frequency of 2, so they are sorted in
 * decreasing order.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: nums = [-1,1,-6,4,5,-6,1,4,1]
 * Output: [5,-1,4,4,-6,-6,1,1,1]
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= nums.length <= 100
 * -100 <= nums[i] <= 100
 * 
 * 
 */
class Solution {
public:
    vector<int> frequencySort(vector<int>& nums) {
        // 1. 构造频率 map
        // 2. 把 map 转换为 pair_vector
        // 3. 按照提议排序 pair_vector
        // 4. 遍历排序后的 pair_vector 重新构造为 vector
        map<int, int> count; // k: value, v: count      

        for (auto n: nums)
            count[n]++;

        vector<pair<int, int>> freq(count.begin(), count.end());
        sort(freq.begin(), freq.end(),
            [](pair<int, int> &x, pair<int, int> &y) {
                return x.second != y.second ? x.second < y.second : x.first > y.first;
            });
        int k = 0;
        for (auto &p : freq)
            while (p.second--)
                nums[k++] = p.first;
        return nums;
    }
};
