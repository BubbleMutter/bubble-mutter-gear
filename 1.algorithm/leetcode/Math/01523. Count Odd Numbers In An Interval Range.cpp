/*
 * @lc app=leetcode id=1523 lang=cpp
 *
 * [1523] Count Odd Numbers in an Interval Range
 *
 * https://leetcode.com/problems/count-odd-numbers-in-an-interval-range/description/
 *
 * algorithms
 * Easy (54.72%)
 * Total Accepted:    19.8K
 * Total Submissions: 36.2K
 * Testcase Example:  '3\n7'
 *
 * Given two non-negative integers low and high. Return the count of odd
 * numbers between low and high (inclusive).
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: low = 3, high = 7
 * Output: 3
 * Explanation: The odd numbers between 3 and 7 are [3,5,7].
 * 
 * Example 2:
 * 
 * 
 * Input: low = 8, high = 10
 * Output: 1
 * Explanation: The odd numbers between 8 and 10 are [9].
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= low <= high <= 10^9
 * 
 */
class Solution {
public:
    int countOdds(int low, int high) {
		// diff = high - low;
		// 求得 [low, high-1] 数组(左右闭合)的元素个数
		//
		// 情况1: 都是奇数; diff 是偶数; [low, high-1] 有一半是奇数
		//        diff = high-low;
		//        answer = (high-low)/2 + 1
		// 情况2: 都是偶数; diff 是偶数; [low, high-1] 有一半是奇数
        //        answer = diff / 2;
		// 情况3: 低奇高偶; high++ 转化为情况1; 但最后不计算 high+1
		//        diff = (high+1) - low; // [low, high+1-1] 取一半
		//        answer = ((high+1) - low)/ 2;     // 不要补1
		// 情况4: 低偶高奇; high++ 转化为情况2; 
		//        diff = (high+1) - low; // [low, high+1-1] 取一半
		//        answer = ((high+1) - low)/ 2;
		// Note: 情况3,4 合并
        if (low & 0x1 && high & 0x1)
			return (high - low)/2 + 1;
        if (!(low & 0x1) && !(high & 0x1))
			return (high - low)/2;
		return (high - low + 1)/2;
    }
};
