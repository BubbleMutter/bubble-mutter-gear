/*
 * @lc app=leetcode id=367 lang=cpp
 *
 * [367] Valid Perfect Square
 *
 * https://leetcode.com/problems/valid-perfect-square/description/
 *
 * algorithms
 * Easy (42.03%)
 * Total Accepted:    260.2K
 * Total Submissions: 618.9K
 * Testcase Example:  '16'
 *
 * Given a positive integer num, write a function which returns True if num is
 * a perfect square else False.
 * 
 * Follow up: Do not use any built-in library function such as sqrt.
 * 
 * 
 * Example 1:
 * Input: num = 16
 * Output: true
 * Example 2:
 * Input: num = 14
 * Output: false
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= num <= 2^31 - 1
 * 
 * 
 */
class Solution {
public:
    // [1, num/2] 左右闭合区间用二分查找开根数
    bool isPerfectSquare(int num) {
        if (num == 1) return true;
        int i = 1, j = num/2;
        while (i <= j) {
            long m = i + (j-i)/2;
            long square = m*m;

            if (square < num) {
                i = m+1;
            } else if (square > num) {
                j = m-1;
            } else {
                return true;
            }

        }
        return false;
    }
};
