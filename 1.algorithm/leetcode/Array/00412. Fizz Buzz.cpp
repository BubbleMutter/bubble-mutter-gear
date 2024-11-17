/*
 * @lc app=leetcode id=412 lang=cpp
 *
 * [412] Fizz Buzz
 *
 * https://leetcode.com/problems/fizz-buzz/description/
 *
 * algorithms
 * Easy (65.90%)
 * Total Accepted:    570.2K
 * Total Submissions: 864.5K
 * Testcase Example:  '3'
 *
 * Given an integer n, return a string array answer (1-indexed) where:
 * 
 * 
 * answer[i] == "FizzBuzz" if i is divisible by 3 and 5.
 * answer[i] == "Fizz" if i is divisible by 3.
 * answer[i] == "Buzz" if i is divisible by 5.
 * answer[i] == i (as a string) if none of the above conditions are true.
 * 
 * 
 * 
 * Example 1:
 * Input: n = 3
 * Output: ["1","2","Fizz"]
 * Example 2:
 * Input: n = 5
 * Output: ["1","2","Fizz","4","Buzz"]
 * Example 3:
 * Input: n = 15
 * Output:
 * ["1","2","Fizz","4","Buzz","Fizz","7","8","Fizz","Buzz","11","Fizz","13","14","FizzBuzz"]
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= n <= 10^4
 * 
 * 
 */
class Solution {
public:
    vector<string> fizzBuzz(int n) {
        vector<string> res;
        for (int i = 0, fizz = 0, buzz = 0; i < n; ++i) {
            fizz++;
            buzz++;
            if (fizz == 3 && buzz == 5) {
                res.push_back("FizzBuzz");
                fizz = buzz = 0;
            } else if (fizz == 3) {
                res.push_back("Fizz");
                fizz = 0;
            } else if (buzz == 5) {
                res.push_back("Buzz");
                buzz = 0;
            } else {
                res.push_back(to_string(i+1));
            }
        }
        return res;
    }
};
