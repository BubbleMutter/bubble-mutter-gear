/*
 * @lc app=leetcode id=66 lang=cpp
 *
 * [66] Plus One
 *
 * https://leetcode.com/problems/plus-one/description/
 *
 * algorithms
 * Easy (42.39%)
 * Total Accepted:    783.2K
 * Total Submissions: 1.9M
 * Testcase Example:  '[1,2,3]'
 *
 * Given a non-empty array of decimal digits representing a non-negative
 * integer, increment one to the integer.
 * 
 * The digits are stored such that the most significant digit is at the head of
 * the list, and each element in the array contains a single digit.
 * 
 * You may assume the integer does not contain any leading zero, except the
 * number 0 itself.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: digits = [1,2,3]
 * Output: [1,2,4]
 * Explanation: The array represents the integer 123.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: digits = [4,3,2,1]
 * Output: [4,3,2,2]
 * Explanation: The array represents the integer 4321.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: digits = [0]
 * Output: [1]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= digits.length <= 100
 * 0 <= digits[i] <= 9
 * 
 * 
 */

class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
#if 0 // 原始版
        bool carry = true; // 最开始加1
        for (int i = digits.size()-1; i >= 0; i--) {
            digits[i] += carry;
            carry = digits[i] >= 10;
            digits[i] %= 10;
        }
        if (carry) {
            vector<int> answer(digits.size()+1, 0);
            answer[0] = 1;
            return answer;
        }
        return digits;
#endif

#if 1 // 优化版本
// 1. 从右往左分别++
// 2. 因为 digits[i] 只能是 0~9;
//    所以 digits[i]++ 后, 只能是 1~10,
// 3. 如果 不是10; 那么循环终止; 因为不再有进位
// 4. 如果 是  10; 那么循环继续; digits[i] = 置位0;
// 5. 如果 能跑出循环; 那么一定是 99999 这种形式地数值
//    只需要返回 100000.. 即可

        for (int i = digits.size()-1; i >= 0; i--) {
            digits[i]++;
            if (digits[i] < 10)
                return digits;
            else
                digits[i] -= 10;
        }
        vector<int> answer(digits.size()+1, 0);
        answer[0] = 1;
        return answer;
#endif
    }
};

