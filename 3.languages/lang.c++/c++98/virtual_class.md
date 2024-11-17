[TOC]
# 纯虚类无法被实例化; 不能作为 stl 容器的模板类
```c++
#include <vector>
#include <list>
#include <stdio.h>
using namespace std;

class X { };
class Y { public: virtual ~Y() = 0; };

template<class T>
class Z {
public:
    Z() { printf("sizeof(template) == %zd\n", sizeof(T)); }
};

int main(void) {
        vector<X> vx(1);

        // vector<Y> vy(1); // compile error: stl limitation

        Z<X> z2;
        Z<Y> z3; // comile valid: template can be pure virtual class
        // Y singleY; // compile error: pure virtual object can't be created
        return 0;
}
```