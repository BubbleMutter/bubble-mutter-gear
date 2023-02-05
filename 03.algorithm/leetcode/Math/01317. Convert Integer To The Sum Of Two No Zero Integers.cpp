/*
 * @lc app=leetcode id=1317 lang=cpp
 *
 * [1317] Convert Integer to the Sum of Two No-Zero Integers
 *
 * https://leetcode.com/problems/convert-integer-to-the-sum-of-two-no-zero-integers/description/
 *
 * algorithms
 * Easy (57.35%)
 * Total Accepted:    29K
 * Total Submissions: 50.8K
 * Testcase Example:  '2'
 *
 * No-Zero integer is a positive integer that does not contain any 0 in its
 * decimal representation.
 * 
 * Given an integer n, return a list of two integers [A, B] where:
 * 
 * 
 * A and B are No-Zero integers.
 * A + B = n
 * 
 * 
 * The test cases are generated so that there is at least one valid solution.
 * If there are many valid solutions you can return any of them.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 2
 * Output: [1,1]
 * Explanation: A = 1, B = 1. A + B = n and both A and B do not contain any 0
 * in their decimal representation.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 11
 * Output: [2,9]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 2 <= n <= 10^4
 * 
 * 
 */
class Solution {
public:
    vector<int> getNoZeroIntegers(int n) {
        // 1. 找到最高位然后进行拆分
        //    1010 -> 1000 + 10
        //    2100 -> 2000 + 100
        //    2000 -> 2000 + 0
        // 2. 当两个部分都不满足条件时
        //    res[0]--, res[1]++
        //    直到满足条件, 由题意可得必然存在结果


        // 找最高位 10进制
        int val = n, pow = 1;
        while (val > 10) { /* 找最高位的方法 */
            val /= 10;
            pow *= 10;
        }
        // 分割
        vector<int> res { 0, 0 };
        res[0] = (n / pow) * pow;
        res[1] = n - res[0];
        // 校验器
        auto IsNoZero = [] (int val) {
            if (val == 0) return false;
            while (val) {
                if (val % 10 == 0)
                    return false;
                val /= 10;
            }
            return true;
        };
        // 转移
        while (!IsNoZero(res[0]) || !IsNoZero(res[1])) {
            res[0]--;
            res[1]++;
        }
        return res;
    }
};
