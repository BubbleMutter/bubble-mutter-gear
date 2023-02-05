/*
 * @lc app=leetcode id=1030 lang=cpp
 *
 * [1030] Matrix Cells in Distance Order
 *
 * https://leetcode.com/problems/matrix-cells-in-distance-order/description/
 *
 * algorithms
 * Easy (68.85%)
 * Total Accepted:    39.8K
 * Total Submissions: 57.8K
 * Testcase Example:  '1\n2\n0\n0'
 *
 * You are given four integers row, cols, rCenter, and cCenter. There is a rows
 * x cols matrix and you are on the cell with the coordinates (rCenter,
 * cCenter).
 *
 * Return the coordinates of all cells in the matrix, sorted by their distance
 * from (rCenter, cCenter) from the smallest distance to the largest distance.
 * You may return the answer in any order that satisfies this condition.
 *
 * The distance between two cells (r1, c1) and (r2, c2) is |r1 - r2| + |c1 -
 * c2|.
 *
 *
 * Example 1:
 *
 *
 * Input: rows = 1, cols = 2, rCenter = 0, cCenter = 0
 * Output: [[0,0],[0,1]]
 * Explanation: The distances from (0, 0) to other cells are: [0,1]
 *
 *
 * Example 2:
 *
 *
 * Input: rows = 2, cols = 2, rCenter = 0, cCenter = 1
 * Output: [[0,1],[0,0],[1,1],[1,0]]
 * Explanation: The distances from (0, 1) to other cells are: [0,1,1,2]
 * The answer [[0,1],[1,1],[0,0],[1,0]] would also be accepted as correct.
 *
 *
 * Example 3:
 *
 *
 * Input: rows = 2, cols = 3, rCenter = 1, cCenter = 2
 * Output: [[1,2],[0,2],[1,1],[0,1],[1,0],[0,0]]
 * Explanation: The distances from (1, 2) to other cells are: [0,1,1,2,2,3]
 * There are other answers that would also be accepted as correct, such as
 * [[1,2],[1,1],[0,2],[1,0],[0,1],[0,0]].
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= rows, cols <= 100
 * 0 <= rCenter < rows
 * 0 <= cCenter < cols
 *
 *
 */
class Solution {
public:
    vector<vector<int>> allCellsDistOrder(int rows, int cols, int rCenter, int cCenter) {
        vector<vector<int>> res;
        vector<int> point { rCenter, cCenter };
        res.push_back(point);
        int distance = 0;
        size_t size = 0;
        do {
            size = res.size();
            distance++;
            static const vector<vector<int>> direction {
                { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }
            };
            for (int i = 0; i <= distance; ++i) {
                int j = distance - i;
                set<vector<int>> pset;
                for (size_t k = 0; k < direction.size(); ++k) {
                    point[0] = rCenter + i*direction[k][0];
                    point[1] = cCenter + j*direction[k][1];
                    if (isPointValid(rows, cols, point))
                        pset.insert(point);
                }
                // 垂直和水平方向扩展时; 会有重复的点; 简单处理用set
                for (const auto& p: pset) {
                    res.push_back(p);
                }
            }
        } while (size != res.size());
        return res;
    }
private:
    bool isPointValid(int rows, int cols, const vector<int>& point) {
        return (0 <= point[0] && point[0] < rows) && (0 <= point[1] && point[1] < cols);
    }
};
