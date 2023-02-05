/*
 * @lc app=leetcode id=599 lang=cpp
 *
 * [599] Minimum Index Sum of Two Lists
 *
 * https://leetcode.com/problems/minimum-index-sum-of-two-lists/description/
 *
 * algorithms
 * Easy (52.93%)
 * Total Accepted:    139.9K
 * Total Submissions: 264.4K
 * Testcase Example:  '["Shogun","Tapioca Express","Burger King","KFC"]\n["Piatti","The Grill at Torrey Pines","Hungry Hunter Steakhouse","Shogun"]'
 *
 * Suppose Andy and Doris want to choose a restaurant for dinner, and they both
 * have a list of favorite restaurants represented by strings.
 *
 * You need to help them find out their common interest with the least list
 * index sum. If there is a choice tie between answers, output all of them with
 * no order requirement. You could assume there always exists an answer.
 *
 *
 * Example 1:
 *
 *
 * Input: list1 = ["Shogun","Tapioca Express","Burger King","KFC"], list2 =
 * ["Piatti","The Grill at Torrey Pines","Hungry Hunter Steakhouse","Shogun"]
 * Output: ["Shogun"]
 * Explanation: The only restaurant they both like is "Shogun".
 *
 *
 * Example 2:
 *
 *
 * Input: list1 = ["Shogun","Tapioca Express","Burger King","KFC"], list2 =
 * ["KFC","Shogun","Burger King"]
 * Output: ["Shogun"]
 * Explanation: The restaurant they both like and have the least index sum is
 * "Shogun" with index sum 1 (0+1).
 *
 *
 *
 * Constraints:
 *
 *
 * 1 <= list1.length, list2.length <= 1000
 * 1 <= list1[i].length, list2[i].length <= 30
 * list1[i] and list2[i] consist of spaces ' ' and English letters.
 * All the stings of list1 are unique.
 * All the stings of list2 are unique.
 *
 *
 */
class Solution {
public:
    vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
        vector<string> res;
        map<string, int> index1, index2;
        int least_index_sum = list1.size() + list2.size();
        // 构造餐馆索引映射表
        for (size_t i = 0; i < list1.size(); ++i)
            index1[list1[i]] = i;
        for (size_t i = 0; i < list2.size(); ++i) {
            // 查找共同喜欢的餐馆
            auto iter = index1.find(list2[i]);
            if (iter != index1.end()) {
                // 求索引和
                if (iter->second+i < least_index_sum) {
                    // 索引和 更小; 更新结果
                    least_index_sum = iter->second+i;
                    res.clear();
                    res.push_back(list2[i]);
                } else if (iter->second+i == least_index_sum) {
                    // 索引和 相同; 新增餐馆
                    res.push_back(list2[i]);
                }
            }
        }
        return res;
    }
};
