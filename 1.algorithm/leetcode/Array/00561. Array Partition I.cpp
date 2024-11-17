/*
 * @lc app=leetcode id=561 lang=cpp
 *
 * [561] Array Partition I
 *
 * https://leetcode.com/problems/array-partition-i/description/
 *
 * algorithms
 * Easy (72.76%)
 * Total Accepted:    273.3K
 * Total Submissions: 369.4K
 * Testcase Example:  '[1,4,3,2]'
 *
 * Given an integer array nums of 2n integers, group these integers into n
 * pairs (a1, b1), (a2, b2), ..., (an, bn) such that the sum of min(ai, bi) for
 * all i is maximized. Return the maximized sum.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [1,4,3,2]
 * Output: 4
 * Explanation: All possible pairings (ignoring the ordering of elements) are:
 * 1. (1, 4), (2, 3) -> min(1, 4) + min(2, 3) = 1 + 2 = 3
 * 2. (1, 3), (2, 4) -> min(1, 3) + min(2, 4) = 1 + 2 = 3
 * 3. (1, 2), (3, 4) -> min(1, 2) + min(3, 4) = 1 + 3 = 4
 * So the maximum possible sum is 4.
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [6,2,6,5,1,2]
 * Output: 9
 * Explanation: The optimal pairing is (2, 1), (2, 5), (6, 6). min(2, 1) +
 * min(2, 5) + min(6, 6) = 1 + 2 + 6 = 9.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= n <= 10^4
 * nums.length == 2 * n
 * -10^4 <= nums[i] <= 10^4
 * 
 * 
 */
// 如题; 要让 min(a, b) 总和最大; 显然是要把 (a,b) 的差最小
// 因为 b 超出 a 的值会被 a 吞掉
// 先排序; 两两配对即可; 然后直接选较少的; 即 [0], [2] [4] 位置的
class Solution {
public:
    int arrayPairSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int answer = 0;
        for (int i = 0; i < nums.size(); i += 2) {
            answer += nums[i];
        }
        return answer;
    }
};