+ 如果一个模板从来没有使用过 那么它将会被编译器忽略 不会生成可执行码
+ 一个没有被使用的模板可能会包含一些编程错误 包含这个模板的程序仍然可以编译和运行
+ 当模板用于生成代码时 即当被编译时 模板中的错误才会被编译器标识出来
+ 第八章 随机与分布 没有看; 需要数学理论作为支撑; 长期无法触及; 先放弃

```c++
// 模板类示例
template<typename T>
class Array {
private:
    T* element;                        // Array of Type T
    size_t count;
public:
    explicit Array(size_t size);              // Constructor
    Array(const Array & other);               // Copy Constructor
    Array(Array && other);                    // Move Constructor
    Array& operator=(const Array& rhs);       // Assignment operator
    Array& operator=(const Array&& rhs);      // Move assignment operator

    virtual ~Array();                         // Destructor
    T& operator[] (size_t index);             // Subscript operator
    const T& operator[] (size_t index) const; // Subscript operator-const
    size_t size() { return count; }           // Accessor for count
};
```
覆盖一个 class 的5个构造函数

```c++
template<typename T> using ptr = std::shared_ptr<T>; // 模板类的别名
ptr<std::string> m_ptr; // 同于 std::shared_ptr<std::string>
```

# 迭代器介绍
有 5种不同类型的迭代器
所有迭代器都要求有 一个拷贝构造函数, 一个析构函数 一个拷贝赋值运算符
所有迭代器指向的对象必须是可交换的 (swappable) // 2021/08/14 ?这是为什么呢?
不同迭代器有不同的功能  不同的算法函数要求不同功能的迭代器
每种类别由一个 "迭代器标签类" 的空类 作为迭代器的模板参数指定
"迭代器标签类" 又称为 iterator tag class

迭代器类型及其继承关系
输入迭代器 = input_iterator_tag
输出迭代器 = output_iterator_tag
正向迭代器 = forward_iterator_tag 派生于 input_iterator_tag
双向迭代器 = bidirectional_iterator_tag 派生于 forward_iterator_tag
随机访问迭代器 = random_access_iterator_tag 派生于 bidirectional_iterator_tag

流迭代器
istream_iterator = 输入流迭代器
ostream_iterator = 输出流迭代器

迭代器适配器是一个类模板; 为标准迭代器提供了一些特殊的行为; 从标准迭代器派生出来
迭代器适配器分为3种
反向迭代器 = 容器方法 `rbegin()` / `rend()` / `crbegin(()` / `crend()`
插入迭代器 = 分为3种 `back_insert_iterator` / `front_insert_iterator` / `insert_iterator`
移动迭代器 = `make_move_iterator`

迭代器的运算
`advance`  使一个迭代器后移 n 个位置
`distance` 计算两个迭代器的 距离
`next`     返回一个迭代器的后一个位置
`prev`     返回一个迭代器的前一个位置


# 智能指针模板
1. 智能指针只能用来保存"堆上"的内存地址
2. 智能指针不能进行"自增"和"自减"的运算

## 三种不同类型的智能指针模板
1. `unique_ptr<T>`: 互斥指针 只能通过 `std::move` 来移动
2. `shared_ptr<T>`: 共享指针 分别有引用计数
3. `weak_ptr<T>`  : 共享指针 无引用计数版本

## unique_ptr 用法
```c++
// 使用 make_unique<T> 提供参数 的创建方式
auto pname = std::make_unique<std::string>("Hello World!");

// 指向 数组的 unique_ptr
auto parray = std::make_unique<int []>(10);

// 获 unique_ptr 中的原生指针; get方法
auto unique_pstr = std::make_unique<std::string>("Hello World!");
std::string pstr(*unique_pstr.get());

// 析构 unique_ptr 指向的原生对象; 然后指向新的一个对象
pname.reset(new std::string("Hello World!")); // to another

// 析构 unique_ptr 指向的原生对象; 并置空 unique_ptr 本身
pname.reset(); // release memory for string object
if (!pname)
    std::cout << "empty" << std::endl;

// 释放 unique_ptr 对象本身; 并返回原生对象
auto unique_name = std::make_unique<std::string>("Hello World!");
std::unique_ptr<std::string> unique_name2(unique_name.release());

// 交换两个 unique_ptr 指向的对象
auto pn1 = std::make_unique<std::string>("Jane");
auto pn2 = std::make_unique<std::string>("John");
pn1.swap(pn2);
```

## shared_ptr 用法
```c++
auto pdata = std::make_shared<double>(10.01); // Points to a double variable

// 检查两个 shared_ptr 是不是指向同一个对象
if (pA && (pA == pB))
    std::cout << "Both pointers point to the same object.\n" << std::endl;

if (pdata.unique()) // 检查一个 shared_ptr 是不是还有副本
    std::cout << "there is only one..." << std::endl;
else // 检查一个 shared_ptr 的实例个数
    std::cout << "there are " << pdata.use_count() << " instances." << std::endl;

```

# 函数指针的升级版
## 1. 函数对象 又被称为 作坊函数
```c++
class Volume {
public:
  double operator()(double x, double y, double z) { return x*y*z; };
  double operator()(const Box& box) {
    return box.getLength() * box.getWidth() * box.getHeight();
  }
};

Volume volume;
double room = volume(4, 5, 6);
```

## 2. lambda 表达式
1. lambda 表达式以值引用的方式
   每捕获一个变量 就会增加一定的开销
   都会为它们创建一个副本
   建议 值引用的方式只捕获有需要的变量
   禁止 值引用的方式捕获所有的变量
   `[&,val1,val2] (...) { };` 比如这样子

## 3. 传参
```c++
class Root {
public:
  double operator()(double x) { return std::sqrt(x); };
};

void usage() {
    double data[] { 1.5, 2.5, 3.5, 4.5, 5.5 };

    // passing a function object
    Root root; // function object
    std::transform(std::begin(data), std::end(data),
                   std::ostream_iterator<double>(std::cout, " "), root);

    // using an lambda expression as an argument
    std::cout << "\n\nCubes are:" << std::endl;
    std::transform(std::begin(data), std::end(data)),
                   std::ostream_iterator<double>(std::cout, " "),
                   [] (double x) { return x*x*x; });

    // using a variable of type std::function<> as argument
    std::function<double(double)> op { [](double x) { return x*x; } };
    std::cout << "\n\nSquares are:" << std::endl;
    std::transform(std::begin(data), std::end(data),
                   std::ostream_iterator<double>(std::cout, " "), op);

    // using a lambda expression that calls another
    // lambda expression as argument
    std::cout << "\n\n4th powers are:" << std::endl;
    std::transform(std::begin(data), std::end(data),
                   std::ostream_iterator<double>(std::cout, " "),
                   [&op] (double x) { return op(x)*op(x); });
}
```

# 容器特性
1. array = 固定大小; 随机访问
2. vector = 变长 array; 随机访问;  快速增删末尾元素; 增删首部中间元素相当耗时
3. deque = 变长 array; 随机访问; 快速增删首尾元素; 增删中间元素相当耗时
4. list  = 双向变长链表 = 不能随机访问; 快速增删任意元素
5. forward_list = 单向变长链表 = 不能随机访问; 快速增删任意元素
6. stack = 用 deque 实现的适配类模板; 提供 后入先出的栈功能
7. queue = 用 deque 实现的适配类模板; 提供 先入先出的队列功能
8. priority_queue = 用 vector 实现的堆; 大定堆或者小顶堆


priority_queue 其底层创建了一个堆
priority_queue 可以提供堆接口本身缺乏的功能 a) 增删元素后自动维持堆结构 b) 只能访问第一个元素
make_heap 创建的堆可以 a) 随机访问; b) 偶尔破坏堆结构; c) 重新调用 make_heap 还原
vector::reverse 方法提前指定足够的空间, 比起动态增长过程中频繁申请内存移动数据 性能有一定提升

# 排序
sort 算法是不稳定排序(快排), 会改变相等元素的顺序,
比如说排序对象的成员 id 作为排序方法的基准, 但是排序对象附带着其他是事务,
而原始容器中 相同id 的事务是符合现实时间先后顺序的,
如果经有排序被改变后, 将会导致程序逻辑错误.
如 { {id1, {event1}}, {id2, {event1}}, {id1, {event2}}, {id2, {event2}} }
我排序算法针对 id 进行排序, 但是又想保留 event 出现的顺序, 可以使用 `stable_sort`