/**
 * @file 给定纯数字的字符串, 分割成合法ip
 */
#include <iostream>
#include <vector>
#include "prettyprint.hpp"
using namespace std;

void string2ip(string &s, int curr_size, vector<int> & psize) {
    // 递归方法 (不要用全排列)
    // 1. 记录每个 part (ip 分为4个部分) 的长度
    // 2. 每个part 只能是 1,2,3
    // 3. 递归消耗字符串长度, 如果已消耗长度大于 总长度, 终止递归
    // 4. 递归消耗字符串长度, 刚好消耗完, 但不够4个part, 终止递归
    // 5. 满足条件, 构造4个ip地址的4个部分, 检查每个部分不大于255
    //    然后, 把4个满足条件的part合并为合法ip地址
    if (curr_size > s.size())
        return;
    if (curr_size == s.size()) {
        if (psize.size() != 4)
            return;
        string res;
        for (int i = 0, begin = 0; i < psize.size(); begin += psize[i++]) {
            string part = string(s.begin()+begin, s.begin()+begin+psize[i]);
            if (stoul(part) > 255)
                return;
            res += part + ".";
        }
        *(res.end()-1) = '\0'; /* 去掉末尾的 "." */
        cout << res << endl;
    }
    for (int i = 1; i <= 3; i++) {
        psize.push_back(i);
        string2ip(s, curr_size+i, psize);
        psize.pop_back();
    }
    return;
}

int main( int argc, char **argv ) {
    unsigned int value;
    vector<int> psize;
    string s = "11121314";
    string2ip(s, 0, psize);
    return 0;
}