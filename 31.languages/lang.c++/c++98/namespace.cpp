#include <iostream>
using namespace std;

namespace O {
    int x = 1;
    namespace P {
        int x = 2;
    }
}

int x = 3;

void usage() {
    /* 命名空间的嵌套 以及 其作用域
     */
    cout << O::x << endl; // 1
    cout << O::P::x << endl; // 2
    cout << x << endl; // 3
}