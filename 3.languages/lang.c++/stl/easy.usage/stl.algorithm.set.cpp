#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <numeric>
#include <prettyprint.hpp>
using namespace std;
void usage();
int main( int argc, char **argv ) {
    usage();
    return 0;
}

void usage() {
    vector<int> x, y;
    // set 接口簇 只能处理排序后的两个输入源
    sort(x.begin(), x.end());
    sort(y.begin(), y.end());
    {
        // 双输入container; 观察y是否在x中;
        bool x_includes_y = includes(x.begin(), x.end(), y.begin(), y.end());
    }
    {
        // 双输入container;
        // x 关于 "x与y 的交集" 的补集
        vector<int> z;
        set_difference(x.begin(), x.end(), y.begin(), y.end(), back_inserter(z));
    }
    {
        // 双输入源; 求交集合
        vector<int> z;
        set_intersection(x.begin(), x.end(), y.begin(), y.end(), back_inserter(z));
        /*
         * 同个元素 [first1, last1) 出现 m 次
        *           [first2, last2) 出现 n 次
         * only return min(m, n) elements
         */
    }
    {
        // 双输入源; 求并集
        /*
         * 同个元素 [first1, last1) 出现 m 次
         *          [first2, last2) 出现 n 次
         * only return max(m, n) elements (but not m+n)
         *
         * Note: 几乎形同 std::merge()
         *       除了merge 函数针对多次出现的相同元素
         *       返回 m+n 次
         */
        vector<int> z;
        set_union(x.begin(), x.end(), y.begin(), y.end(), back_inserter(z));
    }
    {
        // 双输入源; 求对称差; 实际上是 "并集-交集" 的结果
        vector<int> z;
        set_symmetric_difference(x.begin(), x.end(), y.begin(), y.end(), back_inserter(z));
    }
}
