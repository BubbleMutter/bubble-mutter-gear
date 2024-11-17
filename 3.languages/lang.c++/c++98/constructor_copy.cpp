/** 
 * @file 拷贝构造函数
 * @note 在创建对象时，是使用同一类中之前创建的对象来初始化新创建的对象。 
 */
class People {
private:
    int age;
    int len;
public:
    People(int _age, int _len) : age(_age), len(_len) { }
    People(const People &people); /* 拷贝构造函数a) */
    People(const People *people); /* 拷贝构造函数b) */
};

/* 拷贝构造函数a) */
People::People(const People & people) {
    this->age = people.age;
    this->len = people.len;
}

/* 拷贝构造函数b) */
People::People(const People *people) {
    this->age = people->age;
    this->len = people->len;
}

void usage() {

}

#if 0
拷贝函数: 用同一类中之前创建的对象来初始化新创建的对象
用途:
    1. 通过使用另一个同类型的对象来初始化新创建的对象
    2. 复制对象作为函数的参数
    3. 复制对象作为函数的返回
#endif