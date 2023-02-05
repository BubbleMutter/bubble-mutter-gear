// 容器拷贝 --------------------------------
std::vector<string> names { "Alan", "Beth", "Carol", "Dan", "Eve", 
                            "Fred", "Iain", "Harry", "George", "Joe" };
std::unordered_set<string> more_names;

/* inserter 为没有后向 特性的容器; 提供后插入特性 */

/* 拷贝指定个数 */
std::copy_n(names.begin(), 3, std::inserter(more_names, more_names.begin()));

/* 拷贝满足条件 */
std::copy_if(names.begin(),
             names.end(),
             inserter(more_names, more_names.begin()),
             [](const string& s) { s.size() == 3; });

/* 往后复制 */
vector<int> nums { 10, 20, 30, 40, 50 };
nums.resize(nums.size() + 3);

std::copy_backwards(nums.begin(), nums.begin()+nums.size(), nums.end());
/* 复制结果: 10 20 30 10 20 30 40 50


/* 翻转复制 */
vector<int> reversed;
std::reverse_copy(nums.begin(), nums.end(), back_inserter(reversed));

/* 唯一复制 */
std::string text("There's no air in spaaaaaaaace!"), result;
std::unique_copy(text.begin(), text.end(), back_inserter(result));

/* 删除相邻重复元素 */
text.resize(distance(text.begin(), unique(text.begin(), text.end())));
// ----------------------------------------