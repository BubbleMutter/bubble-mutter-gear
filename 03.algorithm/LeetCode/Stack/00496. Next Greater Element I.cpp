/*
 * @lc app=leetcode id=496 lang=cpp
 *
 * [496] Next Greater Element I
 *
 * https://leetcode.com/problems/next-greater-element-i/description/
 *
 * algorithms
 * Easy (69.25%)
 * Total Accepted:    313.1K
 * Total Submissions: 451.7K
 * Testcase Example:  '[4,1,2]\n[1,3,4,2]'
 *
 * The next greater element of some element x in an array is the first greater
 * element that is to the right of x in the same array.
 *
 * You are given two distinct 0-indexed integer arrays nums1 and nums2, where
 * nums1 is a subset of nums2.
 *
 * For each 0 <= i < nums1.length, find the index j such that nums1[i] ==
 * nums2[j] and determine the next greater element of nums2[j] in nums2. If
 * there is no next greater element, then the answer for this query is -1.
 *
 * Return an array ans of length nums1.length such that ans[i] is the next
 * greater element as described above.
 *
 *
 * Example 1:
 *
 *
 * Input: nums1 = [4,1,2], nums2 = [1,3,4,2]
 * Output: [-1,3,-1]
 * Explanation: The next greater element for each value of nums1 is as follows:
 * - 4 is underlined in nums2 = [1,3,4,2]. There is no next greater element, so
 * the answer is -1.
 * - 1 is underlined in nums2 = [1,3,4,2]. The next greater element is 3.
 * - 2 is underlined in nums2 = [1,3,4,2]. There is no next greater element, so
 * the answer is -1.
 *
 * 
 * Example 2:
 * 
 * 
 * Input: nums1 = [2,4], nums2 = [1,2,3,4]
 * Output: [3,-1]
 * Explanation: The next greater element for each value of nums1 is as follows:
 * - 2 is underlined in nums2 = [1,2,3,4]. The next greater element is 3.
 * - 4 is underlined in nums2 = [1,2,3,4]. There is no next greater element, so
 * the answer is -1.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= nums1.length <= nums2.length <= 1000
 * 0 <= nums1[i], nums2[i] <= 10^4
 * All integers in nums1 and nums2 are unique.
 * All the integers of nums1 also appear in nums2.
 * 
 * 
 * 
 * Follow up: Could you find an O(nums1.length + nums2.length) solution?
 */
class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        // 1. 维护一个单调递减的栈 (栈中的元素即还没有找到 下一个比ta大地元素)
        // 2. 当前元素比 栈顶要大; 则栈顶元素找到下一个较大元素;
        // 3. 出栈; 通过索引hash 把结果缓存到对应地方
        // 4. 找完之后; 当前元素继续入栈
        map<int, int> next;
        stack<int> decreaseStack;
        for (auto it = nums2.begin(); it != nums2.end(); ++it) {
            while (!decreaseStack.empty() && decreaseStack.top() < *it) {
                next[decreaseStack.top()] = *it;
                decreaseStack.pop();
            }
            // 找完之后; 当前元素继续入栈
            decreaseStack.push(*it);
        }
        // 输出结果
        vector<int> res(nums1.size(), -1); // 默认找不到
        for (size_t i = 0; i < nums1.size(); ++i) {
            auto it = next.find(nums1[i]);
            if (it != next.end()) {
                res[i] = it->second;
            }
        }
        return res;
    }
};
