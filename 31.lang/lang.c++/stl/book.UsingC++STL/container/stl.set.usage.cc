// set 的 增删改查 --------------------------------------
// 初始化列表
std::set<int> numbers { 8, 7, 6, 5, 4, 3, 2, 1 };
// 拷贝迭代器; 从 stdin 中获取
std::copy(numbers.begin(), numbers.end(),
    std::ostream_iterator<int>(std::cout, " "));
// 从另一个迭代器初始化过来
std::set<int> numbers2(numbers.begin(), numbers.end());
// 指定比较函数; 默认 less; 比如改为 greater
std::set<int, std::greater<int>> numbers3(numbers.begin(), numbers.end());

// 添加元素
numbers.insert(10);
numbers.insert({10, 20, 30, 40});
vector<int> numbers4{ 100, 200, 300 };
numbers.insert(numbers4.begin(), numbers.end());

// 删除元素
numbers.erase(numbers.begin()); // 通过迭代器指定
numbers.erase(10); // 通过成员元素指定
numbers.erase(++numbers.begin(), --numbers.end());

// 查找元素
auto iter = numbers.find(15);
if (iter != numbers.end())
    cout << "value = " << *iter << endl;

// 检查元素是否存在
if (numbers.count(100))
    cout << "It's in the set" << endl;
// --------------------------------------------------

// 集合操作; 操作容器对象不限制在 std::set; 可以是其他容器
std::vector<int> set1 { 1, 2, 3, 4, 5, 6 };
std::vector<int> set2 { 4, 5, 6, 7, 8, 9 };
std::vector<int> res_union, res_intersection,
                res_difference1, res_difference2,
                res_symmetric_difference;

// 求并集
std::set_union(set1.begin(), set1.end(),
               set2.begin(), set2.end(),
               std::back_inserter(res_union)); // 转存到结果 vector 中
// 求交集
std::set_intersection(set1.begin(), set1.end(),
                      set2.begin(), set2.end(),
                      std::back_inserter(res_intersection));
// 求差集
std::set_difference(set1.begin(), set1.end(), /* 求第一个集合 关于 第二个集合的差集 */
                    set2.begin(), set2.end(),
                    std::back_inserter(res_difference1));
std::set_difference(set2.begin(), set2.end(), /* 求第二个集合 关于 第一个集合的差集 */
                    set1.begin(), set1.end(),
                    std::back_inserter(res_difference2));
// 求对称差集
std::set_symmetric_difference(set1.begin(), set1.end(),
                              set2.begin(), set2.end(),
                              std::back_inserter(res_symmetric_difference));
// 判断是不是包含关系
std::cout << std::boolalpha << 
          << std::includes(set1.begin(), set1.end(), set2.begin(), set2.end())
          << std::endl;
// --------------------------------------------------

// 

// --------------------------------------------------