/*
 * @lc app=leetcode id=1184 lang=cpp
 *
 * [1184] Distance Between Bus Stops
 *
 * https://leetcode.com/problems/distance-between-bus-stops/description/
 *
 * algorithms
 * Easy (54.02%)
 * Total Accepted:    40.2K
 * Total Submissions: 74.3K
 * Testcase Example:  '[1,2,3,4]\n0\n1'
 *
 * A bus has n stops numbered from 0 to n - 1 that form a circle. We know the
 * distance between all pairs of neighboring stops where distance[i] is the
 * distance between the stops number i and (i + 1) % n.
 * 
 * The bus goes along both directions i.e. clockwise and counterclockwise.
 * 
 * Return the shortest distance between the given start and destination
 * stops.
 * 
 * 
 * Example 1:
 * 
 * 
 * 
 * 
 * Input: distance = [1,2,3,4], start = 0, destination = 1
 * Output: 1
 * Explanation: Distance between 0 and 1 is 1 or 9, minimum is 1.
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * 
 * 
 * Input: distance = [1,2,3,4], start = 0, destination = 2
 * Output: 3
 * Explanation: Distance between 0 and 2 is 3 or 7, minimum is 3.
 * 
 * 
 * 
 * 
 * Example 3:
 * 
 * 
 * 
 * 
 * Input: distance = [1,2,3,4], start = 0, destination = 3
 * Output: 4
 * Explanation: Distance between 0 and 3 is 6 or 4, minimum is 4.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= n <= 10^4
 * distance.length == n
 * 0 <= start, destination < n
 * 0 <= distance[i] <= 10^4
 * 
 */
class Solution {
public:
    int distanceBetweenBusStops(vector<int>& distance, int start, int destination) {
        // 1. 首先调整 起点 和 终点, 使得起点 < 终点
        // 2. 计算 起点 ~ 终点 的累加
        // 3. 计算 0~起点 + 终点~end 的累加
        // 4. 比较
        if (start > destination) swap(start, destination);
        int forward = accumulate(distance.begin()+start, distance.begin()+destination, 0);
        int backward = accumulate(distance.begin(), distance.begin()+start, 0);
        backward = accumulate(distance.begin()+destination, distance.end(), backward);
        return min(forward, backward);
    }
};
