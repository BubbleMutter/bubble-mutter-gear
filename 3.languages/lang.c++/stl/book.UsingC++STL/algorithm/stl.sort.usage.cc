/**
 * @brief book Using C++ STL; note algorithm sort/merge/search/partition
 */
std::vector<int> numbers{99, 77, 33, 66, 22, 11, 44, 88};
// 排序; 升序
std::sort(numbers.begin(), numbers.end());
// 排序; 降序
std::sort(numbers.begin(), numbers.end(), greater<>());
// --------------------------------------------------

// 排序; 自定义比较方法 --------------------------------
std::deque<string> words{"one",   "two",  "nine", "nine", "one",
                         "three", "four", "five", "six"};
std::sort(words.begin(), words.end(),
          [](const std::string& x, const std::string& y) {
            return x.back() > y.back();
          });
// --------------------------------------------------

// 稳定排序 -------------------------------------------
stable_sort(words.begin(), words.end(),
            [](const std::string& x, const std::string& y) {
              return x.front() > y.front()
            });
// --------------------------------------------------

// 部分排序 -------------------------------------------
// 适合从超大容器中, 获取最小 count 个, 或者最大 count 个
// 最小的 count 个
std::partial_sort(numbers.begin(), numbers.begin() + count, numbers.end());
// 最大的 count 个
std::partial_sort(numbers.begin(), numbers.begin() + count, numbers.end(),
                  greater<>());
// --------------------------------------------------

// 单个排序 -------------------------------------------
// 适合从超大容器, 获取最小的第count个, 或者最大的第count个
// 排序完成后, numbers[count] 的元素是完全排序后的位置, 即最小的第count个
// 但其他元素位置上的元素是不确定的
// 不过, numbers[0]~numbers[count-1] 比 numbers[count] 小
//      numbers[count+1] ~ numbers[-1] 都比 numbers[count] 要大
// 场景: 只想获得第n个元素, 但无需获得前n个元素, 性能要比 partial_sort 快
nth_element(numbers.begin(), numbers.end() + count, numbers.end());
// --------------------------------------------------

// 把前 count 个结果输出到另一个容器中; 不改变原容器元素 ----
// 此时 通过目标容器的尺寸, 指示被排序元素的数目
std::vector<int> result(count);
std::partial_sort_copy(numbers.begin(), numbers.end(), result.begin(),
                       numbers.end());
std::partial_sort_copy(numbers.begin(), numbers.end(), result.begin(),
                       numbers.end(), greater<>());
// --------------------------------------------------

// 检查容器是否已经排序 --------------------------------
std::cout << is_sorted(numbers.begin(), numbers.end()) << std::endl;
std::cout << is_sorted(numbers.begin(), numbers.end(), std::greater<>())
          << std::endl;
// --------------------------------------------------

// 合并两个有序容器 -----------------------------------
std::vector<int> these{2, 15, 4, 11, 6, 7};
std::vector<int> those{5, 2, 3, 2, 14, 6};
std::stable_sort(these.begin(), these.end());
std::stable_sort(those.begin(), those.end());
std::vector<int> result;
std::merge(these.begin(), these.end(), those.begin(), those.end(),
           back_inserter(result));
// -------------------------------------------------

