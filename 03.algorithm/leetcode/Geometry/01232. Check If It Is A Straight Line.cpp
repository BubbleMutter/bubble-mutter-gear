/*
 * @lc app=leetcode id=1232 lang=cpp
 *
 * [1232] Check If It Is a Straight Line
 *
 * https://leetcode.com/problems/check-if-it-is-a-straight-line/description/
 *
 * algorithms
 * Easy (42.22%)
 * Total Accepted:    106.1K
 * Total Submissions: 251.8K
 * Testcase Example:  '[[1,2],[2,3],[3,4],[4,5],[5,6],[6,7]]'
 *
 * You are given an array coordinates, coordinates[i] = [x, y], where [x, y]
 * represents the coordinate of a point. Check if these points make a straight
 * line in the XY plane.
 *
 *
 *
 *
 * Example 1:
 *
 *
 *
 *
 * Input: coordinates = [[1,2],[2,3],[3,4],[4,5],[5,6],[6,7]]
 * Output: true
 *
 *
 * Example 2:
 *
 *
 *
 *
 * Input: coordinates = [[1,1],[2,2],[3,4],[4,5],[5,6],[7,7]]
 * Output: false
 *
 *
 *
 * Constraints:
 *
 *
 * 2 <= coordinates.length <= 1000
 * coordinates[i].length == 2
 * -10^4 <= coordinates[i][0], coordinates[i][1] <= 10^4
 * coordinates contains no duplicate point.
 *
 */
class Solution {
public:
    bool checkStraightLine(vector<vector<int>>& coordinates) {
        // 1. 根据斜率公式:
        //    (y1-y0) / (x1-x0) = (y2-y0) / (x2-x0)
        // 2. 从第三个座标点开始遍历, 套入公式
        int x0 = coordinates[0][0],
            y0 = coordinates[0][1],
            x1 = coordinates[1][0],
            y1 = coordinates[1][1];
        for (size_t i = 2; i < coordinates.size(); ++i) {
            int x2 = coordinates[i][0], y2 = coordinates[i][1];
            if ( (y1-y0)*(x2-x0) != (y2-y0)*(x1-x0) ) {
                return false;
            }
        }
        return true;
    }
};
