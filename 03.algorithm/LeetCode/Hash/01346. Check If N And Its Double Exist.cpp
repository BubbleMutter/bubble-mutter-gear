/*
 * @lc app=leetcode id=1346 lang=cpp
 *
 * [1346] Check If N and Its Double Exist
 *
 * https://leetcode.com/problems/check-if-n-and-its-double-exist/description/
 *
 * algorithms
 * Easy (35.49%)
 * Total Accepted:    177.2K
 * Total Submissions: 499.8K
 * Testcase Example:  '[10,2,5,3]'
 *
 * Given an array arr of integers, check if there exists two integers N and M
 * such that N is the double of M ( i.e. N = 2 * M).
 * 
 * More formally check if there exists two indices i and j such that :
 * 
 * 
 * i != j
 * 0 <= i, j < arr.length
 * arr[i] == 2 * arr[j]
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [10,2,5,3]
 * Output: true
 * Explanation: N = 10 is the double of M = 5,that is, 10 = 2 * 5.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [7,1,14,11]
 * Output: true
 * Explanation: N = 14 is the double of M = 7,that is, 14 = 2 * 7.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: arr = [3,1,7,11]
 * Output: false
 * Explanation: In this case does not exist N and M, such that N = 2 * M.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 2 <= arr.length <= 500
 * -10^3 <= arr[i] <= 10^3
 * 
 * 
 */
class Solution {
public:
    bool checkIfExist(vector<int>& arr) {
        // 禁忌: 全部转为 hash 后再遍历, 这样会慢
        // 1. 逐个遍历
        // 2. arr[i]*2 已存在
        //    arr[i]为偶数且 arr[i]/2 已存在
        // 3. 
        set<int> hash;
        for (const auto val: arr) {
            if (hash.count(val*2) > 0 || (!(val & 0x1) && hash.count(val/2) > 0)) {
                return true;
            }
            hash.insert(val);
        }
        return false;
    }
};
