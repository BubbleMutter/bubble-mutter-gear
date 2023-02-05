// 自定义类; 输入流 和 输出流 -------------------------------------
class Name {
 private:
  std::string m_first;
  std::string m_second;

 public:
  Name() = default;
  Name(const std::string& first, const std::string& second)
      : m_first(first), m_second(second) {}
  friend std::istream& operator>>(std::istream& in, Name& name);
  friend std::ostream& operator<<(std::ostream& out, const Name& name);
};

inline std::istream& operator>>(std::istream& in, Name& name) {
  return in >> name.m_first >> name.m_second;
}

inline std::ostream& operator<<(std::ostream& out, const Name& name) {
  return out << "Your name is: " << name.m_first << " " << name.m_second;
}

{
  Name name;
  std::cout << "Please input your name: ";
  std::cin >> name;   // 从 stdin 输入
  std::cout << name;  // 输出到 stdout
  return 0;
}
// ------------------------------------------------------------

// 文件流 ------------------------------------------------------
std::ifstream word(string("./word.txt"));
vector<string> word_list;
/* 读入文件流; istream_iterator 迭代器以 空格/tab/换行 作为元素分割 */
copy(std::istream_iterator<string>(word) /* 流迭代器.begin() */,
     std::istream_iterator<string>() /* 流迭代器.end() */,
     back_inserter(word_list));
/* 标准输出流; ostream_iterator 迭代器可以指定分割符 */
copy(word_list.begin(), word_list.end(),
     std::ostream_iterator<string>(std::cout, "\n"));
// ------------------------------------------------------------