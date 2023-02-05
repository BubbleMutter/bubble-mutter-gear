/**
 * @file private_public_protected.cpp
 *       private / public / protected 3种继承方法
 *       都不会影响派生类 内部访问原则:
 *         只允许访问基类的 public / protected; 不允许访问基类的private
 *       但会转换基类成员变量在派生类中的 访问修饰符
 *       进而影响 a) 2级派生的访问 和 b) 派生类对象的外部访问
 *
 *       访问修饰符的作用时机是 compile 阶段
 */
#include<iostream>
using namespace std;

class Parent {
private:
    int privateValue;
public:
    int publicValue;
    void accessFromParent() {
        // 基类 内部可以访问自己的 private public protected
        cout << this->privateValue   << endl;
        cout << this->publicValue    << endl;
        cout << this->protectedValue << endl;
    }
protected:
    int protectedValue;
};

class ChildPrivate: private Parent {
public:
    void accessFromChild() {
        // 任意继承;  都允许访问基类的 public / protected; 但不能访问基类的 private
        // 原理: private 继承 使得基类的public/protected 在派生类中变成了 private

        // cout << this->privateValue   << endl; // compile error
        cout << this->publicValue    << endl; // compile valid
        cout << this->protectedValue << endl; // compile valid
    }
};

class ChildPublic: public Parent {
public:
    void accessFromChild() {
        // 任意继承;  都允许访问基类的 public / protected; 但不能访问基类的 private
        // 原理: public 继承不改变基类的访问修饰符
        // cout << this->privateValue   << endl; // compile error
        cout << this->publicValue    << endl; // compile valid
        cout << this->protectedValue << endl; // compile valid
    }
};

class ChildProtected: protected Parent {
public:
    void accessFromChild() {
        // 任意继承;  都允许访问基类的 public / protected; 但不能访问基类的 private
        // 实际上 protected 继承; 基类的public/protected在派生类中变成了 protected
        // cout << this->privateValue   << endl; // compile error
        cout << this->publicValue    << endl; // compile valid
        cout << this->protectedValue << endl; // compile valid
    }
};

class GrandChildPublic: public ChildPublic {
public:
    void accessFromGrand() {
        // Child 类 public 继承不改变基类的 访问修饰符
        // cout << this->privateValue   << endl; // compile error
        cout << this->publicValue    << endl; // compile valid
        cout << this->protectedValue << endl; // compile valid

    }
};

class GrandChildPrivate: public ChildPrivate {
public:
    void accessFromGrand() {
        // Child 类 private 继承使得基类 public/protected 都变成private
        // cout << this->privateValue   << endl; // compile error
        // cout << this->publicValue    << endl; // compile error 
        // cout << this->protectedValue << endl; // compile error
    }
};

class GrandChildProtected : public ChildProtected {
public:
    void accessFromGrand() {
        // Child 类 protected 继承使得基类 public/protected 都变成 protected
        // cout << this->privateValue   << endl; // compile error
        cout << this->publicValue    << endl; // compile valid
        cout << this->protectedValue << endl; // compile valid
    }
};

int usage() {
    Parent mParent;
    ChildPrivate mChildPrivate;
    ChildPublic mChildPublic;
    ChildProtected mChildProtected;

    /* 基类 只有 public 变量才能被访问
     */
    // cout << mParent.privateValue << endl; // compile error
    cout << mParent.publicValue << endl;
    // cout << mParent.protectedValue << endl; // compile error

    /* private 继承的所有基类变量 都无法外部访问
     * 实际上 private 继承的基类变量都是 private
     */
    // cout << mChildPrivate.privateValue << endl; // compile error
    // cout << mChildPrivate.publicValue << endl;  // compile error
    // cout << mChildPrivate.protectedValue << endl; // compile error

    /* public 继承的基类 只有 public变量允许外部访问
     *
     */
    // cout << mChildPublic.privateValue << endl; // compile error
    cout << mChildPublic.publicValue << endl;     // compile valid
    // cout << mChildPublic.protectedValue << endl;  // compile error

    /* protected 继承的基类 都无法外部访问
     * 实际上 protected 继承的基类变量都是 protected
     */
    // cout << mChildProtected.privateValue << endl; // compile error
    // cout << mChildProtected.publicValue << endl;  // compile error
    // cout << mChildProtected.protectedValue << endl; // compile error
    return 0;
}