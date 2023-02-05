/*
 * @lc app=leetcode id=1005 lang=cpp
 *
 * [1005] Maximize Sum Of Array After K Negations
 *
 * https://leetcode.com/problems/maximize-sum-of-array-after-k-negations/description/
 *
 * algorithms
 * Easy (51.71%)
 * Total Accepted:    50.9K
 * Total Submissions: 98.7K
 * Testcase Example:  '[4,2,3]\n1'
 *
 * Given an integer array nums and an integer k, modify the array in the
 * following way:
 * 
 * 
 * choose an index i and replace nums[i] with -nums[i].
 * 
 * 
 * You should apply this process exactly k times. You may choose the same index
 * i multiple times.
 * 
 * Return the largest possible sum of the array after modifying it in this
 * way.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [4,2,3], k = 1
 * Output: 5
 * Explanation: Choose index 1 and nums becomes [4,-2,3].
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [3,-1,0,2], k = 3
 * Output: 6
 * Explanation: Choose indices (1, 2, 2) and nums becomes [3,1,0,2].
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: nums = [2,-3,-1,5,-4], k = 2
 * Output: 13
 * Explanation: Choose indices (1, 4) and nums becomes [2,3,-1,5,4].
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= nums.length <= 10^4
 * -100 <= nums[i] <= 100
 * 1 <= k <= 10^4
 *
 *
 */
class Solution {
public:
    // 1. 排序
    // 2. 遍历
    // 2.1 如果还有翻转次数 且 A[i] < 0；那就翻转后累加
    // 2.2 否则; 直接累加
    // 2.3 翻转过程中; 记录绝对值最小的索引
    // 3. 如果还剩下 奇数次翻转
    // 3.1 那就把绝对值最小的数还原 再 累加 (即减两次)
    int largestSumAfterKNegations(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int sum = 0, min_abs = abs(nums[0]);
        // 1. 如果 A[] 中的负数个数小于 K
        //    那么尽可能地把较大的负数 转换为正数
        // 2. 如果 A[] 中的已经全部为正数
        //    那么检查 K 的奇偶
        //    K 为偶数; 则不再做翻转  （可以连续翻转同一个数 消耗K)
        //    K 为奇数; 找到最小的翻转
        for (size_t i = 0; i < nums.size(); ++i) {
            if (nums[i] < 0 && k > 0) {
                // 还有翻转次数; 那就把较小的负数翻转
                sum += -nums[i];
                --k;
            } else {
                sum += nums[i];
            }
            min_abs = min(abs(nums[i]), min_abs);
        }
        // '2*A[min_abs]' 有两层
        // 先把之前累加的结果还原
        // 再把原始地数值累加进去
        if (k & 0x1) {
            sum -= 2*min_abs;
        }
        return sum;
    }
};
