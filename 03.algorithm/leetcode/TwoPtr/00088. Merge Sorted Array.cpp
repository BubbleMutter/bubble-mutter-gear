/*
 * @lc app=leetcode id=88 lang=cpp
 *
 * [88] Merge Sorted Array
 *
 * https://leetcode.com/problems/merge-sorted-array/description/
 *
 * algorithms
 * Easy (40.65%)
 * Total Accepted:    807.7K
 * Total Submissions: 2M
 * Testcase Example:  '[1,2,3,0,0,0]\n3\n[2,5,6]\n3'
 *
 * Given two sorted integer arrays nums1 and nums2, merge nums2 into nums1 as
 * one sorted array.
 * 
 * The number of elements initialized in nums1 and nums2 are m and n
 * respectively. You may assume that nums1 has a size equal to m + n such that
 * it has enough space to hold additional elements from nums2.
 * 
 * 
 * Example 1:
 * Input: nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
 * Output: [1,2,2,3,5,6]
 * Example 2:
 * Input: nums1 = [1], m = 1, nums2 = [], n = 0
 * Output: [1]
 * 
 * 
 * Constraints:
 * 
 * 
 * nums1.length == m + n
 * nums2.length == n
 * 0 <= m, n <= 200
 * 1 <= m + n <= 200
 * -10^9 <= nums1[i], nums2[i] <= 10^9
 * 
 * 
 */
class Solution {
public:
    // 1. 分别从A, B 的 m, n 往前遍历;
    // 2. 把较大的 A[i] or B[j] 放到A的末尾
    // 3. 把B[0:n) 剩下的; 继续放到前面A的前面
    //    (实际上A[0:m)如果有剩下, 那么一定是排序好的

    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {

        int k = nums1.size()-1;
        int i = m-1, j = n-1;
        while (i >= 0 && j >= 0) {
            if (nums1[i] > nums2[j])
                nums1[k--] = nums1[i--];
            else
                nums1[k--] = nums2[j--];
        }

#if 0
        // 这里可以优化掉; 因为跑到这里
        // 说明 nums2 全部跑完了;
        // 那么 nums1 前面的都是已经排序好的
        while (i >= 0)
            nums1[k--] = nums1[i--];
#endif

        while (j >= 0)
            nums1[k--] = nums2[j--];
    }
};
