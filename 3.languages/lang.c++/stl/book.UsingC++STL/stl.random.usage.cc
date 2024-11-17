/* 生成一个随机数 */
std::random_device rd;
std::cout << rd() << std::endl;

/* 构造种子序列 */
std::seed_seq seeds1; /* 默认构造; 该对象已具备生成特性 */
/* 通过初始化列表; 传入种子数 */
std::seed_seq seeds2{ 2, 3, 4, 5 }; 
std::vector<int> data { 25, 36, 47, 58 };
/* 通过迭代器; 传入种子数 */
std::seed_seq seeds3(data.begin(), data.end()); 

std::vector<int> numbers(10);
/* 种子序列 生成伪随机数 */
seeds.generate(numbers.begin(), numbers.end());