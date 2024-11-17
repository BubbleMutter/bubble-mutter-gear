// 分区序列 -----------------------------------------
/* 根据平均值分区序列; 小于平均值的在前面; 大于平均值的在后面 */
std::vector<int> temperatures{ 65, 75, 56, 48, 31, 28, 32, 29, 40, 41, 44, 50 };
auto average = accumulate(temperatures.begin(), temperatures.end(), 0.0) /
               temperatures.size();
auto f = [average](int value) { return value <= average; };
std::partition(temperatures.begin(),
               temperatures.end(),
               f /* 返回 true 在前面 */);
// 分区结果 44 41 40 29 31 28 32 48 56 75 65 50 (破坏了满足同一条件的元素的原始顺序)

/* 分区时, 保证元素的相对排序 */
std::stable_partition(temperatures.begin(),
                      temperatures.end(),
                      f /* 返回 true 在前面 */);
// 分区结果 31 28 32 29 40 41 44 65 75 56 48 50

/* 分区结果输出到 另外两个容器 */
std::vector<int> lower, upper;
std::partition_copy(temperatures.begin(),
                    temperatures.end(),
                    std::back_inserter(lower),
                    std::back_inserter(upper),
                    f);

/* 针对已分区的容器; 取得中间边界; 即 [begin, point), [point, end) */
auto point = partition_point(temperatures.begin(), temperatures.end(), f);

/* 判断队列是否分区 */
std::cout << std::boolalpha
          << is_partitioned(temperatures.begin(), temperatures.end(), f);
// -------------------------------------------------