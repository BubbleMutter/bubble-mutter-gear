/*
 * @lc app=leetcode id=1175 lang=cpp
 *
 * [1175] Prime Arrangements
 *
 * https://leetcode.com/problems/prime-arrangements/description/
 *
 * algorithms
 * Easy (52.43%)
 * Total Accepted:    19.4K
 * Total Submissions: 36.8K
 * Testcase Example:  '5'
 *
 * Return the number of permutations of 1 to n so that prime numbers are at
 * prime indices (1-indexed.)
 *
 * (Recall that an integer is prime if and only if it is greater than 1, and
 * cannot be written as a product of two positive integers both smaller than
 * it.)
 *
 * Since the answer may be large, return the answer modulo 10^9 + 7.
 *
 *
 * Example 1:
 *
 *
 * Input: n = 5
 * Output: 12
 * Explanation: For example [1,2,5,4,3] is a valid permutation, but [5,2,3,4,1]
 * is not because the prime number 5 is at index 1.
 *
 *
 * Example 2:
 *
 *
 * Input: n = 100
 * Output: 682289015
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= n <= 100
 *
 *
 */
class Solution {
public:
    int numPrimeArrangements(int n) {
        // 1. 把数组分割为两个部分;
        // 2. 质数只能与质数进行全排列
        // 3. 素数只能与素数进行全排列
        // 4. 根据排列公式求得两者的积
        //
        // 5. 优化: 提前计算 100 内的质数
#if 0 /* 动态计算质数数组 */
        vector<bool> prime(n, false);
        for (size_t i = 1; i < n; ++i) {
            int j = i+1;
            if (j % 2 == 0) {
                prime[i] = true;
                continue;
            }
            int k = 2;
            while (j*k-1 < n) {
                prime[j*k-1] = true;
                ++k;
            }
        }
        int n_prime = count(prime.cbegin(), prime.cend(), true);
#else /* 优化: 提前计算出 100 内的质数 */
        set<int> prime {
            1,3,5,7,11,13,17,19,23,29,31,37,41,
            43,47,53,59,61,67,71,73,79,83,89,97
        };
        int n_prime = 0;
        for (int i = 1; i <=n; ++i) {
            if (prime.count(i))
                ++n_prime;
        }
#endif
        int res = 1;
        for (size_t i = 1; i <= n_prime; ++i) {
            res = (res * i) % 1000000007;
        }
        for (size_t i = 1; i <= n-n_prime; ++i) {
            res = (res * i) % 1000000007;
        }
        return res;
    }
};
