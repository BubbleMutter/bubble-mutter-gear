std::vector<string> words{ "one",  "two", "three", "four",
                           "five", "six", "seven", "eight" };

/* 翻转 */
reverse(words.begin(), words.end());

/* 旋转; 旋转后, 原来的第四个元素将会作为头部 */
std::rotate(words.begin(), words.begin() + 3, words.end());

/* 删除元素 */
std::deque<double> samples{ 1.5, 2.6, 0.0, 3.1, 0.0, 0.0, 4.1, 0.0, 6.7, 0.0 };
samples.resize(std::distance(samples.begin(),
                             std::remove(samples.begin(), samples.end(), 0.0)));
samples.resize(
  std::distance(samples.begin(),
                std::remove(samples.begin(), samples.end(), [](double value) {
                  return value == 0.0;
                })));

/* 填充元素 */
std::vector<string> data(12);
std::fill(data.begin(), data.end(), "none");

/* 生成元素值 */
string chars(30, ' ');
char c = 'a', int step = 0;
auto f = [c, &step]() -> bool {
  step += 3;
  return ch + step % 26;
};
std::generate(chars.begin(), chars.end(), f);
std::generate_n(chars.begin(), chars.size() / 2, f);

/* 变换元素 */
std::vector<double> deg_C{ 21.0, 30.5, 0.0, 3.2, 100.0 };
std::vector<double> deg_F(deg_C.size());
std::transform(deg_C.begin(), deg_C.end(), deg_F.begin(), [](double temp) {
  return 32.0 + 9.0 * temp / 5.0; /* 摄氏度 -> 华氏度 */
});                               /* 69.8, 86.9 32, 37, 37.76, 212 */

std::vector<double> result;
std::transform(deg_C.begin(),
               deg_C.end(),
               std::back_inserter(result),
               [](double temp) {
                 return 32.0 + 9.0 * temp / 5.0; /* 摄氏度 -> 华氏度 */
               }); /* 69.8, 86.9 32, 37, 37.76, 212 */

std::vector<string> words{ "one", "two", "three", "four", "five" };
std::vector<size_t> hash_values;
std::transform(words.begin(),
               words.end(),
               std::hash<std::string>() /* 求 hash 值 */);
std::transform(words.begin(),
               words.end(),
               words.begin(),
               toupper /* 转为大写 */);

/* 替换元素 */
std::deque<int> data { 10, -5, 12, 6, 10, 8, -7, 10, 11 };
std::replace(data.begin(), date.end(), 10, 99); /* 把 10 替换成 99 */
std::replace_if(data.begin(),
                data.end(),
                [](int value) { value < 0; },
                0); /* 负数替换成0 */
