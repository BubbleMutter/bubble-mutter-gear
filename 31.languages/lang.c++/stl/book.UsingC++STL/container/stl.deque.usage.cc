/**
 * @file stl.deque.usage.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-20
 * 
 * @copyright Copyright (c) 2021
 */
#include <deque>

deque<int> dummy_deque;
dqueu<int> deque;

// 初始化列表
std::deque<std::string> words { "one", "none", "some", "all", "none", "most", "many" };
// 拷贝构造函数; 
std::deque<std::string> words_copy(words);
// 两个迭代器构造
std::deque<std::string> words_part(words.begin(), words.end()+5);

// 带边界检查的随机访问
std::cout << words.at(2) << std::endl;

// 增删容器元素
std::deque<int> numbers { 2, 3, 4 };
numbers.push_front(11); // 添加元素到头部
numbers.push_back(12);  // 添加元素到尾部
numbers.pop_front();    // 删除头部元素
numbers.pop_back();     // 删除尾部元素

// 替换容器元素
deque<std::string> words { "black", "white", "red" };
word.assign({ "one", "two", "three", "four" });
word = { "symphony", "sonota", "concerto" };

// 从 stdin 读取数据
std::copy(std::istream_iterator<string>(std::cin),
          std::istream_iterator<string>(),
          std::front_inserter(names));
// 输出数据到 stdout
std::copy(names.begin(), names.end(),
          ostream_iterator<string>(std::cout, " "));