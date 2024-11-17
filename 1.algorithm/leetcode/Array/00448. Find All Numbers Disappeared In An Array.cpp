/*
 * @lc app=leetcode id=448 lang=cpp
 *
 * [448] Find All Numbers Disappeared in an Array
 *
 * https://leetcode.com/problems/find-all-numbers-disappeared-in-an-array/description/
 *
 * algorithms
 * Easy (56.09%)
 * Total Accepted:    326.1K
 * Total Submissions: 581.5K
 * Testcase Example:  '[4,3,2,7,8,2,3,1]'
 *
 * Given an array of integers where 1 ≤ a[i] ≤ n (n = size of array), some
 * elements appear twice and others appear once.
 * 
 * Find all the elements of [1, n] inclusive that do not appear in this array.
 * 
 * Could you do it without extra space and in O(n) runtime? You may assume the
 * returned list does not count as extra space.
 * 
 * Example:
 * 
 * Input:
 * [4,3,2,7,8,2,3,1]
 * 
 * Output:
 * [5,6]
 * 
 * 
 */

class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
#if 1 // 不适用额外空间; [ time = O(n), space = O(1) ]
        // 思路: 把 value 放到对应索引的位置 nums[value-1] = value
        // 遍历整个数组; 有三种情况
        // 1. value 已经放到 正确的位置了
        // 2. value 没有放到正确的位置; 但 正确的位置上 == value
        //    说明 value 是重复的; 遍历下一个元素; i++
        // 3. value 没有放到正确的位置; 正确位置上不是 value
        //    那就把 value 放到正确的位置; (用 swap)
        //    继续遍历当前位置; i 不变
        // 4. 再次遍历nums[]; 如果 位置上的 value 不正确
        //    那么 位置上对应的值 就是消失不见的
        vector<int> answer;
        for (int i = 0; i < nums.size();) {
            if (nums[i] != i+1) {
                if (nums[nums[i]-1] != nums[i])                                 
                    swap(nums[nums[i]-1], nums[i]);
                else
                    i++;
            } else
                i++;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] != i+1)
                answer.push_back(i+1);
        }
        return answer;
#endif
        return ans;
    }
};
// Note: 使用额外空间的版本; 运行效率要快一点