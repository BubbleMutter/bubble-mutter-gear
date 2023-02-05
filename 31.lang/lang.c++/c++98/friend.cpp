/* friendly 标记只影响 compile 行为
 * 不影响 running 行为
 */
#include<iostream>
using namespace std;
class X;

class Y {
private:
    int numPrivate;
public:
    Y(const X &x);
protected:
    int numProtected;
};

class X {
private:
    int numPrivate;
public:
    X(int _numPrivate, int _numProtected);
    friend int getNumPrivate(const X &x);   // 友元函数: 一个不属于任何类的函数
    friend int getNumProtected(const X &x); // 友元函数: 一个不属于任何类的函数
    friend class Y; /* 友元类 */
protected:
    int numProtected;
};

X::X(int _numPrivate, int _numProtected):
    numPrivate(_numPrivate), numProtected(_numProtected) { }

/* 友元函数允许访问 private / protected 变量 */
int getNumPrivate(const X &x) { return x.numPrivate; }
int getNumProtected(const X &x) { return x.numProtected; }

/* 友元类的所有成员函数 允许访问 privte / protected 变量 */
Y::Y(const X &x) {
    this->numPrivate = x.numPrivate;
    this->numProtected = x.numProtected;
}

void usage() {
    X x(1, 2);
    cout << getNumPrivate(x)   << endl;
    cout << getNumProtected(x) << endl;
    Y y(x);
}