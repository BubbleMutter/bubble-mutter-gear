/*
 * @lc app=leetcode id=1089 lang=cpp
 *
 * [1089] Duplicate Zeros
 *
 * https://leetcode.com/problems/duplicate-zeros/description/
 *
 * algorithms
 * Easy (51.09%)
 * Total Accepted:    247K
 * Total Submissions: 483.1K
 * Testcase Example:  '[1,0,2,3,0,4,5,0]'
 *
 * Given a fixed-length integer array arr, duplicate each occurrence of zero,
 * shifting the remaining elements to the right.
 * 
 * Note that elements beyond the length of the original array are not written.
 * Do the above modifications to the input array in place and do not return
 * anything.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [1,0,2,3,0,4,5,0]
 * Output: [1,0,0,2,3,0,0,4]
 * Explanation: After calling your function, the input array is modified to:
 * [1,0,0,2,3,0,0,4]
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [1,2,3]
 * Output: [1,2,3]
 * Explanation: After calling your function, the input array is modified to:
 * [1,2,3]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= arr.length <= 10^4
 * 0 <= arr[i] <= 9
 * 
 * 
 */
class Solution {
public:
    void duplicateZeros(vector<int>& arr) {
        for (size_t i = 0; i < arr.size()-1; ++i) {
            if (arr[i] != 0) continue;
            for (size_t j = arr.size()-1; j > i; --j) {
                arr[j] = arr[j-1];
            }
            if (i+1 < arr.size()) {
                ++i;
                arr[i] = 0;
            }
        }
    }
};
