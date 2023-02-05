/*
 * @lc app=leetcode id=1342 lang=cpp
 *
 * [1342] Number of Steps to Reduce a Number to Zero
 *
 * https://leetcode.com/problems/number-of-steps-to-reduce-a-number-to-zero/description/
 *
 * algorithms
 * Easy (85.66%)
 * Total Accepted:    242.5K
 * Total Submissions: 283K
 * Testcase Example:  '14'
 *
 * Given an integer num, return the number of steps to reduce it to zero.
 * 
 * In one step, if the current number is even, you have to divide it by 2,
 * otherwise, you have to subtract 1 from it.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: num = 14
 * Output: 6
 * Explanation: 
 * Step 1) 14 is even; divide by 2 and obtain 7. 
 * Step 2) 7 is odd; subtract 1 and obtain 6.
 * Step 3) 6 is even; divide by 2 and obtain 3. 
 * Step 4) 3 is odd; subtract 1 and obtain 2. 
 * Step 5) 2 is even; divide by 2 and obtain 1. 
 * Step 6) 1 is odd; subtract 1 and obtain 0.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: num = 8
 * Output: 4
 * Explanation: 
 * Step 1) 8 is even; divide by 2 and obtain 4. 
 * Step 2) 4 is even; divide by 2 and obtain 2. 
 * Step 3) 2 is even; divide by 2 and obtain 1. 
 * Step 4) 1 is odd; subtract 1 and obtain 0.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: num = 123
 * Output: 12
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= num <= 10^6
 * 
 * 
 */
class Solution {
public:
    int numberOfSteps(int num) {
        // 由题意可推导得:
        // 1. 除最高bit 的 1以外
        //    如果 bit 位1 则需要操作2个步骤
        //    如果 bit 位0 则需要操作2个步骤
        // 2. 最后最高位 bit 1 只操作1个步骤
        if (num == 0) return 0;
        int count = 0;
        while (num > 1) {
            count += (num & 1) ? 2 : 1;
            num >>= 1;
        }
        return count+1;
    }
};
