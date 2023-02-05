/*
 * @lc app=leetcode id=1154 lang=cpp
 *
 * [1154] Day of the Year
 *
 * https://leetcode.com/problems/day-of-the-year/description/
 *
 * algorithms
 * Easy (51.09%)
 * Total Accepted:    31.5K
 * Total Submissions: 61.8K
 * Testcase Example:  '"2019-01-09"'
 *
 * Given a string date representing a Gregorian calendar date formatted as
 * YYYY-MM-DD, return the day number of the year.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: date = "2019-01-09"
 * Output: 9
 * Explanation: Given date is the 9th day of the year in 2019.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: date = "2019-02-10"
 * Output: 41
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * date.length == 10
 * date[4] == date[7] == '-', and all other date[i]'s are digits
 * date represents a calendar date between Jan 1^st, 1900 and Dec 31^th, 2019.
 * 
 * 
 */
class Solution {
public:
    int dayOfYear(string date) {
        // 1. 取得年月日
        // 2. 计算前置累加月的天数和
        // 3. 首先加上 日数
        // 4. 如果大于1月, 则加上前置累加月的天数和
        // 5. 如果4的整倍数年, 大于2月份的月份多加1
        int year = stoul(date.substr(0, 4));
        int month = stoul(date.substr(5, 2));
        int day = stoul(date.substr(8, 2));
        vector<int> month_ndays {
            31,  59,  90,  120, 151, 181,
            212, 243, 273, 304, 334, 365,
        };
        int res = day;
        if (month > 1) {
            res += month_ndays[month-2];
        }
        if (month > 2 && ((year % 100 == 0) ? (year % 400 == 0) : (year % 4 == 0))) {
            res += 1;
        }
        return res;
    }
};
