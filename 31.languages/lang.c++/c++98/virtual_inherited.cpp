#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

class A {
    public:
        int x;
        A() { x = 0; }
};

#if 0 // 普通继承; 在 C 中 A 有两个实例; 成员变量 x 冲突

/* error: reference to‘x’is ambiguous */
#if 1 // 两个都是普通继承
class B1 : public A { };
class B2 : public A { };
#else // 只要有一个普通继承 也是不行的
class B1 : public A { };
class B2 : virtual public A { };
#endif

#else // 虚拟继承; 在 C 中 A 有且只有1个实例; 成员变量 x 不冲突
class B1 : virtual public A { };
class B2 : virtual public A { };
#endif

class C : public B1, public B2 {
    public:
        int getC() { return x; }
};

void usage() {
    cout << sizeof(A) << endl;  // 4
    cout << sizeof(B1) << endl; //
    cout << sizeof(B2) << endl; //
    cout << sizeof(C) << endl;  //
}

int main(int argc, char *argv[]) {
    usage();
    return 0;
}