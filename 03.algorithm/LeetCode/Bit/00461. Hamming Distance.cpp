/*
 * @lc app=leetcode id=461 lang=cpp
 *
 * [461] Hamming Distance
 *
 * https://leetcode.com/problems/hamming-distance/description/
 *
 * algorithms
 * Easy (74.36%)
 * Total Accepted:    458.6K
 * Total Submissions: 616.6K
 * Testcase Example:  '1\n4'
 *
 * The Hamming distance between two integers is the number of positions at
 * which the corresponding bits are different.
 * 
 * Given two integers x and y, return the Hamming distance between them.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: x = 1, y = 4
 * Output: 2
 * Explanation:
 * 1   (0 0 0 1)
 * 4   (0 1 0 0)
 * ⁠      ↑   ↑
 * The above arrows point to positions where the corresponding bits are
 * different.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: x = 3, y = 1
 * Output: 1
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= x, y <= 2^31 - 1
 * 
 * 
 */
class Solution {
public:
    int hammingDistance(int x, int y) {
        /**
         * 汉明距离
         * 求出 两个数; 其不同的 bit 位个数
         * 1. 取 xor ; 其中1的位置就是 不同的位置
         * 2. 求上一步骤结果中地 1 的个数 (00191. Number of 1 Bits)
         */
        int res = 0, z = x^y;
        while (z) {
            z &= z-1; /* 每次循环消除一个 1-bit */
            res++;
        }
        return res;
    }
};
