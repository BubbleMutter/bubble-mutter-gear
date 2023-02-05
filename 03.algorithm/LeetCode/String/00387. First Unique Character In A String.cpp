/*
 * @lc app=leetcode id=387 lang=cpp
 *
 * [387] First Unique Character in a String
 *
 * https://leetcode.com/problems/first-unique-character-in-a-string/description/
 *
 * algorithms
 * Easy (53.76%)
 * Total Accepted:    696.4K
 * Total Submissions: 1.3M
 * Testcase Example:  '"leetcode"'
 *
 * Given a string, find the first non-repeating character in it and return its
 * index. If it doesn't exist, return -1.
 * 
 * Examples:
 * 
 * 
 * s = "leetcode"
 * return 0.
 * 
 * s = "loveleetcode"
 * return 2.
 * 
 * 
 * 
 * 
 * Note: You may assume the string contains only lowercase English letters.
 * 
 */
class Solution {
public:
    int firstUniqChar(string s) {
#if 1
        // 1. 构造 字母表
        //    a) tbl[] < 0 表示没有出现过
        //    b) tbl[] > 0 表示出现过的index
        //    c) tbl[] == s.size() 表示多次出现
        // 2. 遍历字母表
        //    找到符合条件的最小index
        static int tbl[26], answer = s.size();
        fill_n(tbl, 26, -1);
        for (int i = 0; i < s.size(); i++) {
            if (tbl[s[i]-'a'] == s.size())
                continue; // 重复出现
            if (tbl[s[i]-'a'] < 0)
                tbl[s[i]-'a'] = i; // 首次出现
            else
                tbl[s[i]-'a'] = s.size(); // 重复出现
        }
        for (int i = 0; i < 26; i++) {
            if (tbl[i] >= 0)
                answer = min(answer, tbl[i]);
        }
        return answer == s.size() ? -1 : answer;
    }
#endif
#if 0
    // 1. 遍历第一次: 记录每个字母出现的次数
    // 2. 遍历第二次: 查询当前位置字母的次数
    //    如果满足条件; 则返回索引
    int firstUniqChar(string s) {
        static int a[26];
        memset(a, 0, 26*sizeof(int));
        for (int i = 0; i < s.size(); ++i)
            a[s[i]-'a']++;
        for (int i = 0; i < s.size(); ++i)
            if (a[s[i]-'a'] == 1)
                return i;
        return -1;
    }
#endif
};
