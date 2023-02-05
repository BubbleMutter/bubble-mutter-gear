/*
 * @lc app=leetcode id=345 lang=cpp
 *
 * [345] Reverse Vowels of a String
 *
 * https://leetcode.com/problems/reverse-vowels-of-a-string/description/
 *
 * algorithms
 * Easy (44.99%)
 * Total Accepted:    267K
 * Total Submissions: 591.7K
 * Testcase Example:  '"hello"'
 *
 * Write a function that takes a string as input and reverse only the vowels of
 * a string.
 * 
 * Example 1:
 * 
 * 
 * Input: "hello"
 * Output: "holle"
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: "leetcode"
 * Output: "leotcede"
 * 
 * 
 * Note:
 * The vowels does not include the letter "y".
 * 
 * 
 * 
 */
// 形同快排的交换法
// 1. 两个指针；一个前一个后
// 2. 前指针++ (注意单次循环++ 也要判断终止条件 前指针<后指针)
// 3. 后指针-- (注意单次循环-- 也要判断终止条件 前指针<后指针)
// 4. 单次循环中; 符合条件就停止 ++ 和 --
// 5. 交换两个符合条件的 元素
//
// Note:
//   这个问题 待交换元素的条件是 都是元音(包括大小写)
//   而快速排序  待交换元素 a[tail] 的条件是 <  pivot
//               待交换元素 a[head] 的条件是 >= pivot
class Solution {
    inline bool isVowel(char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }
public:
    string reverseVowels(string s) {
        int head = 0, tail = s.size()-1;
        while (head < tail) {
            while (head < tail && !isVowel(tolower(s[head])))
                head++;
            while (head < tail && !isVowel(tolower(s[tail])))
                tail--;
            if (head < tail)
                swap(s[head++], s[tail--]);
        }
        return s;
    }
};
