# Q: 面向对象的特性
1. 封装:  把 "一系列数据" 及其 "相关的函数" 封装在一起, 构成一个一个的类 (解耦合)
    1. 数据称为属性 attribute, 函数又称为方法 method
    2. 程序中其他对象只能通过 暴露出来的方法 与该对象进行交互
2. 抽象: 仅仅暴露关键信息, 隐藏内部实现细节 (屏蔽细节)
    1. 使用 private 访问标签对外隐藏底层细节
    2. 使用 public  访问标签对外暴露感兴趣的方法
3. 继承: 基类的基础上创建派生类 (代码复用)
    1. 派生类可以 新增属性 和 方法
    2. 派生类可以 有条件地(权限)访问 基类的属性 和 方法
4. 多态: 同名方法有不同的实现   (屏蔽细节/代码复用)
    1. 分为 编译时多态 和 运行时多态
    2. 即 重载overload 和 重写overwrite
    3. 重载即同名函数但参数,返回值不同
    4. 重写即派生类重写基类方法
       使得通过基类句柄调用其方法的上下文 实际上调用派生类的方法
       从而在该上下文中不需要关系派生类实现的细节

# Q: 引用 和 指针的区别? 引用的优点在哪里?

# Q: 什么时候一个类需要提供一个 拷贝构造函数; 什么时候不需要?

# Q: 构造函数可不可以是纯虚函数? (不行)
因为虚表指针的赋值代码; 由编译器自动插入到构造函数中.

# Q: 析构函数可不可以是纯虚函数? (可以)

# set<pair<vector<int>, vector<int>>> 与 set<vector<int>> 的性能差区别
1. 如果 `set<pair<vector<int>, vector<int>>>` 两个入列vector.size 都是固定的
2. 而且 `set<vector<int>>` 中的 vector 尺度等同于上步骤两个vector.size和
3. 经验发现, 在随机 数值的增删改查测试下, 后者的效率明显高于前者

# set<vector<int>> 与 set<array<int, n>> 的性能差区别
1. 当 vector 和 array 的数组成员个数已知且相同
2. 经验发现, 在随机 数值的增删改查测试下, 后者的效率明显高于前者

# Q: vector<T>, T 满足什么条件
1. 不可以是纯虚类, 因为没法实例化

# Q: map<K, V>, K 满足什么条件 (同理 set, multiset, multimap)
1. 实现了 `operator < (const T&) const { };` 方法的类
2. 因为 map 的模板参数, 需要比较函数, (include/bits/stl_map.h); </br>
   默认的比较函数是模板函数 `std::less`  (include/bits/stl_funct ion.h) </br>
   而 less 要求模板类实现了 `operator<` 方法
而 less 要求模板类实现了 `operator<` 方法
3. 关键源码
```c++
template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
        typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
class map
{
    /* ... */
};

template<typename _Tp>
struct less : public binary_function<_Tp, _Tp, bool>
{
    _GLIBCXX14_CONSTEXPR
    bool
    operator()(const _Tp& __x, const _Tp& __y) const
    { return __x < __y; }
};
```
4. 另外, 如果 map 定义时, 模板参数使用 std::greater,
   那么就要求实现类实现了 `operator>(const T &) const { }` 方法

# Q: multimap 和 map 有什么区别 (同理 multiset 和 set 等)
1. 插入方法不同:
   map      使用了红黑树的插入函数 _M_insert_unique
   multimap 使用了红黑树的插入函数 _M_insert_equal
```c++
class map {
    std::pair<iterator, bool> insert(value_type&& __x)
    { return _M_t._M_insert_unique(std::move(__x)); }
}
class multimap {
    std::pair<iterator, bool> insert(value_type&& __x)
    { return _M_t._M_insert_equal(std::move(__x)); }
}
```

# Q: unordered_map 和 map 有什么区别
1. 底层结构不同
   map 使用红黑树
   unordered_map 使用哈希表
2. 比较方法不同
   map 使用了 `std::less` (默认的), 也可以使用 `std::greater` 作为 红黑树的比较方法
   unordered_map 使用了 `std::equal_to` (默认的) 作为 hash 表的比较方法
3. 关键源码
```c++
template <typename _Key, typename _Tp, typename _Compare /* 模板参数 比较方法 */ = std::less<_Key>,
    typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
class map
{
    typedef _Compare key_compare; /* 模板参数 比较方法; 重命名了一下? */
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>,
            key_compare /* 模板参数 比较方法 */, _Pair_alloc_type> _Rep_type;
    /// The actual tree structure.
    _Rep_type _M_t;
}

template<typename _Key, typename _Tp,
    typename _Hash = hash<_Key>,
    typename _Pred = equal_to<_Key>, /* 模板参数 比较方法 */
    typename _Alloc = allocator<std::pair<const _Key, _Tp>>>
class unordered_map
{
    typedef __umap_hashtable<_Key, _Tp, _Hash, _Pred  /* 模板参数 比较方法 */, _Alloc>  _Hashtable;
    _Hashtable _M_h;
}
```

# Q: list 的底层实现
1. 带有头节点的双向链表
