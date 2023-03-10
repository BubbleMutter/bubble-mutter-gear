/**
 * @file 排序算法练习模板
 */

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
using namespace std;

bool valid_check(vector<int> &a, vector<int> &b) {
    if (b.size() != a.size()) return false;
    for (int i = 0; i < a.size(); ++i)
        if (a[i] != b[i])
            return false;
    return true;
}

void build_array(vector<int> &a) {
    for (int j = 0; j < a.size(); ++j)
        a[j] = rand() % 200;
}

void print_array(vector<int> &a, const char *s) {
    printf("%10s", s);
    for (int j = 0; j < a.size(); ++j)
        printf("%5d ", a[j]);
    printf("\n");
}

class Sort {
private:
    /**
     * @exp: 从右往左 从一开始 的十进制位数
     */
    int findval(int a, int exp) {
        return (a / exp) % 10;
    }
    int findpivot(vector<int> &a, int head, int tail) {
        int mid = (tail - head) % 2 ?  (tail + head - 1)/2 : (tail + head) / 2;
        if ( (a[mid] - a[head])*(a[tail]-a[head]) < 0)
            return head;
        if ( (a[mid] - a[tail])*(a[head]-a[tail]) < 0)
            return tail;
        return mid;
    }
public:
    void sort(vector<int> &a) {
        // 1. 求值域 [min, max]
        // 2. 申请 偏移数组
        // 3. 统计出现次数
        // 4. 计算偏移
        // 5. 根据偏移存放到 临时结果数组
        // 6. 交换结果

        // 1.
        int mmin = *min_element(a.begin(), a.end());
        int mmax = *max_element(a.begin(), a.end());
        // 2.
        vector<int> count(mmax-mmin+1, 0);
        // 3.
        for (auto n: a)
            count[n-mmin]++;
        // 4.
        for (int i = 1; i < count.size(); i++)
            count[i] += count[i-1];
        // 5.
        vector<int> b(a.size(), 0);
        for (int i = a.size()-1; i >= 0; --i)
            b[--count[a[i]-mmin]] = a[i];
        // 6.
        copy(b.begin(), b.end(), a.begin());
    }
};

int main( int argc, char **argv ) {
    Sort sorter;
    for (int i = 1; i <= 1000; ++i) { // 检测 100 次
        // 1. 创建随机数组
        vector<int> a(i, 0);
        build_array(a);
        vector<int> b(a);

        // 2. 打印初始数组
        print_array(a, "before");

        // 3. 排序
        sorter.sort(a);

        // 4. 打印排序数组
        print_array(a, "after");

        // 5. 校验数组
        sort(b.begin(), b.end());

        // 6. 打印正确数组
        print_array(b, "answer");

        // 7. 校验结果
        printf("valid_check is %5s\n\n",valid_check(a, b) == true ? "true" : "false");
    }
    return 0;
}
