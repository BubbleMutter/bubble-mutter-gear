class People {
private:
    int age;
    int len;
public:
    People(int _age, int _len);
};

/* 构造函数初始化 列表 */
People::People(int _age, int _len): age(_age), len(_len) { }