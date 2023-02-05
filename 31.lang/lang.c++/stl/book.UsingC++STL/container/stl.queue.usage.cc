std::queue<std::string> words;
std::queue<std::string> copy_words(words);

// queue<T> 默认封装 deque<T>, 但是可以选择 list<T>
std::queue<std::string, std::list<std::string>> words;

// 获取队列头元素
std::string head_word = words.front();
// 获取队列尾元素
std::string tail_word = words.back();
// 在队列尾部加入元素
words.push(std::string("apple"));
// 在队列头部去掉元素
words.pop();
// 判断队列是否为空
cout << "queue is " << words.empty() ? "empty" : "not empty" << endl;
// 返回队列尺度
cout << "queue size is " << words.size() << endl;