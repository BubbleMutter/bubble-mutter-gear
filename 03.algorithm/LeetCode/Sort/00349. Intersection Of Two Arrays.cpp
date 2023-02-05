/*
 * @lc app=leetcode id=349 lang=cpp
 *
 * [349] Intersection of Two Arrays
 *
 * https://leetcode.com/problems/intersection-of-two-arrays/description/
 *
 * algorithms
 * Easy (64.59%)
 * Total Accepted:    455.3K
 * Total Submissions: 701.2K
 * Testcase Example:  '[1,2,2,1]\n[2,2]'
 *
 * Given two arrays, write a function to compute their intersection.
 * 
 * Example 1:
 * 
 * 
 * Input: nums1 = [1,2,2,1], nums2 = [2,2]
 * Output: [2]
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums1 = [4,9,5], nums2 = [9,4,9,8,4]
 * Output: [9,4]
 * 
 * 
 * Note:
 * 
 * 
 * Each element in the result must be unique.
 * The result can be in any order.
 * 
 * 
 * 
 * 
 */
class Solution {
public:
#if 0
    // 哈希法
    // Time: O(n+m) Space: O(n+m)
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        /**
         * 1. 遍历 nums1 hash
         * 2. 遍历 nums2; 每个能再 hash 中找到的值; 加入答案中
         */
        set<int> hash1(nums1.begin(), nums1.end()), hash2;
        for (auto n : nums2 ) {
            if (hash1.count(n)) {
                hash2.insert(n);
            }
        }
        return vector<int>(hash2.begin(), hash2.end());
    }
#endif
#if 1 // 排序后的双指针法; 用时间换空间
    // Time: O(n+m+log2n+log2m);  Space: O(1)
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // 1. 排序
        // 2. 双指针法: 指针j 和 指针i 定义为 nums1[i] < nums2[j]
        // 3.1 如果相等 则加入到交集; (加入时判断是否重复; 因为已排序 只需要判断交集当前的最大值是否相等即可)
        // 3.2 nums1[i] < nums2[j] 符合约定: i++; 即i指向更大的数
        // 3.3 nums1[i] > nums2[j] 约定不合; 尝试继续保证约定; 使得j指向更大地数; ++j
        vector<int> intersection;

        sort(nums1.begin(), nums1.end());
        sort(nums2.begin(), nums2.end());

        int i = 0, j = 0;
        while (i < nums1.size() && j < nums2.size()) {
            // 3.1
            if (nums1[i] == nums2[j]) {
                if (intersection.empty() || nums1[i] != intersection.back())
                    intersection.push_back(nums1[i]);
                ++i;
                ++j;
            }
            else if (nums1[i] < nums2[j]) // 3.1
                ++i;
            else if (nums1[i] > nums2[j]) // 3.2
                ++j;
        }
        return intersection;
    }
#endif

};
