/**
 * @file 给定不重复数字的数组, 和目标数值
 *       求所有 和 为目标数值的三个数组合
 * @note 00001 Two Sum.cpp 的 plus 版本
 */
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <prettyprint.hpp>
using namespace std;

void ThreeSumZero(vector<int> &a, int target) {
    // 1. 两两数值和, 做hash
    //    注意, hash的值, 应该是 set<vector>, 因为不同组合和可能相同
    // 2. 遍历每个数值, 求剩余数值的 hash 的 set
    //    遍历 set 中的每个组合
    //    条件 set 的 vector 的元素, 不同于 当前元素
    // 3. 把满足条件的组合 构造成 vector, 排序后插入 set 中
    // 4. 结果是 set
    map<int, set<vector<int>>> hash;
    set<vector<int>> mset;
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < i; j++) {
            vector<int> onep(2, 0);
            onep[0] = a[i];
            onep[1] = a[j];
            // 其实不用排序的, 因为相同组合不会重复
            sort(onep.begin(), onep.end()); 
            hash[a[i]+a[j]].insert(onep);
        }
    }
    for (int i = 0; i < a.size(); i++) {
        auto remain = hash.find(target-a[i]);
        if (remain != hash.end()) {
            vector<int> oneres(3, 0);
            for (auto & p: remain->second) {
                if (p[0] != a[i] && p[1] != a[i]) {
                    oneres[0] = p[0];
                    oneres[1] = p[1];
                    oneres[2] = a[i];
                }
            }
            sort(oneres.begin(), oneres.end());
            mset.insert(move(oneres));
        }
    }
    cout << mset << endl;
}

int main( int argc, char **argv ) {
    vector<int> a{ -1, -2, 3, 4, -3, 0};
    ThreeSumZero(a, 0);
    return 0;
}
