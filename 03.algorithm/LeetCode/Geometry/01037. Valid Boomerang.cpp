/*
 * @lc app=leetcode id=1037 lang=cpp
 *
 * [1037] Valid Boomerang
 *
 * https://leetcode.com/problems/valid-boomerang/description/
 *
 * algorithms
 * Easy (37.50%)
 * Total Accepted:    29.9K
 * Total Submissions: 79.9K
 * Testcase Example:  '[[1,1],[2,3],[3,2]]'
 *
 * Given an array points where points[i] = [xi, yi] represents a point on the
 * X-Y plane, return true if these points are a boomerang.
 *
 * A boomerang is a set of three points that are all distinct and not in a
 * straight line.
 *
 *
 * Example 1:
 * Input: points = [[1,1],[2,3],[3,2]]
 * Output: true
 * Example 2:
 * Input: points = [[1,1],[2,2],[3,3]]
 * Output: false
 *
 *
 * Constraints:
 *
 *
 * points.length == 3
 * points[i].length == 2
 * 0 <= xi, yi <= 100
 *
 *
 */
class Solution {
public:
    bool isBoomerang(vector<vector<int>>& points) {
        // 平面几何的问题
        // 给定3个点判断是否同一直线
        //
        // (y1-y0)/(x1-x0) = (y2-y0)/(x2-x0)
        // 因为计算机的整数除法 有精度问题
        // 所以变换为乘法公式
        // (y1-y0)*(x2-x0) = (y2-y0)*(x1-x0)
        //
        // 另一个斜率公式的应用:
        // 01232. Check If It Is A Straight Line.cpp
        int x0 = points[0][0], y0 = points[0][1];
        int x1 = points[1][0], y1 = points[1][1];
        int x2 = points[2][0], y2 = points[2][1];
        return points[0] != points[1] && points[0] != points[2] &&
            (y1-y0)*(x2-x0) != (y2-y0)*(x1-x0);
    }
};
