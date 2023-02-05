/*
 * @lc app=leetcode id=219 lang=cpp
 *
 * [219] Contains Duplicate II
 *
 * https://leetcode.com/problems/contains-duplicate-ii/description/
 *
 * algorithms
 * Easy (38.56%)
 * Total Accepted:    320K
 * Total Submissions: 828.7K
 * Testcase Example:  '[1,2,3,1]\n3'
 *
 * Given an array of integers and an integer k, find out whether there are two
 * distinct indices i and j in the array such that nums[i] = nums[j] and the
 * absolute difference between i and j is at most k.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [1,2,3,1], k = 3
 * Output: true
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [1,0,1,1], k = 1
 * Output: true
 * 
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: nums = [1,2,3,1,2,3], k = 2
 * Output: false
 * 
 * 
 * 
 * 
 * 
 */
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
#if 0 /* 全缓存; 算距离 */
        map<int, int> pos; // k: num, v: idx
        for (int i = 0; i < nums.size(); i++) {
            auto last = pos.find(nums[i]);
            if (last != pos.end() &&
                i - last->second <= k)
                return true;
            pos[nums[i]] = i;
        }
        return false;
#endif

#if 1 /* 题解优化: 窗口法; */
        // 1. 动态维护前k个的集合
        // 2. 先判断在不在 集合中;
        //    在 return true;
        // 3. 干掉 nums[i-k-1]
        // 4. 插入 nums[i]
        set<int> window;
        if (k == 0) return false;
        for (int i = 0; i < nums.size(); i++) {
            auto retval = window.insert(nums[i]);
            if (retval.second == false)
                return true;
            if (i >= k)
                window.erase(nums[i-k]);
        }
        return false;
#endif
    }
};
