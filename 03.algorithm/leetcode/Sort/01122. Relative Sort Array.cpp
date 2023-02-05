/*
 * @lc app=leetcode id=1122 lang=cpp
 *
 * [1122] Relative Sort Array
 *
 * https://leetcode.com/problems/relative-sort-array/description/
 *
 * algorithms
 * Easy (68.01%)
 * Total Accepted:    119.8K
 * Total Submissions: 175.9K
 * Testcase Example:  '[2,3,1,3,2,4,6,7,9,2,19]\n[2,1,4,3,9,6]'
 *
 * Given two arrays arr1 and arr2, the elements of arr2 are distinct, and all
 * elements in arr2 are also in arr1.
 * 
 * Sort the elements of arr1 such that the relative ordering of items in arr1
 * are the same as in arr2. Elements that do not appear in arr2 should be
 * placed at the end of arr1 in ascending order.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr1 = [2,3,1,3,2,4,6,7,9,2,19], arr2 = [2,1,4,3,9,6]
 * Output: [2,2,2,1,4,3,3,9,6,7,19]
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: arr1 = [28,6,22,8,44,17], arr2 = [22,28,8,6]
 * Output: [22,28,8,6,17,44]
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= arr1.length, arr2.length <= 1000
 * 0 <= arr1[i], arr2[i] <= 1000
 * All the elements of arr2 are distinct.
 * Each arr2[i] is in arr1.
 * 
 * 
 */
class Solution {
public:
    vector<int> relativeSortArray(vector<int>& arr1, vector<int>& arr2) {
#if 0
        // 1. 先用 hash 法; 缓存 arr2 的数值相应的 index
        // 2. 计数排序法: 求得 arr1 中在 arr2 的数值的个数 count[]
        //                通过 1. 步骤的索引; 算的索引
        // 3. 同时把不属于 arr2 的数值取出 remain
        // 4. 计数排序法: 还原 count[] 到 结果
        // 5. 排序 remain[] 后把剩余结果升序放入结果
        //
        vector<int> count(arr2.size(), 0);
        map<int, size_t> index;
        vector<int> remain;
        for (size_t i = 0; i < arr2.size(); ++i) {
            index[arr2[i]] = i;
        }
        for (size_t i = 0; i < arr1.size(); ++i) {
            auto iter = index.find(arr1[i]);
            if (iter != index.end()) {
                count[iter->second]++;
            } else {
                remain.push_back(arr1[i]);
            }
        }
        vector<int> res(arr1.size());
        size_t k = 0;
        for (size_t i = 0; i < count.size(); ++i) {
            while (count[i]-- > 0) {
                res[k++] = arr2[i];
            }
        }
        sort(remain.begin(), remain.end());
        for (size_t i = 0; i < remain.size(); ++i) {
            res[k++] = remain[i];
        }
        return res;
#else
        // 6. 优化: 根据 arr[i] <= 1000 的特性;
        // 6.1 hash[1001] 作为 hash
        // 6.2 省略排序
        vector<int> hash(1001, 0);
        for (size_t i = 0; i < arr1.size(); ++i) {
            hash[arr1[i]]++;
        }
        int k = 0;
        for (size_t i = 0; i < arr2.size(); ++i) {
            while (hash[arr2[i]]--) {
                arr1[k++] = arr2[i];
            }
        }
        for (size_t i = 0; i < hash.size(); ++i) {
            while (hash[i]-- > 0) {
                arr1[k++] = i;
                cout << i << endl;
            }
        }
        return arr1;
#endif
    }
};
