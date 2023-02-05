std::vector<int> ages{ 22, 19, 46, 75, 54, 19, 27, 66, 61, 33, 22, 19 };

/* 全都不满足条件 */
std::cout << "None of `ages' is negative " << std::boolalpha
          << std::none_of(ages.begin(), ages.end(), [](int value) {
               return value < 0;
             });

/* 全都满足条件 */
std::cout << "All of `ages' is less than 100 " << std::boolalpha
          << std::none_of(ages.begin(), ages.end(), [](int value) {
               return value < 100;
             });

/* 其中一个满足条件即可 */
std::cout << "Any of `ages' is greater than 50 " << std::boolalpha
          << std::any_of(ages.begin(), ages.end(), [](int value) {
               return value > 50;
             });

/* 检查满足条件的元素个数 */
std::cout << std::count(ages.begin(), ages.end(), 30);
std::cout << std::count_if(ages.begin(), ages.end(), [](int value) {
  return value & 0x1;
});
