#include <vector>
#include <numeric>
#include <functional>
#include <iostream>
#include <prettyprint.hpp>
using namespace std;

void usage() {
    {
        // 累加器
        vector<int> x(10);
        iota(x.begin(), x.end(), 0); // like python range(10)
        int sum = accumulate(x.begin(), x.end(), 0);
        int product = accumulate(x.begin(), x.end(), 1, [](int x, int y) { return x*y; });
        // 00136. Single Number.cpp
        int single = accumulate(x.begin(), x.end(), 0, bit_xor<int>());
    }
    {
        // 求叉积
        vector<int> x(5), y(5);
        iota(x.begin(), x.end(), 0);
        iota(y.begin(), y.end(), 0);
        // = x[0]*y[0] + x[1]*y[1] + ... + x[n]*y[n]
        int answer = inner_product(x.begin(), x.end(), y.begin(), 0);
    }
    {
        // 求导数
        /*
         * *(d_first)   = *first;
         * *(d_first+1) = *(first+1) - *(first);
         * *(d_first+2) = *(first+2) - *(first+1);
         * *(d_first+3) = *(first+3) - *(first+2);
         */
        vector<int> x(5), y;
        iota(x.begin(), x.end(), 0);
        transform(x.begin(), x.end(), x.begin(), [](int x) { return x*x; });
        adjacent_difference(x.begin(), x.end(), back_inserter(y));
        // x == [ 0, 1, 4, 9, 16 ]
        // y == [ 0, 1, 3, 5,  7 ]
    }
    {
        // 求前缀和数组
        /*
         * *(d_first)   = *first;
         * *(d_first+1) = *first + *(first+1);
         * *(d_first+2) = *first + *(first+1) + *(first+2);
         * *(d_first+3) = *first + *(first+1) + *(first+2) + *(first+3);
         */
        vector<int> x(5), y;
        iota(x.begin(), x.end(), 0);
        partial_sum(x.begin(), x.end(), back_inserter(y));
        // x == [ 0, 1, 2, 3, 4 ]
        // y == [ 0, 1, 3, 6, 10 ]
    }
}

int main( int argc, char **argv ) {
    usage();
    return 0;
}
