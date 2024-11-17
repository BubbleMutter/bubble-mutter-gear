[TOC]
# Hash String
+ resource
    + https://oi-wiki.org/string/hash/
    + https://cp-algorithms.com/string/string-hashing.html
    + https://byvoid.com/zht/blog/string-hash-compare/
1. 把字符串映射到整数
2. 映射后方便判断两字符串是否相等
    1. 即 hash值 不一样时 字符串一定不相同
    2. 但字符串不相同时, hash 值可以一样 (冲突)

# Hash String Example 1
```c++
const int M = 1e9 + 7;
const int B = 233;
int get_hash(const string& s) {
    int res = 0;
    for (int i = 0; i < s.size(); ++i) {
        res = (long long)(res * B + s[i]) % M;
    }
    return res;
}
// 返回后还需要根据桶取模
```
1. (每一个字符 + base * 乘上个字符的摸) 然后取模
    + 目的: hash值与字符所在的位置有关
2. Base 与 Module 必须互质; 否则槽位 Module 上分布概率不均匀
    + 数学原理不是很理解
3. 一般地, Module 会选择较大的质数
