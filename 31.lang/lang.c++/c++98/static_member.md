[TOC]
# 1. 所有类成员函数有且只有1个实体

# 2. 非静态类成员变量 每个对象 都有 1个副本

# 3. 静态成员变量 有且只有 1个实体
1. 即所有对象的this指针指向该变量时; 都是同一个变量
2. static-data-member 在 elf 中放在 `.data` section 

# 4. 静态成员函数与普通成员函数的区别
1. 静态成员函数没有 this 指针，只能访问静态成员(包括变量和函数)
2. 普通成员函数有 this 指针，可以访问类中的任意成员(静态和非静态)

# 代码示例
```c++
#include <iostream>
using namespace std;

class C {
    static int member;
public:
    C() { member++; }
    ~C() { member--; }
    int getMemberFromObject1() {
        return this->member; // valid: static member can be accessed by this
    }
    int getMemberFromObject2() {
        return member; // valid: compiler will replace code with `return this->member`
    }
    static int getMemberFromClass() {
        // return this->member; // error: static member function can't access `this'
        return member; // valid: compiler will replace code with `return C::member'
    }
};

int C::member = 0;

int main( int argc, char **argv ) {
    C c;
    cout << c.getMemberFromObject1() << endl;
    cout << c.getMemberFromObject2() << endl;
    cout << C::getMemberFromClass() << endl;
    return 0;
}
```