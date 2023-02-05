std::vector<double> numbers { 2.5, 10.0, 3.5, 6.5, 8.0, 12.0, 1.5, 6.0 };

// 生成大顶堆
std::make_heap(numbers.begin(), numbers.end());
// 生成小顶堆
std::make_heap(numbers.begin(), numbers.end(), std::greater<>());


// 新增一个元素后维持堆结构
std::make_heap(numbers.begin(), numbers.end());
numbers.push_back(11);
std::push_heap(numbers.begin(), numbers.end());

// 新增一个元素后维持小顶堆结构
std::make_heap(numbers.begin(), numbers.end(), std::greater<>());
numbers.push_back(1.2);
std::push_heap(numbers.begin(), numbers.end(), std::greater<>());

// 去掉大顶堆中的最大元素; 然后维持堆结构
std::make_heap(numbers.begin(), numbers.end());
std::pop_heap(numbers.begin(), numbers.end());
numbers.pop_back();

// 去掉小顶堆中的最小元素; 然后维持堆结构
std::make_heap(numbers.begin(), numbers.end(), std::greater<>());
std::pop_heap(numbers.begin(), numbers.end(), std::greater<>());
numbers.pop_back();

// 判断是否是堆
if (std::is_heap(numbers.begin(), numbers.end()))
    std::cout << "It's a heap" << std::endl;
else
    std::cout << "It's not a heap." << std::endl;

// 构造成大顶堆; 然后排序成升序
std::vector<double> numbers { 2.5, 10.0, 3.5, 6.5, 8.0, 12.0, 1.5, 6.0 };
make_heap(numbers.begin(), numbers.end());
sort_heap(numbers.begin(), numbers.end());

// 构造成小顶堆; 然后排序成降序
std::vector<double> numbers { 2.5, 10.0, 3.5, 6.5, 8.0, 12.0, 1.5, 6.0 };
make_heap(numbers.begin(), numbers.end(), std::greater<>());
sort_heap(numbers.begin(), numbers.end(), std::greater<>());

