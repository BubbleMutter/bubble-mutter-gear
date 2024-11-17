/*
 * @lc app=leetcode id=1207 lang=cpp
 *
 * [1207] Unique Number of Occurrences
 *
 * https://leetcode.com/problems/unique-number-of-occurrences/description/
 *
 * algorithms
 * Easy (72.07%)
 * Total Accepted:    116.5K
 * Total Submissions: 161.8K
 * Testcase Example:  '[1,2,2,1,1,3]'
 *
 * Given an array of integers arr, return true if the number of occurrences of
 * each value in the array is unique, or false otherwise.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [1,2,2,1,1,3]
 * Output: true
 * Explanation: The value 1 has 3 occurrences, 2 has 2 and 3 has 1. No two
 * values have the same number of occurrences.
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [1,2]
 * Output: false
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: arr = [-3,0,1,-3,1,1,1,-3,10,0]
 * Output: true
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= arr.length <= 1000
 * -1000 <= arr[i] <= 1000
 * 
 * 
 */
class Solution {
public:
    bool uniqueOccurrences(vector<int>& arr) {
        // 1. hash 法记录每个数值的频次
        // 2. hash 法判断是否有重复频次
        map<int, int> count;
        for (const auto& val: arr) {
            count[val]++;
        }
        set<int> unique;
        for (const auto& p: count) {
            if (unique.count(p.second)) {
                return false;
            }
            unique.insert(p.second);
        }
        return true;
    }
};
