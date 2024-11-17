# Q: 计算两个数的平均值; 如何防止溢出?
```c++
unsigned average(unsigned x, unsigned y) {
    return x/2 + y/2 + ((x & 1) && (y & 1));
}
```
1. 分别除以2再相加; 如果都是奇数, 最后再补上1