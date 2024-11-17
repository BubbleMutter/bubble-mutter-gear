/**
 * @file virtual_polymorphism.cpp 
 * @note 当类之间存在层次结构，
 *       在继承时重写(overwrite)了基类的函数，就产生多态
 *       派生类对象  通过基类指针调用成员函数时
 *       会根据派生类对象的类型来执行相应的函数
 */
#include <iostream>
using namespace std;

/* 实函数: 类继承时, 同名/同参数 成员函数将会覆盖其函数地址 */
class AnimalX {
public:
    void noise() { }
};

/* 虚函数: 类继承时, 函数地址不再静态链接; 而是动态链接 */
class AnimalY {
public:
    virtual void noise() { cout << "??" << endl; }
};

/* 纯虚函数: 使得类不可以拥有实体 
 * 拥有纯虚函数的类 即 纯虚类
 * 纯虚类有被称为 "接口类(形同 java 的 interface)"
 */
class AnimalZ {
public:
    virtual void noise() = 0;
};

class CatX: public AnimalX {
public:
    void noise() { cout << "mi" << endl; } /* 重写 overwrite */
};

class DogX: public AnimalX {
public:
    void noise() { cout << "wo" << endl; } /* 重写 overwrite */
};

class CatY: public AnimalY { };

class DogY: public AnimalY { }; 

class CatZ: public AnimalZ {
public:
    void noise() { cout << "mi" << endl; } /* 重写 overwrite */
};

class DogZ: public AnimalZ {
public:
    void noise() { cout << "wo" << endl; } /* 重写 overwrite */
};

void usage() {
    AnimalX animalx; AnimalY animaly;
    // AnimalZ animalz; // compile error: 纯虚类, 无法实例化
    AnimalZ *animal;
    CatZ catz; CatY caty;
    DogZ dogz; DogY dogy;

    animaly.noise(); // 普通虚拟函数; 可以正常调用
    caty.noise(); // 派生类没有覆盖 基类的虚拟函数; 仍然是基类的虚函数
    dogy.noise(); // 派生类没有覆盖 基类的虚拟函数; 仍然是基类的虚函数

    animal = dynamic_cast<AnimalZ *>(&catz);
    animal->noise(); /* 多态 polymorphism 使用例子 */
    animal = dynamic_cast<AnimalZ *>(&dogz);
    animal->noise(); /* 多态 polymorphism 使用例子 */
}
int main(void) {
    usage();
    return 0;
}