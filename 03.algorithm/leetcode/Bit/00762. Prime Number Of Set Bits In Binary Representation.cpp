/*
 * @lc app=leetcode id=762 lang=cpp
 *
 * [762] Prime Number of Set Bits in Binary Representation
 *
 * https://leetcode.com/problems/prime-number-of-set-bits-in-binary-representation/description/
 *
 * algorithms
 * Easy (66.16%)
 * Total Accepted:    65.1K
 * Total Submissions: 98.4K
 * Testcase Example:  '6\n10'
 *
 * Given two integers left and right, return the count of numbers in the
 * inclusive range [left, right] having a prime number of set bits in their
 * binary representation.
 *
 * Recall that the number of set bits an integer has is the number of 1's
 * present when written in binary.
 * 
 * 
 * For example, 21 written in binary is 10101, which has 3 set bits.
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: left = 6, right = 10
 * Output: 4
 * Explanation:
 * 6  -> 110 (2 set bits, 2 is prime)
 * 7  -> 111 (3 set bits, 3 is prime)
 * 8  -> 1000 (1 set bit, 1 is not prime)
 * 9  -> 1001 (2 set bits, 2 is prime)
 * 10 -> 1010 (2 set bits, 2 is prime)
 * 4 numbers have a prime number of set bits.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: left = 10, right = 15
 * Output: 5
 * Explanation:
 * 10 -> 1010 (2 set bits, 2 is prime)
 * 11 -> 1011 (3 set bits, 3 is prime)
 * 12 -> 1100 (2 set bits, 2 is prime)
 * 13 -> 1101 (3 set bits, 3 is prime)
 * 14 -> 1110 (3 set bits, 3 is prime)
 * 15 -> 1111 (4 set bits, 4 is not prime)
 * 5 numbers have a prime number of set bits.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= left <= right <= 10^6
 * 0 <= right - left <= 10^4
 * 
 * 
 */
class Solution {
public:
    int countBits(int n) {
        // LeetCode 00191. Number Of 1 Bits
        int count = 0;
        while (n) {
            n = (n & (n - 1));
            count++;
        }
        return count;
    }
    int isPrime19(int n) {
        return n ==  2 || n ==  3 || n ==  5 ||  n == 7 ||
               n == 11 || n == 13 || n == 17 || n == 19;
    }
    int countPrimeSetBits(int L, int R) {
        // 因为 R 最大值是 10^6
        // 而且 log2(10^6) < 20
        // 所以 任意L~R数值 二进制位树最大是 19
        // 即可 判断L~R数值 的位数; 是否属于 [1, 19] 中的质数集合
        int ans = 0;
        for (int i = L; i <= R; ++i) {
            if (isPrime19(countBits(i)))
                ans++;
        }
        return ans;
    }
};
