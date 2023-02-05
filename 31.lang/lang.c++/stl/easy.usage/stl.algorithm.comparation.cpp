#include <algorithm>
#include <vector>
using namespace std;

void usage() {
    {
        vector<int> v;
        // 遍历取得最大元素
        int mmax = *max_element(v.begin(), v.end());
        // 遍历取得最小元素
        int mmin = *min_element(v.begin(), v.end());
    }
    {
        int a, b;
        // 比较区分较大值和较小值
        auto p = minmax(a, b);
        int mmin = p.first;
        int mmax = p.second;
    }
    {
        vector<int> x, y;
        // 双输入; 比较两个容器范围是否相同
        bool same1 = equal(x.begin(), x.end(), y.begin(), y.end());
        // 双输入;
        bool same2 = equal(x.begin(), x.end(), y.begin());
        // 双输入变单输入
        bool palindrome = equal(x.begin(), x.begin()+x.size()/2, x.rbegin());
    }
}
