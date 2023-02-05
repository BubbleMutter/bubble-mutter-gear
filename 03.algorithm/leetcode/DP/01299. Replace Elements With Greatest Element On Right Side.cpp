/*
 * @lc app=leetcode id=1299 lang=cpp
 *
 * [1299] Replace Elements with Greatest Element on Right Side
 *
 * https://leetcode.com/problems/replace-elements-with-greatest-element-on-right-side/description/
 *
 * algorithms
 * Easy (74.35%)
 * Total Accepted:    194.5K
 * Total Submissions: 261.4K
 * Testcase Example:  '[17,18,5,4,6,1]'
 *
 * Given an array arr, replace every element in that array with the greatest
 * element among the elements to its right, and replace the last element with
 * -1.
 * 
 * After doing so, return the array.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [17,18,5,4,6,1]
 * Output: [18,6,6,6,1,-1]
 * Explanation: 
 * - index 0 --> the greatest element to the right of index 0 is index 1 (18).
 * - index 1 --> the greatest element to the right of index 1 is index 4 (6).
 * - index 2 --> the greatest element to the right of index 2 is index 4 (6).
 * - index 3 --> the greatest element to the right of index 3 is index 4 (6).
 * - index 4 --> the greatest element to the right of index 4 is index 5 (1).
 * - index 5 --> there are no elements to the right of index 5, so we put -1.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [400]
 * Output: [-1]
 * Explanation: There are no elements to the right of index 0.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= arr.length <= 10^4
 * 1 <= arr[i] <= 10^5
 * 
 * 
 */
class Solution {
public:
    vector<int> replaceElements(vector<int>& arr) {
        // 1. 动态规划; 从右往左遍历; 维护最大值
        // 2. 最后把 末尾元素 设为-1
        int mmax = arr[arr.size()-1];
        for (int i = arr.size()-2; i >= 0; --i) {
            if (arr[i] > mmax) {
                int next = arr[i];
                arr[i] = mmax;
                mmax = next;
            } else {
                arr[i] = mmax;
            }
        }
        arr[arr.size()-1] = -1;
        return arr;
    }
};
