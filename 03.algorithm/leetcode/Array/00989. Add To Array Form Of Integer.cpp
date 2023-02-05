/*
 * @lc app=leetcode id=989 lang=cpp
 *
 * [989] Add to Array-Form of Integer
 *
 * https://leetcode.com/problems/add-to-array-form-of-integer/description/
 *
 * algorithms
 * Easy (45.24%)
 * Total Accepted:    99.6K
 * Total Submissions: 220.3K
 * Testcase Example:  '[1,2,0,0]\n34'
 *
 * The array-form of an integer num is an array representing its digits in left
 * to right order.
 * 
 * 
 * For example, for num = 1321, the array form is [1,3,2,1].
 * 
 * 
 * Given num, the array-form of an integer, and an integer k, return the
 * array-form of the integer num + k.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: num = [1,2,0,0], k = 34
 * Output: [1,2,3,4]
 * Explanation: 1200 + 34 = 1234
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: num = [2,7,4], k = 181
 * Output: [4,5,5]
 * Explanation: 274 + 181 = 455
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: num = [2,1,5], k = 806
 * Output: [1,0,2,1]
 * Explanation: 215 + 806 = 1021
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= num.length <= 10^4
 * 0 <= num[i] <= 9
 * num does not contain any leading zeros except for the zero itself.
 * 1 <= k <= 10^4
 * 
 * 
 */
class Solution {
public:
    vector<int> addToArrayForm(vector<int>& num, int k) {
        reverse(num.begin(), num.end());
        int plus = 0, i = 0;
        // 1. 把A 翻转
        // 2. 低位开始加上
        // 3. k 超出 num 的位
        // 4. 最后还有进位
        auto add = [](int x, int y, int &plus) {
            int val = x + y + plus;
            plus = val > 9;
            return val % 10;
        };
        while (i < num.size()) {
            num[i++] = add(k%10, num[i], plus);
            k /= 10;
            // 提前终止
            if (plus == 0 && k == 0)
                break;
        }
        // 补k
        while (k) {
            num.push_back(add(k%10, 0, plus));
            k /= 10;
        }
        // 补位
        if (plus > 0) {
            num.push_back(1);
        }
        reverse(num.begin(), num.end());
        return num;
    }
};
#if 0 /* 较慢算法 */
class Solution {
public:
    vector<int> __addVectors(vector<int> &x, vector<int> &y) {
        // 00415. Add Strings.cpp
        auto addone = [&](int &l, int r, int &give) {
            l += r + give;
            give = l / 10;
            l %= 10;
        };
        int i = 0, give = 0;
        if (x.size() < y.size())
            swap(x, y);
        for (; i < y.size(); i++)
            addone(x[i], y[i], give);
        for (; i < x.size() && give; i++)
            addone(x[i], 0, give);
        if (give)
            x.push_back(1);
        return x;
    }

    vector<int> addToArrayForm(vector<int>& A, int K) {
        // 1. 把A 翻转
        // 2. K 转化为逆序的 vector
        // 3. 使用 00415. Add Strings.cpp 的 vector 加法
        // 4. 再翻转
        reverse(A.begin(), A.end());
        vector<int> Y, Z;
        while (K) {
            Y.push_back(K % 10);
            K /= 10;
        }
        Z = __addVectors(A, Y);
        reverse(Z.begin(), Z.end());
        return Z;
    }
};
#endif
