
// 查找元素 -----------------------------------------
std::vector numbers{5, 46, -5, -6, 23, 17, 5, 9, 6, 5};

/* 顺序查找, 找到第一个相等的元素 */
auto iter = find(numbers.begin(), numbers.end(), 23);

/* 顺序查找, 找到第一个满足条件的元素 */
auto iter =
    find_if(numbers.begin(), numbers.end(), [](int n) { return n > 23; });

/* 顺序查找, 找到第一个不满足条件的元素 */
auto iter =
    find_if_not(numbers.begin(), numbers.end(), [](int n) { return n > 5; });

/* 顺序查找, 在第一个容器中, 找到第二个容器中第一次出现的任一元素 */
std::string text("The world of searching");
std::string vowels("aeiou");
std::string found;
for (auto iter = text.begin();
     (iter = std::find_first_of(text.begin(), text.end(), vowels.begin(),
                                vowels.end()) != text.end();
      ++iter);) {
  // 找到 text 中所有的元音字母
  found += *iter;
}

/* 查找序列中两个连续相等的元素 */
string saying("Children should be seen and not heard");
auto iter = std::adjacent_find(saying.begin(), saying.end());
if (iter != saying.end()) {
  /* 将会输出字母 'e' */
  std::cout << "The repeated charater is " << *iter << endl;
}
// -------------------------------------------------

// 查找子序列 ----------------------------------------
string text(
    "Smith, where Jones had had \"had\", had had \"had had\"."
    " \"Had had\" had had the examiners\' approval.");
string phrase("had had");
/* 在第一个序列中查找第一个和另一个元素段匹配的匹配项;
 * 也可以看作在一个元素序列中查找子序列的第一个匹配项;
 * 返回一个指向子序列的第一个匹配项的第一个元素的迭代器 */
size_t found_count = 0;
auto iter = text.begin();
while ((iter = std::search(iter, text.end(), phrase.begin(),
                           phrase.end())) != text.end()) {
  cout << "\"had had\" was found at index "
       << std::distance(text.begin(), iter) << std::endl;
  found_count++;
}

/* 在第一个序列中查找最后一个和另一个元素段匹配的匹配项;
 * 也可以看作在一个元素序列中查找子序列的最后一个匹配项;
 * 返回一个指向子序列的最后一个匹配项的第一个元素的迭代器 */
size_t found_count = 0;
auto iter = text.begin();
while ((iter = std::find_end(iter, text.end(), phrase.begin(),
                             phrase.end())) != text.end()) {
  cout << "\"had had\" was found at index "
       << std::distance(text.begin(), iter) << std::endl;
  found_count++;
}
while ((iter = std::find_end(text.begin(), text.end(), phrase.begin(),
                             phrase.end(), [](char x, char y) {
                               return std::toupper(x) ==  /* 指定比较函数 */
                                      std::toupper(y);    /* 不区分大小写 */
                             })) != text.end()) {
  cout << "\"had had\" was found at index "
       << std::distance(text.begin(), iter) << std::endl;
  found_count++;
}

/* 查找 给定元素在序列中连续出现给定次数 的子序列 */
std::vector<double> values { 2.7, 2.7, 2.7, 3.14, 3.14, 3.14, 2.7, 2.7 };
double value = 3.14;
size_t times = 3;
auto iter = std::search_n(values.begin(), values.end(), times, value);
if (iter != values.end()) {
    cout << times << " successive instances of " << value
         << " found starting index " << std::distance(values.begin(), iter);
}
// -------------------------------------------------

// 二分查找 -----------------------------------------
/* 在已排序的序列中; 确定某元素是否存在 */
std::list<int> values{ 17, 11, 40, 36, 22, 54, 48, 70,
                       61, 82, 78, 89, 99, 92, 43 };
std::cout << std::boolalpha << binary_search(values.begin(), values.end(), 70);

/* 在已排序的序列中; 找到不小于第三个参数的第一个元素; 
 * leetcode 00035. Search Insert Position */
std::cout << "lower bound = " << *lower_bound(values.begin(), values.end(), 70);

/* 在已排序的序列中; 找到首个大于第三个参数的元素; 
 * leetcode 00744. Find Smallest Letter Greater Than Target */
std::cout << "upper bound = " << *upper_bound(values.begin(), values.end(), 70);

/* 一次返回 lower bound 和 upper bound 两个结果 */
auto pr = std::equal_range(values.begin(), values.end(), 70);
std::cout << "lower bound = " << *(pr.first);
std::cout << "upper bound = " << *(pr.second);
// -------------------------------------------------