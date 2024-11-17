#include<iostream>
using namespace std;

class Cls {
public:
    Cls() { cout << "constructor: " << __func__ << endl; }
    ~Cls() { cout << "destructor: " << __func__ << endl; }
};

void usage() {
    int n = 10;
    // 动态分配
    int *a = new int[n];

    // 释放内存
    delete [] a;

    /* delete/new 和 malloc/free 的区别: 
     * 1. new    会调用构造函数; malloc 不会
     * 2. delete 会调用析构函数; free   不会
     */
    cout << "before new" << endl;
    Cls *c = new Cls;
    cout << "after new" << endl;


    cout << "before delete" << endl;
    delete c;
    cout << "after delete" << endl;

    // new 连带初始化
    {
        int *a = new int[3]{ 1, 2, 3};
        vector<int> v(a, a+3);
        cout << v << endl; // [1, 2, 3]
        delete [] a;
    }
    {
        int *a = new int(1024);
        cout << *a << endl; // 1024
        delete a;
    }
}