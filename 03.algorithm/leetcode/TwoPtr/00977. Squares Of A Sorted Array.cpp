/*
 * @lc app=leetcode id=977 lang=cpp
 *
 * [977] Squares of a Sorted Array
 *
 * https://leetcode.com/problems/squares-of-a-sorted-array/description/
 *
 * algorithms
 * Easy (71.48%)
 * Total Accepted:    861K
 * Total Submissions: 1.2M
 * Testcase Example:  '[-4,-1,0,3,10]'
 *
 * Given an integer array nums sorted in non-decreasing order, return an array
 * of the squares of each number sorted in non-decreasing order.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [-4,-1,0,3,10]
 * Output: [0,1,9,16,100]
 * Explanation: After squaring, the array becomes [16,1,0,9,100].
 * After sorting, it becomes [0,1,9,16,100].
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [-7,-3,2,3,11]
 * Output: [4,9,9,49,121]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= nums.length <= 10^4
 * -10^4 <= nums[i] <= 10^4
 * nums is sorted in non-decreasing order.
 * 
 * 
 * 
 * Follow up: Squaring each element and sorting the new array is very trivial,
 * could you find an O(n) solution using a different approach?
 */
class Solution {
public:
    vector<int> sortedSquares(vector<int>& nums) {
        // 1. 已排序数组存在负数;
        // 2. 最左边是负数平方最大的; 最右边是正数平方最大的
        // 3. 因此 "平方数组" 每取一个较大的数; 本质就是从原数组左边 or 右边取一个绝对值较大的
        // 4. 双指针法: i 表示左边 j表示右边
        // 5. 如果左边平方更大则选取左边的; 如果右边平方更大则取右边的
        // 6. 优化: 原数组左边平方比右边的 更大唯一的 可能就是 左边是负数且 加上右边仍然是负数
        int i = 0, j = nums.size()-1;
        int k = j;
        vector<int> squares(nums.size());
        while (i <= j) {
#if 0 /* 原始方法 */
            if (abs(nums[i]) < abs(nums[j])){
#else /* 优化: 加法比较 */
            if (nums[i]+nums[j] > 0) {
#endif
                squares[k--] = nums[j]*nums[j];
                --j;
            } else {
                squares[k--] = nums[i]*nums[i];
                ++i;
            }
        }
        return squares;
    }
};
