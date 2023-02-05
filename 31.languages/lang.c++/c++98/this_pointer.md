[TOC]
+ this 指针的两个作用
# 示例代码
```c++
class Cls {
private:
    int member;
public:
    Cls() {}
    int getMember() { return member; }
}
void usage() {
    Cls c;
    cout << c.getMember() << endl;
}
```
# 作用1) 调用对象非静态成员函数时; 隐式传递 对象指针为 this
即 `c.getMember();` 编译时转换为 `Cls::getMember(&c); this = &c;`

# 作用2) 在对象非静态成员函数内部; 隐式使用 this 指向成员变量
即 `return member;` 转换为 `return this->member;`
