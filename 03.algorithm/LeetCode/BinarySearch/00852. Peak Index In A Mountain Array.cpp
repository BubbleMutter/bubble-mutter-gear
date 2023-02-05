/*
 * @lc app=leetcode id=852 lang=cpp
 *
 * [852] Peak Index in a Mountain Array
 *
 * https://leetcode.com/problems/peak-index-in-a-mountain-array/description/
 *
 * algorithms
 * Easy (71.23%)
 * Total Accepted:    316.8K
 * Total Submissions: 445.2K
 * Testcase Example:  '[0,1,0]'
 *
 * Let's call an array arr a mountain if the following properties hold:
 *
 *
 * arr.length >= 3
 * There exists some i with 0 < i < arr.length - 1 such that:
 *
 * arr[0] < arr[1] < ... arr[i-1] < arr[i] 
 * arr[i] > arr[i+1] > ... > arr[arr.length - 1]
 *
 *
 *
 *
 * Given an integer array arr that is guaranteed to be a mountain, return any i
 * such that arr[0] < arr[1] < ... arr[i - 1] < arr[i] > arr[i + 1] > ... >
 * arr[arr.length - 1].
 *
 *
 * Example 1:
 *
 *
 * Input: arr = [0,1,0]
 * Output: 1
 *
 *
 * Example 2:
 *
 *
 * Input: arr = [0,2,1,0]
 * Output: 1
 *
 *
 * Example 3:
 *
 *
 * Input: arr = [0,10,5,2]
 * Output: 1
 *
 *
 *
 * Constraints:
 *
 *
 * 3 <= arr.length <= 10^4
 * 0 <= arr[i] <= 10^6
 * arr is guaranteed to be a mountain array.
 *
 *
 *
 * Follow up: Finding the O(n) is straightforward, could you find an O(log(n))
 * solution?
 */

#if 1
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        int l = 0, r = arr.size() - 1;
        while (l < r) {
            int m = l + (r-l)/2;
            if (arr[m] > arr[m+1]) {
                r = m;    // m 比后面的要高; 顶峰可能是 m or 它的前面
            } else {
                l = m+1;  // m 比后面的要低; 顶峰一定在 m 后面
            }
        }
        return l;
    }
};
#endif

#if 0 /* 另一种做法 */
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        int l = 0, r = arr.size()-1;
        int res = 0;
        while (l <= r) {
            // 取中间
            int m = l + (r-l)/2;
            printf("l=[%d] r=[%d] m=[%d]\n", l, r, m);
            // 更新结果
            if (arr[res] < arr[m]) {
                res = m;
            }
            // 逼近
            if (m == 0 || arr[m-1] < arr[m]) {
                l = m+1;
            } else {
                r = m-1;
            }
        }
        return res;
    }
};
#endif
