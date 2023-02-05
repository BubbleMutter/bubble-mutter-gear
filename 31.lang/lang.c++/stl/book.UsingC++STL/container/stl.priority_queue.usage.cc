std::priority_queue<std::string> words;
std::priority_queue<std::string> words2 { "one", "two", "three", "four" };
std::priority_queue<std::string> words3(words2);

// 默认队列是大顶堆; pop 操作返回最大元素; 想要小顶堆的方式
std::priority_queue<std::string, std::vector<std::string>, std::greater<std::string>> words4(words3);

// 判断 priority_queue 为空
while (!words_copy.empty()) {
    std::cout << words_copy.top() << " ";
    words_copy.pop();
}
std::cout << std::endl;

// 入列对象 emplace
words.emplace("nine");
// 入列对象 push
words.push("nine");
