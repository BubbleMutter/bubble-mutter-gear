/*
 * @lc app=leetcode id=67 lang=cpp
 *
 * [67] Add Binary
 *
 * https://leetcode.com/problems/add-binary/description/
 *
 * algorithms
 * Easy (46.78%)
 * Total Accepted:    567.9K
 * Total Submissions: 1.2M
 * Testcase Example:  '"11"\n"1"'
 *
 * Given two binary strings a and b, return their sum as a binary string.
 * 
 * 
 * Example 1:
 * Input: a = "11", b = "1"
 * Output: "100"
 * Example 2:
 * Input: a = "1010", b = "1011"
 * Output: "10101"
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= a.length, b.length <= 10^4
 * a and b consist only of '0' or '1' characters.
 * Each string does not contain leading zeros except for the zero itself.
 * 
 * 
 */
class Solution {
public:
    char addBinary(char x, char y, char & carry) {
        if (x == y) {
            char prime_carry = carry;
            carry = x;
            return prime_carry;
        }
        return carry == '0' ? '1' : '0';
    }
    string addBinary(string a, string b) {
        string answer;
        int i = a.size()-1, j = b.size()-1;
        char carry = '0';
        while (i >= 0 && j >= 0)
            answer += addBinary(a[i--], b[j--], carry);
        while (i >= 0)
            answer += addBinary(a[i--], '0', carry);
        while (j >= 0)
            answer += addBinary('0', b[j--], carry);
        if (carry == '1')
            answer += '1';
        reverse(answer.begin(), answer.end());
        return answer;
    }
};
