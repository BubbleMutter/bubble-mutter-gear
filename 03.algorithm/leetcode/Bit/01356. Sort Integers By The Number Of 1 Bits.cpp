/*
 * @lc app=leetcode id=1356 lang=cpp
 *
 * [1356] Sort Integers by The Number of 1 Bits
 *
 * https://leetcode.com/problems/sort-integers-by-the-number-of-1-bits/description/
 *
 * algorithms
 * Easy (70.83%)
 * Total Accepted:    60.5K
 * Total Submissions: 85.1K
 * Testcase Example:  '[0,1,2,3,4,5,6,7,8]'
 *
 * You are given an integer array arr. Sort the integers in the array in
 * ascending order by the number of 1's in their binary representation and in
 * case of two or more integers have the same number of 1's you have to sort
 * them in ascending order.
 * 
 * Return the array after sorting it.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [0,1,2,3,4,5,6,7,8]
 * Output: [0,1,2,4,8,3,5,6,7]
 * Explantion: [0] is the only integer with 0 bits.
 * [1,2,4,8] all have 1 bit.
 * [3,5,6] have 2 bits.
 * [7] has 3 bits.
 * The sorted array by bits is [0,1,2,4,8,3,5,6,7]
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [1024,512,256,128,64,32,16,8,4,2,1]
 * Output: [1,2,4,8,16,32,64,128,256,512,1024]
 * Explantion: All integers have 1 bit in the binary representation, you should
 * just sort them in ascending order.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= arr.length <= 500
 * 0 <= arr[i] <= 10^4
 * 
 * 
 */
class Solution {
public:
    vector<int> sortByBits(vector<int>& arr) {
        // 1. 计算1的个数 00191. Number Of 1 Bits.cpp
        // 2. 构造成 pair (arr[i], nbit) ; 自定义排序方法
        // 3. 排序
        // 4. 提取排序结果
        auto fnCountBit = [] (int val) {
            int count = 0;
            while (val) {
                val &= (val - 1);
                count++;
            }
            return count;
        };

        vector<vector<int>>  bit(arr.size(), vector<int>(2, 0));
        for (size_t i = 0; i < arr.size(); ++i) {
            bit[i][0] = arr[i];
            bit[i][1] = fnCountBit(arr[i]);
        }
        sort(bit.begin(), bit.end(), [](const vector<int>& x, const vector<int>& y) {
                return (x[1] != y[1]) ? (x[1] < y[1]) : (x[0] < y[0]);
                });
        for (size_t i = 0; i < bit.size(); ++i) {
            arr[i] = bit[i][0];
        }
        return arr;
    }
};
