/*
 * @lc app=leetcode id=645 lang=cpp
 *
 * [645] Set Mismatch
 *
 * https://leetcode.com/problems/set-mismatch/description/
 *
 * algorithms
 * Easy (41.17%)
 * Total Accepted:    156.7K
 * Total Submissions: 380.3K
 * Testcase Example:  '[1,2,2,4]'
 *
 * You have a set of integers s, which originally contains all the numbers from
 * 1 to n. Unfortunately, due to some error, one of the numbers in s got
 * duplicated to another number in the set, which results in repetition of one
 * number and loss of another number.
 *
 * You are given an integer array nums representing the data status of this set
 * after the error.
 *
 * Find the number that occurs twice and the number that is missing and return
 * them in the form of an array.
 *
 *
 * Example 1:
 * Input: nums = [1,2,2,4]
 * Output: [2,3]
 * Example 2:
 * Input: nums = [1,1]
 * Output: [1,2]
 *
 *
 * Constraints:
 *
 *
 * 2 <= nums.length <= 10^4
 * 1 <= nums[i] <= 10^4
 *
 *
 */
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        vector<int> res(2, -1);
        /* 索引位置重排法 */
        for (size_t i = 0; i < nums.size(); i++) {
            while (nums[i]-1 != i && nums[i] != nums[nums[i]-1]) {
                swap(nums[i], nums[nums[i]-1]);
            }
        }
        /* 遍历取得结果 */
        for (size_t i = 0; i < nums.size(); ++i) {
            if (nums[i]-1 != i) {
                res[0] = nums[i]; /* number occur twice */
                res[1] = i+1;     /* number is missing */
                break;
            }
        }
        return res;
    }
};
