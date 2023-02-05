/*
 * @lc app=leetcode id=168 lang=cpp
 *
 * [168] Excel Sheet Column Title
 *
 * https://leetcode.com/problems/excel-sheet-column-title/description/
 *
 * algorithms
 * Easy (31.66%)
 * Total Accepted:    245.7K
 * Total Submissions: 774.5K
 * Testcase Example:  '1'
 *
 * Given a positive integer, return its corresponding column title as appear in
 * an Excel sheet.
 * 
 * For example:
 * 
 * 
 * ⁠   1 -> A
 * ⁠   2 -> B
 * ⁠   3 -> C
 * ⁠   ...
 * ⁠   26 -> Z
 * ⁠   27 -> AA
 * ⁠   28 -> AB 
 * ⁠   ...
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: 1
 * Output: "A"
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: 28
 * Output: "AB"
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: 701
 * Output: "ZY"
 * 
 */
class Solution {
public:
    string convertToTitle(int n) {
        vector<char> answer;
        while (n > 0) {
            // 1. 这个是26禁止的变体, 没有数值0:
            //    每个位上的数值是 1~26
            // 2. 当数值能整除26; 不能使用使用0表示当前位
            //    必须用26表示当前位的数值;
            // 3. 所以 取模 26; 如果取得0; 那么当前位应该是26
            int curr = n%26;
            if (curr == 0) curr = 26;
            answer.push_back( curr - 1 + 'A');
            n = (n-curr) / 26;
        }
        return string(answer.rbegin(), answer.rend());
    }
};
