/*
 * @lc app=leetcode id=204 lang=cpp
 *
 * [204] Count Primes
 *
 * https://leetcode.com/problems/count-primes/description/
 *
 * algorithms
 * Easy (32.18%)
 * Total Accepted:    442.4K
 * Total Submissions: 1.4M
 * Testcase Example:  '10'
 *
 * Count the number of prime numbers less than a non-negative number, n.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 10
 * Output: 4
 * Explanation: There are 4 prime numbers less than 10, they are 2, 3, 5, 7.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 0
 * Output: 0
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: n = 1
 * Output: 0
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= n <= 5 * 10^6
 * 
 * 
 */

class Solution {
public:
    int countPrimes(int n) {
        // 完全不会; 看题解的
        // 原理 https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
        //
        // 1. 构造 bool p[n] 数组
        //    p[i] 表示 i 是否为 prime
        //    初始化假设全部为 prime; 即 true
        //
        // 2. 当一个数值 i 为 prime;
        //    那么其 i*i, i*(i+1), i*(i+2), 一定不是 prime
        //
        // 3. 遍历 数值 [0, sqrt(n)), 左闭右开
        //    因为题目要求少于 n; 实际上是 [0, n-1] 区间
        //    针对每个 i 执行步骤 2.
        if (!n) return 0; /* n == 0 不能构造数组 */
        vector<bool> prime(n, true);
        prime[0] = prime[1] = false; // 0,1 都不是质数
        for (int i = 2; i < sqrt(n); i++) {
            if (prime[i]) {
                for (int j = i*i; j < n; j += i)
                    prime[j] = false;
            }
        }
        return count(prime.begin(), prime.end(), true);
    }

};
