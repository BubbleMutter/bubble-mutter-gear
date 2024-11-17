/**
 * @file try_catch.cpp 
 * @note 所有的 exception 类都有 what() 方法; 返回 string
 */
#include <iostream>
#include <typeinfo>
#include <exception>
using namespace std;

class Polymorphic { virtual void member() { } };

class custom_exception : public exception {
public:
    /* 4. 自定义 exception 类
     * overwrite throw 方法即可
     */
    const char * what () const throw () /* 3. 指示向上传递 exception 的函数 */
    {
        return "custom_exception";
    }
};

void usage(void) {
    // 1. std exception 的使用
    try {
        Polymorphic *pb = 0;
        typeid (*pb); // std::bad_typeid exception
    } catch (exception &e) {
        cerr << "exception: " << e.what() << endl;
    }

    // 2. 自定义 exception 的使用
    try {
        throw custom_exception();
    } catch (exception &e) {
        cerr << "exception: " << e.what() << endl;
    }
}

#if 0 /* 所有的 exception 结构 */
exception:
  > bad_alloc (new 抛出)
  > bad_cast (dynamic_cast 抛出)
  > bad_typeid (typeid 抛出)
  > bad_exception
  > logic_error
    > domain_error
    > invalid_argument
    > length_error (string 过长抛出)
    > out_of_range (operator[]() 方法抛出; 如 deque/vector/bitset)
  > runtime_error
    > overflow_error  (数值上溢; leetcode-bit 题目经常写出来)
    > range_error (数值 超出存储范围时)
    > underflow_error (数值下溢: leetcode-bit 题目经常写出来)
#endif