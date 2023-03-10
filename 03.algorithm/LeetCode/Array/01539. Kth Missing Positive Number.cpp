/*
 * @lc app=leetcode id=1539 lang=cpp
 *
 * [1539] Kth Missing Positive Number
 *
 * https://leetcode.com/problems/kth-missing-positive-number/description/
 *
 * algorithms
 * Easy (53.36%)
 * Total Accepted:    60.8K
 * Total Submissions: 110.3K
 * Testcase Example:  '[2,3,4,7,11]\n5'
 *
 * Given an array arr of positive integers sorted in a strictly increasing
 * order, and an integer k.
 * 
 * Find the k^th positive integer that is missing from this array.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [2,3,4,7,11], k = 5
 * Output: 9
 * Explanation: The missing positive integers are [1,5,6,8,9,10,12,13,...]. The
 * 5^th missing positive integer is 9.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [1,2,3,4], k = 2
 * Output: 6
 * Explanation: The missing positive integers are [5,6,7,...]. The 2^nd missing
 * positive integer is 6.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= arr.length <= 1000
 * 1 <= arr[i] <= 1000
 * 1 <= k <= 1000
 * arr[i] < arr[j] for 1 <= i < j <= arr.length
 * 
 * 
 */
class Solution {
public:
    int findKthPositive(vector<int>& arr, int k) {
        // 跳格子法:
        // 1. 假设有无限格子[1,2,3,4...]
        //    一个人最多可以 踩到 k 个格子上
        //    arr[i] 表示可以跳过的格子
        //    问最后一个人踩到哪个格子
        // 2. 人先走k步; 踩到 k 上
        //    如果 arr[i] <= k; 说明 可以跳过一个格子;
        //    k++
        //    直到发现 arr[i] > k; 即k不能再跳了
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i] <= k)
                k++;
            else
                return ;
        }
        return k;
    }
};
// Note: 误区; 不能只计算 小于 k 个原数个数
//       因为 k 要 "跳" 是会变的
