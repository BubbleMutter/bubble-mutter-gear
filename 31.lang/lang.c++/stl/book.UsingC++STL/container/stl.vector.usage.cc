std::vector<double> values;
values.reserve(20); // 增加容器容量 (只能增加, 如果传入参数小于大小, 函数无行为)
values.resize(10); // 改变容器大小

// 初始化列表 
std::vector<unsigned int> primes { 2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u };

// 初始化 20 个元素的 vector, 未初始化
std::vector<double> values(20);
// 初始化  1 个元素的 vector
std::vector<double> values{20};
// 初始化 20 个元素的 vector, 初始化为 99L
std::vector<long> numbers(20, 99L);

std::array<std::string, 5> words { "one", "two", "three", "four", "five" };
// 从迭代器初始化一个 vector (copy)
std::vector<std::string> words_copy(words.begin(), words.end());
// 从迭代器初始化一个 vector (move)
std::vector<std::string> words_move(make_move_iterator(words.begin()),
                                    make_move_iterator(words.end()));

// 元素访问
std::vector<double> values(20);
values[0] = 3.14159;
values[1] = 5.0;
values[2] = 2.0*values[0]*values[1];
values.front() = 2.71828;   // 第一个元素的引用
values.back() = 3.14159;    // 最后一个元素的引用
auto pData = values.data(); // 底层连续数组指针

copy(words.begin(), words.end(), ostream_iterator<int>(std::cout, " "));

vector<int> word;
// 从 stdin 读取数据
copy(istream_iterator<double>(cin), istream_iterator<double>(), back_inserter(word));
// 输出数据到 stdout
copy(word.begin(), word.end(), ostream_iterator<double> (std::cout, " "));

// 移动一个容器
vector<string> word_copy(std::make_move_iterator(word.begin()),
                         std::make_move_iterator(word.end()));

// 新增元素
std::vector<double> values;
values.push_back(3.1415926);
std::vector<string> words;
words.push_back("symphony");
words.emplace_back(string("alleged"), 2, 3); // 插入字符串 "leg", 插入到末尾
words.emplace(words.begin(), 3, 'A');  // 插入字符串 'AAA' 到指定迭代器
words.insert(words.begin(), "hurt");   // 插入字符串 'hurt' 到指定迭代器
words.insert(words.end(), "sonata");   // 插入字符串 'sonata' 到容器的结尾处

// 删除元素 ---------------------------------------------------------------
std::vector<int> data(100, 99); // Contains 100 elements initialized to 99
data.clear(); // Remove all elements
std::vector<int> data(100, 99); // Contains 100 elements initialized to 99
data.pop_back(); // Remove the last element

// 删除第二个元素; 先交换到末尾; 再删除末尾
std::swap(data.begin()+1, data.end()-1);
std::pop_back();
// -----------------------------------------------------------------------

data.shrink_to_fit(); // 去掉容器中多余的容量
data.erase(data.begin()+1); // 删除第二个元素

vector<string> data { "yes", "no", "none", "do", "none", "yes" };

// 删除匹配目标的元素 方法1: resize + distance + remove
data.resize(distance(data.begin(), remove(data.begin(), data.end(), "none")));
// 删除匹配目标的元素 方法2: erase + remove
data.erase(remove(data.begin(), data.end(), "yes"), data.end());
