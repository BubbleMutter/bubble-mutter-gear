/*
 * @lc app=leetcode id=1275 lang=cpp
 *
 * [1275] Find Winner on a Tic Tac Toe Game
 *
 * https://leetcode.com/problems/find-winner-on-a-tic-tac-toe-game/description/
 *
 * algorithms
 * Easy (55.45%)
 * Total Accepted:    67.5K
 * Total Submissions: 122.6K
 * Testcase Example:  '[[0,0],[2,0],[1,1],[2,1],[2,2]]'
 *
 * Tic-tac-toe is played by two players A and B on a 3 x 3 grid. The rules of
 * Tic-Tac-Toe are:
 * 
 * 
 * Players take turns placing characters into empty squares ' '.
 * The first player A always places 'X' characters, while the second player B
 * always places 'O' characters.
 * 'X' and 'O' characters are always placed into empty squares, never on filled
 * ones.
 * The game ends when there are three of the same (non-empty) character filling
 * any row, column, or diagonal.
 * The game also ends if all squares are non-empty.
 * No more moves can be played if the game is over.
 * 
 * 
 * Given a 2D integer array moves where moves[i] = [rowi, coli] indicates that
 * the i^th move will be played on grid[rowi][coli]. return the winner of the
 * game if it exists (A or B). In case the game ends in a draw return "Draw".
 * If there are still movements to play return "Pending".
 * 
 * You can assume that moves is valid (i.e., it follows the rules of
 * Tic-Tac-Toe), the grid is initially empty, and A will play first.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: moves = [[0,0],[2,0],[1,1],[2,1],[2,2]]
 * Output: "A"
 * Explanation: A wins, they always play first.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: moves = [[0,0],[1,1],[0,1],[0,2],[1,0],[2,0]]
 * Output: "B"
 * Explanation: B wins.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: moves = [[0,0],[1,1],[2,0],[1,0],[1,2],[2,1],[0,1],[0,2],[2,2]]
 * Output: "Draw"
 * Explanation: The game ends in a draw since there are no moves to make.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= moves.length <= 9
 * moves[i].length == 2
 * 0 <= rowi, coli <= 2
 * There are no repeated elements on moves.
 * moves follow the rules of tic tac toe.
 * 
 * 
 */
class Solution {
public:
    string tictactoe(vector<vector<int>>& moves) {
        // 1. 不够5, 必然是 pending
        // 2. 根据 moves 填满棋盘
        // 3. 每行, 每列, 两个对角线判断是否全等
        // 4. 最后如果是 9 则 Draw 否则 Pending
        if (moves.size() < 5)  return "Pending";
        vector<vector<int>> mat(3, vector<int>(3, 0));
        for (size_t i = 0; i < moves.size(); ++i) {
            const auto &p = moves[i];
            mat[moves[i][0]][moves[i][1]] = (i & 1) + 1;
        }
        for (size_t i = 0; i < 3; ++i) {
            if (mat[i][0] == mat[i][1] && mat[i][1] == mat[i][2]) {
                if (mat[i][0] == 1) return "A";
                if (mat[i][0] == 2) return "B";
            }
            if (mat[0][i] == mat[1][i] && mat[1][i] == mat[2][i]) {
                if (mat[0][i] == 1) return "A";
                if (mat[0][i] == 2) return "B";
            }
        }
        if (mat[0][0] == mat[1][1] && mat[1][1] == mat[2][2]) {
            if (mat[0][0] == 1) return "A";
            if (mat[0][0] == 2) return "B";
        }
        if (mat[0][2] == mat[1][1] && mat[1][1] == mat[2][0]) {
            if (mat[0][2] == 1) return "A";
            if (mat[0][2] == 2) return "B";
        }
        return (moves.size() == 9) ? "Draw" : "Pending";
    }
};
