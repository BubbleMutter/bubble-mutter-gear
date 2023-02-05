
std::list<std::string> words;
std::list<std::string> sayings(20); // 20 个空的字符串
std::list<double> values(50, 3.14); // 50 个 3.14 元素
std::list<double> save_values(values); // dupclicate of values
std::list<double> samples(++values.cbegin(), --values.cend());


std::list<std::string> names { "Jane", "Jim", "Jules", "Janet" }; // 初始化列表
names.push_front("Ian");  // 头部插入元素 (存在右值构造函数, 触发移动语义)
names.push_back("Kitty"); // 结尾插入元素 (存在右值构造函数, 触发移动语义)
// emplace 直接调用参数的构造函数; 省去 push_front/push_back 的移动
names.emplace_front("Ian");
names.emplace_back("Kitty");

// insert --------------------------------------------------
// 初始化 10个 元素值为 55的
std::list<int> data(10, 55); 
// 插入 66 到第二个元素的前面
data.insert(data.begin()+1, 66);
// 插入  3 个88到第7个元素
data.insert(std::advance(data.begin(), 6), 3, 88);
// 插入 迭代器指定的另一段元素
vector<int> numbers(10, 5); // 10 个值是 5 的元素
data.insert(data.end()-2, numbers.begin(), numbers.end());
// ---------------------------------------------------------

std::list<int> numbers { 2, 5, 2, 3, 6, 7, 8, 2, 9 };
// 删除所有元素 2
numbers.remove(2);
// 删除奇数的元素
numbers.remove_if([](int n) { return n % 2 == 0; });

// 删除连续的重复元素; 只剩下其中一个; 一般配合 sort 后使用
std::list<std::string> words {  "one", "two", "two", "two", "three", "four", "four" };
words.unique(); 

// 排序 ----------------------------------------------------
// 指定比较函数方法1: functional 模板函数类
names.sort(std::greater<std::string>());
// 指定比较函数方法2: lambda 表达式
names.sort([](const std::string &s1, const std::string &s2) -> bool {
    if (s1[0] == s2[0])
        return s1.length() > s2.length();
    else
        return s1 > s2;
});
// 指定比较函数方法3: 函数类实现 operator() 方法
class my_greater {
public:
    bool operator() (const std::string &s1, const std::string &2) {
        if (s1[0] == s2[0])
            return s1.length() > s2.length();
        else
            return s1 > s2;
    }
};
names.sort(my_greater());
// ---------------------------------------------------------

// merge ---------------------------------------------------
std::list<int> Jane_values {  2, 4, 6, 14 };
std::list<int> John_values { -2, 1, 7, 10 };
// John 就会变空; 两个容器的元素都在 Jane 中
// 合并前两个容器必须是 已经排序的
Jane_values.merge(John_values);
Joh_values.empty(); // return true;
// ---------------------------------------------------------

// merge 指定排序函数 ----------------------------------------
std::list<std::string> pin_words { "three", "six", "eight" };
std::list<std::string> pon_words { "seven", "four", "nine" };
auto f = [](const std::string& s1, const std::string& s2) {
    return s1[0] < s2[0];
}
pin_words.sort(f); // "eight" "six" "three"
pop_words.sort(f); // "four" "nine" "seven"
// "eight" "four" "nine" "six" " seven" "three"
pin_words.merge(pop_words, f);
// ---------------------------------------------------------

// splice 移动元素 ------------------------------------------
std::list<std::string> abc_words { "three", "six", "eight" };
std::list<std::string> xyz_words { "seven", "four", "nine" };
// 移动一个元素
// 移动后:
//    --> abc_words: three `four` six eight
//    --> xyz_words: seven nine
abc_words.splice(abc_words.begin()+1 /* 目标迭代器 */,
                 xyz_words /* 待移动容器 */,
                 xyz_words.begin()+1 /* 待移动迭代器 */);

// 移动一段元素
// 移动后:
//    --> abc_words: three
//    --> xyx_words: seven { four six eight } nine
xyz_words.splice(xyz_words.begin()+1 /* 目标迭代器 */,
                 abc_words /* 待移动容器 */,
                 abc_words.begin()+1 /* 待移动范围 */,
                 abc_words.end()/* 待移动范围 */);

// 移动全部元素
abc_words.splice(abc_words.begin(), xyz_words);
xyz_words.splice(xyz_words.end(), abc_words);
// --------------------------------------------------------


// getline 读取键盘输入到容器中
string proverb;
while (getline(std::cin, proverb, '\n'), !proverb.empty())
    proverbs.push_front(proverb);