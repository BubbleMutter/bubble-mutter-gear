#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <numeric>
#include <prettyprint.hpp>
using namespace std;

void usage() {
    vector<int> x(5);
    iota(x.begin(), x.end(), 1); // [2,4,6,8,10]
    transform(x.begin(), x.end(), x.begin(), [](int x) { return x*2; });
    {

        random_shuffle(x.begin(), x.end());
        // 默认: 大顶堆
        if (!is_heap(x.begin(), x.end()))
            make_heap(x.begin(), x.end()); // [10, 8, 6, 2, 4]

        // 先如列一个元素; 然后push_heap把最后一个元素重新调整为heap
        x.push_back(10);
        push_heap(x.begin(), x.end());

        // push_pop 先把对顶元素 move 到最后并调整heap
        // 然后最后一个元素出列; 剩下的heap 依旧是heap
        pop_heap(x.begin(), x.end());
        x.pop_back();

        // make_heap 默认把大顶堆排序; 排序结果是 递增
        sort_heap(x.begin(), x.end());
        cout << x << endl;
    }
    {
        random_shuffle(x.begin(), x.end());
        // 小顶堆
        if (!is_heap(x.begin(), x.end(), greater<int>()))
            make_heap(x.begin(), x.end(), greater<int>());
        // [2, 4, 6, 8, 10]

        // 小顶堆排序要使用 greater; 排序结果是 递减
        sort_heap(x.begin(), x.end(), greater<int>());
        cout << x << endl;
    }
}

int main( int argc, char **argv ) {
    usage();
    return 0;
}
