// 序列递增 -----------------------------------------------------
std::vector<double> data(9);
std::itoa(data.begin(), data.end(), 1.5);
/* 结果 data: [ 1.5, 2.5, 3.5, ..., 9.5 ] */
std::cout << data;
// ------------------------------------------------------------

// 序列求和 -----------------------------------------------------
std::vector<int> values{2, 0, 12, 3, 1};
/* 默认直接累加 */
auto sum = std::accumulate(values.begin(), values.end(), 0);
/* 自定义累加函数 */
auto sum =
    std::accumulate(values.begin(), values.end(), 0, [](int sum, int curr) {
      /* 只计算奇数的和 */
      return sum + curr * (curr & 0x1);
    });
// ------------------------------------------------------------

// 内积 --------------------------------------------------------
std::vector<double> x{ 1.0, 1.5, 2.5 };
std::vector<double> y{ 1.5, 2.5, 3.5 };
/* 计算结果 = 1.0*1.5 + 1.5*2.5 + 2.5*3.5 */
auto z = inner_product(x.begin(), x.end(), y.begin(), 0);
// ------------------------------------------------------------

// 相邻差 ------------------------------------------------------
std::vector<int> data { 2, 3, 5, 7, 11, 13, 17, 19 }, result;
/* 求得输入序列中相邻元素对的差; 第一个元素会原封不动地保留 */
std::adjacent_difference(data.begin(), data.end(), back_inserter(result));
/* 结果 = [ 2, 1, 2, 2, 4, 2, 4, 2 ] */
std::cout << data << std::endl;

/* 使用自定义求差公式; 改为求相邻和; 实现斐波那契 数列 */
std::vector<size_t> fib(15, 1); // 初始化为1 的数列 */
/* 结果 = [ 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610 ] */
std::adjacent_difference(fib.begin(), fib.end()-1, std::begin()+1, std::plus<size_t>());
// ------------------------------------------------------------

// 前缀和 ------------------------------------------------------
std::vector<int> data { 1, 2, 3, 4, 5, 6 }, result;
std::partial_sum(data.begin(), data.end(), back_inserter(result));
/* 结果 = [ 1, 3, 6, 10, 15, 21 ] */
std::cout << data << std::endl;
// ------------------------------------------------------------

// 极大值 极小值 ------------------------------------------------
vector<int> data { 2,  12, 3, 5, 17, -11, 113, 117, 19 };
std::cout << "minimum: " << *std::min_element(data.begin(), data.end()) << std::endl;
std::cout << "maximum: " << *std::max_element(data.begin(), data.end()) << std::endl;
auto pr = minmax(data);
std::cout << "minimum: " << pr.first;
std::cout << "maximum: " << pr.second;
// ------------------------------------------------------------
