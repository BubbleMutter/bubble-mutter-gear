// overload operator()
class CheckTicket {
 public:
  CheckTicket(const Region& region) : m_region(region) {}
  bool operator() (const Ticket& ticket);
 private:
  const Region& m_region;
};

void Usage(const Region& region, const vector<Ticket>& tickets) {
  // 生成可调用对象, 用于 algorithms 迭代器方法
  // 并且可调用对象, 可以携带既定参数, 如 Region 根据区域检查票据列表
  if (!all_of(tickets.cbegin(), tickets.cend(), CheckTicket(region))) {
    cerr << "tickets gets fake" << endl;
    throw std::runtime_error("tickets gets fake");
  }
}

// 使用 bind 写法
bool CheckTicket(const Region& region, const  Ticket& ticket);
void Usage(const Region& region, const vector<Ticket>& tickets) {
  if (!all_of(tickets.cbegin(), tickets.cend(), bind(CheckTicket, region, std::placeholders::_1))) {
    cerr << "tickets gets fake" << endl;
    throw std::runtime_error("tickets gets fake");
  }
}
