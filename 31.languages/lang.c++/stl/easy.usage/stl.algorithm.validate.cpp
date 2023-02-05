#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <prettyprint.hpp>
using namespace std;

void usage_transform() {
    vector<int> a(3, 0); a[0] = 1; a[1] = 2; a[2] = 3;
    vector<int> b(3, 0); b[0] = 10; b[1] = 20; b[2] = 30;
    vector<int> c;

    // 单输入源; 输出指向输入; 形同 {a} += 1;
    transform(a.begin(), a.end(), a.begin(), [](int n) { return n+1; });
    cout << a << endl; // [2, 3, 4]

    // 双输入源; 输出指向输入; 形同 {a} += {b};
    transform(a.begin(), a.end(), b.begin()/* 第二输入 */,
            a.begin() /* 输出 */, [](int x, int y) { return x+y; });
    cout << a << endl; // [12, 23, 34]

    // 单输入源; 输出到动态增长的容器
    transform(a.begin(), a.end(), back_inserter(c),
            [](int n) { return n+2; });
    cout << c << endl; // [14, 25, 36]
}

void usage() {
    string s;
    bool retval;
    vector<int> x, y;

    {
        // all_of / any_of / none_of ------------------------------------
        retval = all_of(s.begin(), s.end(), [](auto c) { return isalnum(c); });
        retval = any_of(s.begin(), s.end(), [](auto c) { return isalnum(c); });
        retval = none_of(s.begin(), s.end(), [](auto c) { return isalnum(c); });
        //---------------------------------------------------------------
    }

    for_each(s.begin(), s.end(), [](auto &c){ c = tolower(c); }); // tolower

    // 00204. Count Primes.cpp 用到 ---------------------------------
    count(s.begin(), s.end(), ' ' /* 空格字符的个数 */);
    count_if(s.begin(), s.end(), [](auto c) { return islower(c); });
    //---------------------------------------------------------------


    {
        // mismatch: 双 container 输入; 双 iterator 输出
        // 找到首个不同的节点
        auto mpair = mismatch(x.begin(), x.end(), y.begin(), y.end());
        cout << "first mismatch of x: " << *(mpair.first) << endl;
        cout << "first mismatch of x: " << *(mpair.second) << endl;
        cout << "same head length is: " << distance(x.begin(), mpair.first) << endl;
    }
    {
        // 找到首个相同的节点
        auto mpair = mismatch(x.begin(), x.end(),
                y.begin(), y.end(),
                [](int x, int y) { return x != y; });
    }

    {
        // find 单container输入; 单 iterator 输出
        auto answer = find(x.begin(), x.end(), 1000);
    } // 首个 1000
    {
        // find_if 是函数版本
        auto answer = find_if(x.begin(), x.end(),
                [](int x) { return x > 100; } );

    } // 首个 大于 100

    {
        // find_first_of 是找子序列的; return 首个出现位置的开始 iterator
        // find_end 也是找子序列的; return 最后一个出现位置的开始 iterator
        // [ 2,1,0,1,2,1,0,1 ]  <- [1,0,1]
        //     ^       ^
        //     |       |
        //     |       *------- find_end      return
        //     |
        //     *--------------- find_first_of return
        //
        // 双container输入
        string in, pattern;
        auto head_of_in = find_first_of(in.begin(), in.end(),
                pattern.begin(), pattern.end());
        auto tail_of_in = find_end(in.begin(), in.end(),
                pattern.begin(), pattern.end());
    }

    {
        // adjacent_find 找到第一对相邻的 节点; 返回 左边
        vector<int> v;
        auto p1 = std::adjacent_find(v.begin(), v.end(), greater<int>());
        auto p2 = std::adjacent_find(v.begin(), v.end(), equal_to<int>());
    }


}

int main( int argc, char **argv ) {
    usage_transform();
    return 0;
}
