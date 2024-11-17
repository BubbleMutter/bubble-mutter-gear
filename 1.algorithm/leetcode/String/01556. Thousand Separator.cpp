/*
 * @lc app=leetcode id=1556 lang=cpp
 *
 * [1556] Thousand Separator
 *
 * https://leetcode.com/problems/thousand-separator/description/
 *
 * algorithms
 * Easy (57.91%)
 * Total Accepted:    19.3K
 * Total Submissions: 33.4K
 * Testcase Example:  '987'
 *
 * Given an integer n, add a dot (".") as the thousands separator and return it
 * in string format.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 987
 * Output: "987"
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 1234
 * Output: "1.234"
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: n = 123456789
 * Output: "123.456.789"
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: n = 0
 * Output: "0"
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= n < 2^31
 * 
 * 
 */
class Solution {
public:
    string thousandSeparator(int n) {
        // 1. 先转为字符串
        // 2. 再反向遍历字符串 入列
        // 3. 当入列数字个数达到3的倍数, 再入列第4个数时
        //    先入 '.', 然后再把当前数字入列
        string answer;
		string s = to_string(n);
		int i = 0;
		for (auto it = s.rbegin(); it != s.rend(); it++) {
			if (i > 0 && i%3 == 0)
				answer += '.';
			answer += *it;
			i++;
		}
        reverse(answer.begin(), answer.end());
		return answer;
    }
};
