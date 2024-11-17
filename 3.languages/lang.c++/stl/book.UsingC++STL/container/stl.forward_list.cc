// 初始化列表
std::forward_list<std::string> words { "three", "six", "eight" };
int size = distance(words.begin(), words.end());

// 前移迭代器
std::forward_list<int> data { 10, 21, 43, 87, 175, 351 };
auto iter = data.begin();
std::distance(iter, 3);

// splice_after -------------------------------------------------
std::forward_list<std::string> abc_words { "three", "six", "eight" };
std::forward_list<std::string> xyz_words { "seven", "four", "nine" };
// 移动一个元素
abc_words.splice_after(abc_words.before_begin() /* 目标迭代器 */,
                       xyz_words /* 待移动容器 */,
                       xyz_words.begin()+1 /* 待移动迭代器 */);
// 移动一段元素
abc_words.splice_after(abc_words.begin() /* 目标迭代器 */,
                       xyz_words /* 待移动容器 */,
                       xyz_words.begin()+1 /* 待移动范围 */,
                       xyz_words.end() /* 待移动范围 */);
// 移动一整个容器
abc_words.splice_after(abc_words.before_begin(), xyz_words);
// --------------------------------------------------------------

// --------------------------------------------------------------
std::forward_list<int> data;
// stdin 输入
std::copy(std::istream_iterator<int>(std::cin),
          std::istream_iterator<int>(),
          std::front_inserter(data));
// stdout 输出
std::copy(data.begin(), data.end(),
          ostream_iterator<int>(std::cout, " "));
// --------------------------------------------------------------

// --------------------------------------------------------------
std::list<double> values { 1.414, 3.14159265, 2.71828 };
std::stack<double, std::list<double>> stk_values(values);
std::stack<double, std::list<double>> stk_values2(stk_values);
// --------------------------------------------------------------
