/*
 * @lc app=leetcode id=728 lang=cpp
 *
 * [728] Self Dividing Numbers
 *
 * https://leetcode.com/problems/self-dividing-numbers/description/
 *
 * algorithms
 * Easy (76.67%)
 * Total Accepted:    169.8K
 * Total Submissions: 221.4K
 * Testcase Example:  '1\n22'
 *
 * A self-dividing number is a number that is divisible by every digit it
 * contains.
 * 
 * 
 * For example, 128 is a self-dividing number because 128 % 1 == 0, 128 % 2 ==
 * 0, and 128 % 8 == 0.
 * 
 * 
 * A self-dividing number is not allowed to contain the digit zero.
 * 
 * Given two integers left and right, return a list of all the self-dividing
 * numbers in the range [left, right].
 * 
 * 
 * Example 1:
 * Input: left = 1, right = 22
 * Output: [1,2,3,4,5,6,7,8,9,11,12,15,22]
 * Example 2:
 * Input: left = 47, right = 85
 * Output: [48,55,66,77]
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= left <= right <= 10^4
 * 
 * 
 */
class Solution {
public:
    vector<int> selfDividingNumbers(int left, int right) {
        vector<int> res;
        for(int i = left; i <= right; ++i) {
            if (isSelfDividing(i)) {
                res.push_back(i);
            }
        }
        return res;
    }
private:
    bool isSelfDividing(int n) {
        int v = n;
        while (v) {
            int c = v % 10;
            v /= 10;
            if (c == 0 || (c && n % c != 0)) {
                return false;
            }
        }
        return true;
    }
};
