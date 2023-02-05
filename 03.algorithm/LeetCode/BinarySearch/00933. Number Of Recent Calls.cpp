/*
 * @lc app=leetcode id=933 lang=cpp
 *
 * [933] Number of Recent Calls
 *
 * https://leetcode.com/problems/number-of-recent-calls/description/
 *
 * algorithms
 * Easy (72.97%)
 * Total Accepted:    117K
 * Total Submissions: 160.2K
 * Testcase Example:  '["RecentCounter","ping","ping","ping","ping"]\n[[],[1],[100],[3001],[3002]]'
 *
 * You have a RecentCounter class which counts the number of recent requests
 * within a certain time frame.
 *
 * Implement the RecentCounter class:
 *
 *
 * RecentCounter() Initializes the counter with zero recent requests.
 * int ping(int t) Adds a new request at time t, where t represents some time
 * in milliseconds, and returns the number of requests that has happened in the
 * past 3000 milliseconds (including the new request). Specifically, return the
 * number of requests that have happened in the inclusive range [t - 3000,
 * t].
 * 
 * 
 * It is guaranteed that every call to ping uses a strictly larger value of t
 * than the previous call.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input
 * ["RecentCounter", "ping", "ping", "ping", "ping"]
 * [[], [1], [100], [3001], [3002]]
 * Output
 * [null, 1, 2, 3, 3]
 * 
 * Explanation
 * RecentCounter recentCounter = new RecentCounter();
 * recentCounter.ping(1);     // requests = [1], range is [-2999,1], return 1
 * recentCounter.ping(100);   // requests = [1, 100], range is [-2900,100],
 * return 2
 * recentCounter.ping(3001);  // requests = [1, 100, 3001], range is [1,3001],
 * return 3
 * recentCounter.ping(3002);  // requests = [1, 100, 3001, 3002], range is
 * [2,3002], return 3
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= t <= 10^9
 * Each test case will call ping with strictly increasing values of t.
 * At most 10^4 calls will be made to ping.
 * 
 * 
 */
class RecentCounter {
public:
    RecentCounter() {
        previous = 0;
    }

    int ping(int t) {
        // 1. t 是递增地传入, 所以构造排序后的升序数组
        // 2. 二分查找法; 找到 最小的 大于等于 t-3000 的数的索引
        // 3. 结果是 size - 索引
        // 4. 二分查找的最小位置, 是上次的结果
        //    因此, 得到结果后, 缓存起来, 作为下次 ping 的二分查找最小位置
        //    (optimize)
        requests.push_back(t);
        int i = previous, j = requests.size()-1;
        int k = j; // 新入列的数值必然属于 [t-3000, t] 的范围
        while (i <= j) {
            int m = i + (j-i)/2;
            if (t-3000 <= requests[m]) {
                k = m;
                j = m-1;
            } else {
                i = m+1;
            }
        }
        previous = k;
        return requests.size() - k;
    }
private:
    vector<int> requests;
    int previous;
};
/**
 * Your RecentCounter object will be instantiated and called as such:
 * RecentCounter* obj = new RecentCounter();
 * int param_1 = obj->ping(t);
 */
