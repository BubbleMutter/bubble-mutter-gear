/*
 * @lc app=leetcode id=167 lang=cpp
 *
 * [167] Two Sum II - Input array is sorted
 *
 * https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/description/
 *
 * algorithms
 * Easy (55.52%)
 * Total Accepted:    529.1K
 * Total Submissions: 951K
 * Testcase Example:  '[2,7,11,15]\n9'
 *
 * Given an array of integers numbers that is already sorted in ascending
 * order, find two numbers such that they add up to a specific target number.
 * 
 * Return the indices of the two numbers (1-indexed) as an integer array answer
 * of size 2, where 1 <= answer[0] < answer[1] <= numbers.length.
 * 
 * You may assume that each input would have exactly one solution and you may
 * not use the same element twice.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: numbers = [2,7,11,15], target = 9
 * Output: [1,2]
 * Explanation: The sum of 2 and 7 is 9. Therefore index1 = 1, index2 = 2.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: numbers = [2,3,4], target = 6
 * Output: [1,3]
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: numbers = [-1,0], target = -1
 * Output: [1,2]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 2 <= numbers.length <= 3 * 10^4
 * -1000 <= numbers[i] <= 1000
 * numbers is sorted in increasing order.
 * -1000 <= target <= 1000
 * Only one valid answer exists.
 * 
 * 
 */
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        // 左右指针往中间逼近
        // Note: 注意题目要求返回 index+1
        vector<int> answer;
        int i = 0, j = numbers.size()-1;
        while (i < j) {
            int curr = numbers[i]+numbers[j];
            if (curr == target) {
                answer.push_back(i+1);
                answer.push_back(j+1);
                return answer;
            } else if (curr < target) {
                i++;
            } else {
                j--;
            }
        }
        return answer;
    }
};
