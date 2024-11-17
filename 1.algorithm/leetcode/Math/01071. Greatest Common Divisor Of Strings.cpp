/*
 * @lc app=leetcode id=1071 lang=cpp
 *
 * [1071] Greatest Common Divisor of Strings
 *
 * https://leetcode.com/problems/greatest-common-divisor-of-strings/description/
 *
 * algorithms
 * Easy (51.75%)
 * Total Accepted:    68K
 * Total Submissions: 131.8K
 * Testcase Example:  '"ABCABC"\n"ABC"'
 *
 * For two strings s and t, we say "t divides s" if and only if s = t + ... + t
 * (i.e., t is concatenated with itself one or more times).
 *
 * Given two strings str1 and str2, return the largest string x such that x
 * divides both str1 and str2.
 *
 *
 * Example 1:
 *
 *
 * Input: str1 = "ABCABC", str2 = "ABC"
 * Output: "ABC"
 *
 *
 * Example 2:
 *
 *
 * Input: str1 = "ABABAB", str2 = "ABAB"
 * Output: "AB"
 *
 *
 * Example 3:
 *
 *
 * Input: str1 = "LEET", str2 = "CODE"
 * Output: ""
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= str1.length, str2.length <= 1000
 * str1 and str2 consist of English uppercase letters.
 *
 *
 */
class Solution {
public:
    string gcdOfStrings(string str1, string str2) {
        int j = 0;
        // 1. 双指针遍历, 遇到不相等的终止循环
        // 2. 如果 str2 的指针遍历完成, 则 str2 就是结果
        // 3. 如果 str2 没有遍历完成, 但 str1 已遍历完成
        //    那么 分割 str2 剩余部分 和 与str1 匹配部分
        // 4. 递归, 注意较长的部分作为第一参数
        //
        // Note: 递归法本质上是求两个数的最大公因数的方法
        //       00365. Water And Jug Problem.cpp
        for (size_t i = 0; i < str1.size(); ++i) {
            if (str1[i] != str2[j])
                return "";
            j = (j+1) % str2.size();
        }
        if (j == 0) {
            return str2;
        }
        string x = str2.substr(0, j);
        string y = str2.substr(j);
        if (x.size() < y.size())
            swap(x, y);
        return gcdOfStrings(x, y);
    }
};
