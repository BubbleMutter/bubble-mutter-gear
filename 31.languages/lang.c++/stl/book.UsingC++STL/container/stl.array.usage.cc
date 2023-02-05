void usage {
// 定义一个未被初始化的值
std::array<double, 100> data;
// 所有元素都初始化为零
std::array<double, 100> data { };
// 没在初始化列表中的将被初始化为零
std::array<double, 10> values { 0.5, 1.0, 1.5, 2.0 }; 
values.fill(3.1415926); // Set all elements to pi

values[4] = values[3] + 2.0*values[1];
// 运行时消耗额外性能检查是否越界
values.at(4) = values.at(3) + 2.0*values.at(1);

double total = 0;
for (size_t i = 0; i < values.size(); ++i) {
    total += values[i];
}

if (values.empty()) {
    std::cout << "The container has no elements." << std::endl;
} else {
    std::cout << "The container has " << values.size() << " elements.\n" << endl;
}

// 使用 get 模板函数; 在编译时 检查是否越界
std::array<std::string, 5> words { "one", "two", "three", "four", "five" };
std::cout << std::get<3>(words) << std::endl; // Output words[3]
std::cout << std::get<6>(words) << std::endl; // Compiler Error Message

// generate 算法模板函数, 填充容器
std::array<unsigned int, ht_count> height_ins;
std::generate(height_ins.begin(), height_ins.end(),
             [height, &min_ht, &ht_step] () mutable 
             { return height += height == 0 ? min_ht : ht_step; });

// iota 用递增值初始化容器
std::array<double, 10> values;
std::iota(values.begin(), values.end(), 10.0); // [ 10.0, 19.0 ]

// 相同元素个数的数组容器比较大小
std::array<dobule, 4> these { 1.0, 2.0, 3.0, 4.0 };
std::array<double, 4> those { 1.0, 2.0, 3.0, 4.0 };
std::cout << these == those ? "same" : "diff" << std::endl;
