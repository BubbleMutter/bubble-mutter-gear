/*
 * @lc app=leetcode id=338 lang=cpp
 *
 * [338] Counting Bits
 *
 * https://leetcode.com/problems/counting-bits/description/
 *
 * algorithms
 * Easy (72.41%)
 * Total Accepted:    468.2K
 * Total Submissions: 646.4K
 * Testcase Example:  '2'
 *
 * Given an integer n, return an array ans of length n + 1 such that for each i
 * (0 <= i <= n), ans[i] is the number of 1's in the binary representation of
 * i.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 2
 * Output: [0,1,1]
 * Explanation:
 * 0 --> 0
 * 1 --> 1
 * 2 --> 10
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 5
 * Output: [0,1,1,2,1,2]
 * Explanation:
 * 0 --> 0
 * 1 --> 1
 * 2 --> 10
 * 3 --> 11
 * 4 --> 100
 * 5 --> 101
 * 
 * 
 * 
 * Constraints:
 *
 *
 * 0 <= n <= 10^5
 *
 *
 *
 * Follow up:
 *
 *
 * It is very easy to come up with a solution with a runtime of O(n log n). Can
 * you do it in linear time O(n) and possibly in a single pass?
 * Can you do it without using any built-in function (i.e., like
 * __builtin_popcount in C++)?
 *
 *
 */
class Solution {
public:
    vector<int> countBits(int n) {
        int lower_bound = 0, upper_bound = 1;
        vector<int> res(n+1, 0);
        for (int i = 1; i < res.size(); ++i) {
            if (i == upper_bound) {
                lower_bound = upper_bound;
                upper_bound *= 2;
            }
            if (i < (lower_bound + upper_bound)/2) {
                res[i] = res[i-lower_bound/2];
            } else {
                res[i] = res[i-lower_bound/2]+1;
            }
        }
        return res;
    }
};

#if 0 /* 推导过程 */
0: b'0     0
1: b'1     1
2: b'10    1
3: b'11    2

4: b'100   1
5: b'101   2
6: b'110   2
7: b'111   3

 8: b'1000  1
 9: b'1001  2
10: b'1010  2
11: b'1011  3

12: b'1100  2
13: b'1101  3
14: b'1110  3
15: b'1111  4


16: b'10000 1
17: b'10001 2
18: b'10010 2
19: b'10011 3

20: b'10100 2
21: b'10101 3
22: b'10110 3
23: b'10111 4

24: b'11000 2
25: b'11001 3
26: b'11010 3
27: b'11011 4

28: b'11100 3
29: b'11101 4
30: b'11110 4
31: b'11111 5

      [0]     0
d:00, [1]     1
d:01, [02~03] 1,2
d:02, [04~07] 1,2,2,3
d:04, [08~15] 1,2,2,3; 2,3,3,4;
d:08, [16~31] 1,2,2,3; 2,3,3,4; 2,3,3,4; 3,4,4,5;

如图, 呈规律的两行中,
      左闭区间为2的指数, 右边区间为下个2的指数减1
假设, 区间为 [x, y]

当 x <= n <= (x+y)/2; 即行的左半边
则 res[n] = res[n的上一行]
而 n距离上一行为d, 即res[n] = res[n-x/2]

当 (x+y)/2 <= n <= y; 即行的右半边
则 res[n] = res[n的左半边对应位置]+1
相当于 res[n] = res[n-x/2]+1
#endif
