string s1("this"), s2("that");
std::pair<std::string, std::string> my_pair(s1, s2);
std::pair<std::string, std::string> your_pair(string("this"), string("that"));

auto my_pair = std::make_pair(s1, s2);
auto your_pair = std::make_pair(string("this"), string("that"));
auto his_pair = std::make_pair<std::string, std::string>("test", std::string("that"));
auto her_pair = std::make_pair<std::string, std::string>("test", "that");

// forward_as_tuple 转发参数生成了一个 tuple 引用
// 如果参数是一个临时对象 forward_as_tuple 会生成一个右值引用的 tuple
const auto& c = std::forward_as_tuple(a, b);
const auto& c = std::forward_as_tuple(1, 2);

// Default Constructor
std::pair<std::string, std::string> new_pair(std::string("his"), std::string("her"));
// Copy Assignment
std::pair<std::string, std::string> old_pair = new_pair;
// Move Assignment
std::pair<std::string, std::string>(std::string("these"), std::string("those"));

// pair 对象有全套的运算符 ==, !=, <, <=, >, >=
std::cout << std::boolalpha <<
    new_pair == std::pair<std::string,  std::string>("his", "her") <<
    std::endl;

// 交换内部元素
new_pair.swap(old_pair);
old_pair.swap(new_pair);


std::tuple<std::string, std::string> mtuple1; // Default initialiation
std::tuple<std::string, std::string> mtuple2(string("Andy"), string("Capp"));
std::tuple<std::string, std::string> mtuple3(mutuple2); // Copy Constructor
std::tuple<std::string, std::string> mtuple4(make_pair("this", "that"));
std::tuple<std::string, std::string> mtuple5(std::pair<string, string>("this", "that"));
auto mtuple6 = make_tuple("hello", "world");
std::cout << std::boolalpha << mtuple4 == mtupl5 << std::endl; // 支持比较

std::cout << std::get<0>(mtuple2); // 读取元素
std::cout << std::get<1>(mtuple2); // 读取元素

std::string member1, member2;
std::tie(member1, member2) = mtuple4; // 拷贝读取
std::tie(std::ignore, member2) = mtuple3; // 忽略部分元素
