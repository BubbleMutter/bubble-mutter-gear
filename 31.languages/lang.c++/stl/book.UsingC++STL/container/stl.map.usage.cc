// --------------------------------------------------
// 初始化列表 
std::map<std::string, size_t> people {
    { "Anne", 25 }, { "Bill", 46 },
    { "Jack", 32 }, { "Jill", 32 },
};
// 复制现有容器 
std::map<std::string, size_t> personnel(people);
// 使用迭代器方式指定元素
std::map<std::string, size_t> personnel(people.begin(), people.end());
// 插入元素
people.insert({"Fred", 22});
// 查询指定键对应的元素数目
int found = people.count("Ian");
// 遍历 map 的键值对
for (const auto & p : people)
    std::cout << std::setw(10) << std::left << p.first << " " << p.second << endl;

try {
    auto age =  people.at("Bill");
    std::cout << "Bill is " << age << endl;
} catch (const std::out_of_range & e) {
    std::cerr << e.what() << '\n' << key << " was not found." << endl;
}

// 取得类型
using vtype = std::map<size_t, string>::value_type;
using ktype = std::map<size_t, string>::key_type;

// --------------------------------------------------

// multimap -----------------------------------------
std::multimap<string, string> pets;
pets.insert(make_pair(string("dog"), string("Spot")));
pets.insert(std::pair<string, string>(string("dog"), string("Rover")));
pets.insert({"cat", "Korky"});
pets.insert({{ "rat", "Rolnad" }, { "pig", "Pinky" }, { "pig", "Perky" }});
pets.emplace("rabbit", "Flopsy");
pets.insert(std::pair<string, string>(string("dog"), string("Fang")));
pets.insert(make_pair(string("dog"), string("Rover"))); // 不会覆盖; 可以同时存在
pets.insert(make_pair(string("cat"), string("Korky")));

// multimap 查询 key 的一个元素
auto single = pets.find("dog");
if (single != pets.end())
    cout << "name: " << single->second << endl;

// multimap 查询 key 的所有元素
auto region = pets.equal_range("dog");
for (auto p = region.first; p != region.second; ++p) {
    cout << "name: " << p->second << endl;
}
// --------------------------------------------------

// 修改比较函数 ---------------------------------------
// 第三个模板参数, 默认赋值 std::less<第一个模板参数> 升序
// 如果想要改为升序, 则使用 greater
std::map<std::string, std::string, std::greater<std::string>> people;
// --------------------------------------------------

// hash 值    ---------------------------------------
std::hash<int> hash_method; // 函数对象; 用于生成 hash 值; 内有成员 operator ()(int)
std::vector<int> vec { -5, -2, 2, 5, 10};
std::vector<int> res;
std::transform(vec.begin(), vec.end(), back_inserter(res), hash_method);
// --------------------------------------------------
 
// unordered_map ------------------------------------
// unordered_map 的比较函数; 默认是 equal_to<第一个模板参数> 生成的函数对象
// 初始化列表
std::unordered_map<std::string, size_t> people {
    { "Jan", 44 }, { "Jim", 33 }, { "Joe", 99 }
};
// 从其他包含 pair 成员的容器中生成
std::vector<std::pair<string, size_t>> folks {
    { "Jan", 44 }, { "Jim", 33 }, { "Joe", 99 },
    { "Dan", 22 }, { "Ann", 55 }, { "Don", 77 }
}
std::unordered_map<std::string, size_t> neighbors(folks.begin(), folks.end());

// 获取槽位数
cout << "bucket_count = " << people.bucket_count() << endl;
// 获取负载因子
cout << "max_load_factor = " << people.max_load_factor() << endl;
// 调整槽位数
people.rehash(2*people.bucket_cout()); // 扩容
// 调整负载因子
people.max_load_factor(1.2 * people.max_load_factor());
// 获取当前负载系数
cout << "load_factor = " << people.load_factor() << endl;

// 逐个槽位遍历
for (size_t i = 0;  i < people.bucket_count(); ++i) {
    for (auto iter = people.begin(i); iter != people.end(i); ++iter) {
        std::cout << "key = " << iter->first << " , " << "value = " << iter->second << endl;
    }
}
// --------------------------------------------------
