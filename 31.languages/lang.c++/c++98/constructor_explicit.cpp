#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

class XPool {
    private:
        void *data;
        int size;
        int item;
        vector<bool> used;
    public:
        XPool(int item, int size = 16): item(item), size(size) {
            data = malloc(size*item);
        }
        void *get() { return nullptr; }
        void put(void *data) { }
        void *dump() { return data; }
        ~XPool() { free(data); }
};

class YPool {
    private:
        void *data;
        int size;
        int item;
        vector<bool> used;
    public:
        explicit YPool(int item, int size = 16): item(item), size(size) {
            data = malloc(size*item);
        }
        void *get() { return nullptr; }
        void put(void *data) { }
        void *dump() { return data; }
        ~YPool() { free(data); }
};

class ZPool {
    private:
        void *data;
        int size;
        int item;
        vector<bool> used;
    public:
        ZPool(int item, int size): item(item), size(size) {
            data = malloc(size*item);
        }
        void *get() { return nullptr; }
        void put(void *data) { }
        void *dump() { return data; }
        ~ZPool() { free(data); }
};


void usage() {
    XPool xpool1(sizeof(int), 64);
    XPool xpool2 = sizeof(long); // compile valid: implicit constructor with one non-auto-parameter
    XPool xpool3 = xpool1; // compile valid: implicit constructor, copy non-static-member-variable directly
                           // running error: 因为 data 指向同一块内存; 两个 内存池使用时 行为会冲突; 释放时 double-free
                           // 实际上 Pool 这个类不允许这样子使用
    if ( xpool1.dump() == xpool3.dump())
        cout << "same data" << endl;

    YPool ypool1(sizeof(int), 64);
    // YPool ypool2 = sizeof(long); // compile error: `explicit` forbit implicit coinstructor
    // YPool ypool3 = ypool1;       // compile error: `explicit` forbit implicit coinstructor

    ZPool zpool1(sizeof(int), 64);
    // ZPool zpool2 = sizeof(long); // compile error: missing constructor
}

#if 0
explicit 禁止了两种编译时的隐式构造行为: (在没有实现 operator= 时的 =初始化)
1. 只有一个非auto参数的构造函数 的 隐式构造
2. 直接复制对象的所有非静态变量

另外:
3. 当不存在 "只有一个非auto参数的构造函数时",
   =初始化 赋值一个非同类对象, compile error
#endif