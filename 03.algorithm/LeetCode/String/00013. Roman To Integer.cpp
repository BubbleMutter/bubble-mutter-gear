/*
 * @lc app=leetcode id=13 lang=cpp
 *
 * [13] Roman to Integer
 *
 * https://leetcode.com/problems/roman-to-integer/description/
 *
 * algorithms
 * Easy (56.46%)
 * Total Accepted:    906K
 * Total Submissions: 1.6M
 * Testcase Example:  '"III"'
 *
 * Roman numerals are represented by seven different symbols: I, V, X, L, C, D
 * and M.
 * 
 * 
 * Symbol       Value
 * I             1
 * V             5
 * X             10
 * L             50
 * C             100
 * D             500
 * M             1000
 * 
 * For example, 2 is written as II in Roman numeral, just two one's added
 * together. 12 is written as XII, which is simply X + II. The number 27 is
 * written as XXVII, which is XX + V + II.
 * 
 * Roman numerals are usually written largest to smallest from left to right.
 * However, the numeral for four is not IIII. Instead, the number four is
 * written as IV. Because the one is before the five we subtract it making
 * four. The same principle applies to the number nine, which is written as IX.
 * There are six instances where subtraction is used:
 * 
 * 
 * I can be placed before V (5) and X (10) to make 4 and 9. 
 * X can be placed before L (50) and C (100) to make 40 and 90. 
 * C can be placed before D (500) and M (1000) to make 400 and 900.
 * 
 * 
 * Given a roman numeral, convert it to an integer.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "III"
 * Output: 3
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "IV"
 * Output: 4
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "IX"
 * Output: 9
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: s = "LVIII"
 * Output: 58
 * Explanation: L = 50, V= 5, III = 3.
 * 
 * 
 * Example 5:
 * 
 * 
 * Input: s = "MCMXCIV"
 * Output: 1994
 * Explanation: M = 1000, CM = 900, XC = 90 and IV = 4.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 15
 * s contains only the characters ('I', 'V', 'X', 'L', 'C', 'D', 'M').
 * It is guaranteed that s is a valid roman numeral in the range [1, 3999].
 * 
 */


class Solution {
public:
    int __romanToInt(char c) {
        switch (c) {
            case 'I': return 1; break;
            case 'V': return 5; break;
            case 'X': return 10; break;
            case 'L': return 50; break;
            case 'C': return 100; break;
            case 'D': return 500; break;
            case 'M': return 1000; break;
            default: return 0;
        }
        return 0;
    }

#if 1
// 1. 从右往左
// 2. 如果当前数值 小于 之前的数值
//    则减去当前数值 (应对 4/9, 40/90, 400/900) 的场景
//    否则 加上当前数值
// 3. 记录当前数值

    int romanToInt(string s) {
        int answer = 0;
        for (int i = s.size()-1, last = 0; i >= 0; --i) {
            int curr = __romanToInt(s[i]);
            answer += (curr < last) ? -curr : curr;
            last = curr;
        }
        return answer;
    }

#endif

#if 0
// 1. from left to right
// 2. if curr > last:
//      answer += curr - last*2
//    else:
//      answer += curr
//
// Note:
//    这里有个坑: 如果last默认写了0 就会有bug
//    last 应该是 最大的数 1000 或者更大
    int romanToInt(string s) {
        int answer = 0;
        for (int i = 0, last = numeric_limits<int>::max(); i < s.size(); i++) {
            int curr = __romanToInt(s[i]);
            answer += curr > last ? curr-2*last: curr;
            last = curr;
        }
        return answer;
    }
#endif
};
