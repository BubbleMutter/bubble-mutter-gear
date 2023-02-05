而静态成员函数没有this指针
stl 的 iterator 本质上是什么东西?
iterator  比起指针高级在什么地方
如果避免迭代器删除的失效问题


左值（lvalue）：指向内存位置的表达式被称为左值（lvalue）表达式。左值可以出现在赋值号的左边或右边。
右值（rvalue）：术语右值（rvalue）指的是存储在内存中某些地址的数值。右值是不能对其进行赋值的表达式，也就是说，右值可以出现在赋值号的右边，但不能出现在赋值号的左边。


面向对象的4个特性: 封装 抽象 继承 多态
区别:  类 对象 指针 引用
变量类型7: bool void short int long float double
考虑与语言特性时: 注意区分  编译时 和 运行时 的行为


C++中, 虚函数可以为private, 并且可以被子类覆盖（因为虚函数表的传递），但子类不能调用父类的private虚函数。虚函数的重载性和它声明的权限无关。

一个成员函数被定义为private属性，标志着其只能被当前类的其他成员函数(或友元函数)所访问。而virtual修饰符则强调父类的成员函数可以在子类中被重写，因为重写之时并没有与父类发生任何的调用关系，故而重写是被允许的。

编译器不检查虚函数的各类属性。被virtual修饰的成员函数，不论他们是private、protect或是public的，都会被统一的放置到虚函数表中。对父类进行派生时，子类会继承到拥有相同偏移地址的虚函数表（相同偏移地址指，各虚函数相对于VPTR指针的偏移），则子类就会被允许对这些虚函数进行重载。且重载时可以给重载函数定义新的属性，例如public，其只标志着该重载函数在该子类中的访问属性为public，和父类的private属性没有任何关系！
纯虚函数可以设计成私有的，不过这样不允许在本类之外的非友元函数中直接调用它，子类中只有覆盖这种纯虚函数的义务，却没有调用它的权利。

# plan list
## level 1 (优先级低)
1. lambda
2. share_ptr, auto_ptr
3. iterator 底层; 我们怎么实现自己类的 iterator
4. algorithm 总结
5. design pathern
6. virtual inheriated
7. overload operator

## level 2 (对象模型)

# library
## stl pretty print
1. https://github.com/louisdx/cxx-prettyprint
    + header only, include and using "<<"
    + It implements all container
    + example `cout << mvecotr << endl;`

## stdc++v3
1. https://github.com/gcc-mirror/gcc/ 源码中的 libstdc++-v3
2. https://code.woboq.org/gcc/libstdc++-v3/ 在线阅读