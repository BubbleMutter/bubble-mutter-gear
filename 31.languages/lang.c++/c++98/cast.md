[TOC]
1. 上行转换(子类指针 转换 基类指针)
2. 下行转换(基类指针 转换 子类指针)
3. 对象的 class_id 只有在创建时赋值
   不存在运行时变更的场景
4. 4种 cast 不影响 compile 行为; 只影响 running 行为

# const_cast
1. 常量{对象/指针/引用} 转化为 非常量{对象/指针/引用}
```c++
void use_const_cast() {
    // 指针
    const int *a;
    int *p = const_cast<int *>a;
    // 引用
    const int &b;
    int &q = const_cast<int &>b;
    // 对象
    const C c;
    C r = const_cast<C> c;
}
```

# static_cast
1.无安全检查的 上行转换 和 下行转换; 类之间的交叉转换
    1. 上行转换 and 下行转换 均不检查
```c++
class Parent {
    public:
        virtual void f() { printf("Parent object\n"); return; }
};
class ChildX : public Parent {
    public:
        void f() { printf("ChildX object\n"); return; }
};

int main( int argc, char **argv ) {
    Parent parent;
    ChildX childX;
    Parent *parent_ptr = NULL;
    ChildX *childX_ptr = NULL;

    // 静态上级转换a) 转换成功; 无隐患
    childX_ptr = &childX;
    parent_ptr = static_cast<Parent *>(childX_ptr);

    // 静态下级转换 b) 上级指针实际上指向上级对象; 转换成功; 但有危险操作
    parent_ptr = &parent;
    childX_ptr = static_cast<ChildX *>(parent_ptr);
    printf("[%d] [%s] static_cast<ChildX *>(parent_ptr); will be dangerous\n",
            __LINE__, childX_ptr ? "success" : "failed");
    printf("[%d] unsafe read member %d\n", childX_ptr->member); // 如果是写操作; 问题很大

    return 0;
}
```

# dynamic_cast
1. 带安全检查的 层次类指针的上行转换 和 下行转换; 类之间的交叉转换
2. 检查对象运行时实际的 class_id (RTT) 与　目标类的　class_id
3. 只有对象的　class_id 同于目标类的　class_id
   或者对象的　class_id 属于目标类的　子类 才能转换成功
4. 即只允许 有效的(对象运行时实际类型) { 同级转换 和 上行转换 }
5. warn: 不是对象的指针类型, 而是指针指向对象的实际类型 class_id
```c++
class Parent {
    public:
        virtual void f() { printf("Parent object\n"); return; }
};
class ChildX : public Parent {
    public:
        void f() { printf("ChildX object\n"); return; }
};
int main( int argc, char **argv ) {
    Parent parent;
    ChildX childX;
    Parent *parent_ptr = NULL;
    ChildX *childX_ptr = NULL;

    // 动态上级转换a) 转换成功; 无隐患
    childX_ptr = &childX;
    parent_ptr = dynamic_cast<Parent *>(childX_ptr);

    // 动态下级转换 a) 上级指针实际上指向下级对象; 转换成功
    parent_ptr = &childX;
    childX_ptr = dynamic_cast<ChildX *>(parent_ptr);
    printf("[%d] [%s] dynamic_cast<ChildX *>(parent_ptr)\n",
            __LINE__, childX_ptr ? "success" : "failed");

    // 动态下级转换 b) 上级指针实际上指向上级对象; 转换失败
    parent_ptr = &parent;
    childX_ptr = dynamic_cast<ChildX *>(parent_ptr);
    printf("[%d] [%s] dynamic_cast<ChildX *>(parent_ptr)\n",
            __LINE__, childX_ptr ? "success" : "failed");
    return 0;
}
```

# reinterprut_cast 实际上 几乎同于 c语言的 指针强转
```c++
int n = 0x6162636400; // 0x61是字符'a'的ASCII码
int  *pint = &num;
char *pstr = reinterpret_cast<char *>(&num);
char *pstr_c = (char *)&n;
```