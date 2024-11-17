#include<vector>
#include<string>
using namespace std;

class Student {
private:
    vector<int> flag;
    string name;
public:
    Student() { }
    string set(string name) {
        string old = this->name;
        this->name = string(name);
        return old;
    }
    vector<int> set(vector<int> flag) {
        vector<int> old = this->flag;
        this->flag = vector<int>(flag);
        return old;
    }
};

void usage() {
    Student student;
    int x[] = {1, 2, 3};
    int y[] = {3, 2, 1};
    student.set("Lisa");
    student.set(vector<int>(x, x+3));

    string oldname = student.set("John");
    vector<int> oldflag = student.set(vector<int>(y, y+3));
}

#if 0
重载函数: 名字相同 但是 参数不同的函数
参数不同指: 
    1. 参数个数不同, 
    2. 相同参数个数但类型顺序不同
不属于类成员函数的函数可以重载
类的成员函数可以重载
#endif