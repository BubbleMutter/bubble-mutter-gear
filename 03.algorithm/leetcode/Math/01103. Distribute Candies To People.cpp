/*
 * @lc app=leetcode id=1103 lang=cpp
 *
 * [1103] Distribute Candies to People
 *
 * https://leetcode.com/problems/distribute-candies-to-people/description/
 *
 * algorithms
 * Easy (63.64%)
 * Total Accepted:    65.7K
 * Total Submissions: 103.2K
 * Testcase Example:  '7\n4'
 *
 * We distribute some number of candies, to a row of n = num_people people in
 * the following way:
 *
 * We then give 1 candy to the first person, 2 candies to the second person,
 * and so on until we give n candies to the last person.
 *
 * Then, we go back to the start of the row, giving n + 1 candies to the first
 * person, n + 2 candies to the second person, and so on until we give 2 * n
 * candies to the last person.
 *
 * This process repeats (with us giving one more candy each time, and moving to
 * the start of the row after we reach the end) until we run out of candies.
 * The last person will receive all of our remaining candies (not necessarily
 * one more than the previous gift).
 *
 * Return an array (of length num_people and sum candies) that represents the
 * final distribution of candies.
 *
 *
 * Example 1:
 *
 *
 * Input: candies = 7, num_people = 4
 * Output: [1,2,3,1]
 * Explanation:
 * On the first turn, ans[0] += 1, and the array is [1,0,0,0].
 * On the second turn, ans[1] += 2, and the array is [1,2,0,0].
 * On the third turn, ans[2] += 3, and the array is [1,2,3,0].
 * On the fourth turn, ans[3] += 1 (because there is only one candy left), and
 * the final array is [1,2,3,1].
 *
 *
 * Example 2:
 *
 *
 * Input: candies = 10, num_people = 3
 * Output: [5,2,3]
 * Explanation:
 * On the first turn, ans[0] += 1, and the array is [1,0,0].
 * On the second turn, ans[1] += 2, and the array is [1,2,0].
 * On the third turn, ans[2] += 3, and the array is [1,2,3].
 * On the fourth turn, ans[0] += 4, and the final array is [5,2,3].
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= candies <= 10^9
 * 1 <= num_people <= 1000
 *
 *
 */
class Solution {
public:
    vector<int> distributeCandies(int candies, int num_people) {
        auto fnGaussianSum = [](const unsigned n) -> unsigned long {
            return ((n+1)*n)/2;
        };
        vector<int> res(num_people);
        // 1. 先计算最大可以有多少 round (高斯和公式)
        // 2. 计算比最大 round 数多出来的数值
        // 3. 根据 round 和 i 公式推倒出 res[i]
        // 4. 补上 扣除最大 round 后多出来的数值
        // 4.1 剩余的数值足够 next
        // 4.2 剩余的数值不够 next, 则全部补上
        int round = 1;
        while (candies >= fnGaussianSum(num_people*round)) {
            round++;
        }
        round--;
        candies -= fnGaussianSum(num_people*round);
        auto base = fnGaussianSum(round)*num_people;
        for (size_t i = 0, next = round*num_people+1; i < res.size(); ++i) {
            res[i] = base - round*(num_people-i-1);
            if (candies >= next) {
                res[i] += next;
                candies -= next;
                ++next;
            } else {
                res[i] += candies;
                candies = 0;
            }
        }
        return res;
    }
};


//  1,  2,  3,
//  4,  5,  6, ->  5,  7,  9
//  7,  8,  9, -> 12, 15, 18
//
// 已知 round = 3; 推倒每个数
//  7 = n-2 + (2n-2) + (3n-2) = ((1+3)*3/2)*n - 3*(size-i-1)
//                            = ((1+r)*r/2)*n - r*(size-i-1)
// 15 = n-1 + (2n-1) + (3n-1) = ((1+3)*3/2)*n - 3*(size-i-2)
//                            = ((1+r)*r/2)*n - r*(size-i-2)
// 18 = n-1 + (2n-1) + (3n-1) = ((1+3)*3/2)*n - 3*(size-i-3)
//                            = ((1+r)*r/2)*n - r*(size-i-3)
