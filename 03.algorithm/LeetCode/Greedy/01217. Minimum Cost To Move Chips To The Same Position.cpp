/*
 * @lc app=leetcode id=1217 lang=cpp
 *
 * [1217] Minimum Cost to Move Chips to The Same Position
 *
 * https://leetcode.com/problems/minimum-cost-to-move-chips-to-the-same-position/description/
 *
 * algorithms
 * Easy (72.79%)
 * Total Accepted:    92.7K
 * Total Submissions: 127.4K
 * Testcase Example:  '[1,2,3]'
 *
 * We have n chips, where the position of the i^th chip is position[i].
 *
 * We need to move all the chips to the same position. In one step, we can
 * change the position of the i^th chip from position[i] to:
 *
 *
 * position[i] + 2 or position[i] - 2 with cost = 0.
 * position[i] + 1 or position[i] - 1 with cost = 1.
 * 
 * 
 * Return the minimum cost needed to move all the chips to the same
 * position.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: position = [1,2,3]
 * Output: 1
 * Explanation: First step: Move the chip at position 3 to position 1 with cost
 * = 0.
 * Second step: Move the chip at position 2 to position 1 with cost = 1.
 * Total cost is 1.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: position = [2,2,2,3,3]
 * Output: 2
 * Explanation: We can move the two chips at position  3 to position 2. Each
 * move has cost = 1. The total cost = 2.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: position = [1,1000000000]
 * Output: 1
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= position.length <= 100
 * 1 <= position[i] <= 10^9
 * 
 * 
 */
class Solution {
public:
    int minCostToMoveChips(vector<int>& position) {
        // 1. 所有筹码都可以 无损 地移动到 0或1
        // 2. 然后判断 0 和 1 的个数谁更少
        // 3. 0 更少则把0移到1
        // 4. 1 更少则把1移到0
        // 5. 即判断 position[] 奇偶个数谁更少
        size_t count_odd = count_if(position.cbegin(), position.cend(), [](const int val) { return val & 1; });
        return min(count_odd, position.size() - count_odd);
    }
};
