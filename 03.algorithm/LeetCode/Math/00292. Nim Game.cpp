/*
 * @lc app=leetcode id=292 lang=cpp
 *
 * [292] Nim Game
 *
 * https://leetcode.com/problems/nim-game/description/
 *
 * algorithms
 * Easy (54.96%)
 * Total Accepted:    241.1K
 * Total Submissions: 437.5K
 * Testcase Example:  '4'
 *
 * You are playing the following Nim Game with your friend:
 * 
 * 
 * Initially, there is a heap of stones on the table.
 * You and your friend will alternate taking turns, and you go first.
 * On each turn, the person whose turn it is will remove 1 to 3 stones from the
 * heap.
 * The one who removes the last stone is the winner.
 * 
 * 
 * Given n, the number of stones in the heap, return true if you can win the
 * game assuming both you and your friend play optimally, otherwise return
 * false.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 4
 * Output: false
 * Explanation: These are the possible outcomes:
 * 1. You remove 1 stone. Your friend removes 3 stones, including the last
 * stone. Your friend wins.
 * 2. You remove 2 stones. Your friend removes 2 stones, including the last
 * stone. Your friend wins.
 * 3. You remove 3 stones. Your friend removes the last stone. Your friend
 * wins.
 * In all outcomes, your friend wins.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 1
 * Output: true
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: n = 2
 * Output: true
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= n <= 2^31 - 1
 * 
 * 
 */

class Solution {
public:
    /*
     * 数学推导
     * 1,2,3 you win
     *     4 you lost
     *     5 you win
     *     6 you win
     *     7 you win
     *     8 you lost
     *     9
     * 显然; 当数字不是4的倍数时;
     *       你取4的余;
     *       然后你的对手无论怎么选 (必然是 1~3)
     *       你取4的余; 使得对手只有 4的倍数数字
     *       那么你讲取得最后的 1~3 个数值
     */
    bool canWinNim(int n) {
        // 参考 01025. Divisor Game.cpp
        return n%4;
    }
};
