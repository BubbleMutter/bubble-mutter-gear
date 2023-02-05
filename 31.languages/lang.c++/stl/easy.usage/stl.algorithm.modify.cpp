#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <prettyprint.hpp>
using namespace std;

static void usage_main();
int main( int argc, char **argv ) {
    usage_main();
    return 0;
}

static void usage_fill() {
    {
        // fill    用指定值; 填充容器; memset 的stl版本
        vector<int> x(10);
        fill(x.begin()+x.size()/2, x.end(), 2048);
        // cout << x << endl; // [0, 0, 0, 0, 0, 2048, 2048, 2048, 2048, 2048]
    }
    {
        // fill_n 用指定值; 填充容器; memset 的 stl版本
        vector<int> x(10);
        fill_n(x.begin(), x.size()/2, 2048);
        // cout << x << endl; // [2048, 2048, 2048, 2048, 2048, 0, 0, 0, 0, 0]
    }
}

static void usage_generate() {
    {
        // generate; 通过"生成器"生成元素值 填充元素
        random_device rd;
        vector<int> x(5);
        generate(x.begin(), x.end(), [&](void) { return -50 + (rd() % 100); } );
        // cout << x << endl;
    }
    {
        // generate_n; 通过"生成器"生成元素值 填充元素; 指定个数
        random_device rd;
        vector<int> x(5);
        generate_n(x.begin(), 5, [&](void) { return -50 + (rd() % 100); } );
        // cout << x << endl;
    }
}

static void usage_transform() {
    vector<int> a(3, 0); a[0] = 1; a[1] = 2; a[2] = 3;
    vector<int> b(3, 0); b[0] = 10; b[1] = 20; b[2] = 30;
    vector<int> c;
    // transform; 变换;
    //     a) 把一个container变换为另一个container
    //     b) 把两个container变换为另一个container

    // 单输入源; 输出指向输入; 形同 {a} += 1;
    transform(a.begin(), a.end(), a.begin(), [](int n) { return n+1; });
    // cout << a << endl; // [2, 3, 4]

    // 双输入源; 输出指向输入; 形同 {a} += {b};
    transform(a.begin(), a.end(), b.begin()/* 第二输入 */,
              a.begin() /* 输出 */, [](int x, int y) { return x+y; });
    // cout << a << endl; // [12, 23, 34]

    // 单输入源; 输出到动态增长的容器 {c} = {a} + 2;
    transform(a.begin(), a.end(), back_inserter(c),
              [](int n) { return n+2; });
    // cout << c << endl; // [14, 25, 36]

    c.clear();
    // 双输入源; 输出指向输入; 形同 {c} = {a} + {b};
    transform(a.begin(), a.end(), b.begin()/* 第二输入 */,
            back_inserter(a) /* 输出 */, [](int x, int y) { return x+y; });
}

static void usage_shuffle() {
    // random_shuffle 重排容器
    vector<int> x{1, 2, 3, 4, 5, 6, 7};
    random_shuffle(x.begin(), x.end());
    // cout << x << endl;
    // 默认是完美洗牌 结果固定 [5, 4, 2, 7, 1, 6, 3]
}

static void usage_copy() {
    vector<int> x{ 1, 2, 3, 4, 5 };
    {
        // copy
        // 单输入; 单输出
        // container 拷贝; 用iterator 指定结尾
        vector<int> y;
        copy(x.begin(), x.end(), back_inserter(y));
        // cout << y << endl;
    }
    {
        // copy_n
        // 单输入; 单输出
        // container 拷贝; 指定 拷贝个数
        vector<int> y;
        copy_n(x.begin(), 3, back_inserter(y));
        // cout << y << endl;
    }
    {
        // copy_if 拷贝满足条件的元素
        // 单输入; 单输出
        vector<int> y;
        copy_if(x.begin(), x.end(), back_inserter(y),
                [](int n) { return n & 1; } /* odd */);
    }
    {
        // copy_backward: 复制到容器的末尾; 覆盖中间部分
        // 单输入; 单输出
        // 把 [first, last) 拷贝到
        //    [result-1-distance(first, last), result-1)
        vector<int> y(x); // y == [1,2,3,4,5]
        y.resize(y.size()+3);
        copy_backward(y.begin(), y.begin()+5, y.end());
        // [1, 2, 3, 1, 2, 3, 4, 5]
    }
}

static void usage_move() {
    {
        // move 避免隐式拷贝; 干掉了耗时操作; 优化性能
        // move    : 创新新的对象;
        //           复制容器内存指针
        //           清理原对象的内存指针; 计数器等
        // 隐式拷贝: 创新新的对象;
        //           内存分配容器空间 (耗时操作)
        //           然后逐个拷贝元素 (数据量大的时候耗时)
        string x("hello");
        string y("world");
        vector<string> z;
        z.push_back(move(x)); // for now; x is clear
        z.push_back(y);       // for now; y is still valid

        // cout << z << endl; // ["hello", "world"]
        // cout << x << endl; // empty
    }
    {
        // move 一个 container
        // 把 [first, last-1] 从右往左 move 到 (-ee , result-1]
        vector<string> x{ "violin", "piano", "cello" }, y(3);
        move_backward(x.begin(), x.end(), y.end());

        // cout << y << endl; // y = [violin, piano, cello]
        // cout << x << endl; // x = [ , , ] 3个空字符串成员
        // Note: 这里不能用 front_inserter(y);
        //       front_inserter() 返回的迭代器只有 operator++ 方法
        //       虽然底层是 头插法
    }
}

static void usage_swap() {
    {
        // swap_ranges 交换两个container;
        vector<string> x { "violin", "piano", "cello" };
        vector<string> y { "symphony", "sonata", "concerto" };
        swap_ranges(x.begin(), x.end(), y.begin());
        // cout << x << endl; // [ "symphony", "sonata", "concerto" ]
        // cout << y << endl; // [ "violin", "piano", "cello" ]
    }
    {
        // iter_swap 交换两个iter_swap ;(交换索引swap 接口的变体)
        string x("abc");
        string y("xyz");
        iter_swap(x.begin(), y.begin());
        // cout << x << endl; // "xbc"
        // cout << y << endl; // "ayz"
    }
}

static void usage_replace() {
    /*
     * std::replace
     * std::replace_if
     * std::replace_copy
     * std::regex_replace
     * std::replace_copy_if
     */
    {
        // replace 把一个容器中的部分元素的值替换
        vector<int> x{ 1, 2, 3, 1, 2, 3, 1, 2, 3 };
        replace(x.begin(), x.end(), 1, 10);
        // cout << x << endl; // [ 10, 2, 3, 10, 2, 3, 10, 2, 3 ]
    }
    {
        // replace_if 把一个容器中的符合条件的元素的值替换
        vector<int> x{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        replace_if(x.begin(), x.end(), [](int x) { return x & 1; }, 0);
        // cout << x << endl; // [0, 2, 0, 4, 0, 6, 0, 8, 0]
    }
    {
        // replace_copy 把一个容器中的部分元素的值替换到另一个容器
        //              但不修改原本数组
        vector<int> x{ 1, 2, 3, 1, 2, 3, 1, 2, 3 };
        vector<int> y;
        replace_copy(x.begin(), x.end(), back_inserter(y), 1, 10);
        // cout << x << endl; // [1, 2, 3, 1, 2, 3, 1, 2, 3]
        // cout << y << endl; // [ 10, 2, 3, 10, 2, 3, 10, 2, 3 ]
    }
    {
        // replace_copy_if 把一个容器中的符合条件的元素的值替换到另一个容器
        //                 但不修改原本数组
        vector<int> x{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        vector<int> y;
        replace_copy_if(x.begin(), x.end(), back_inserter(y), [](int x) { return x & 1; }, 0);
        // cout << x << endl; // [1, 2, 3, 4, 5, 6, 7, 8, 9]
        // cout << y << endl; // [0, 2, 0, 4, 0, 6, 0, 8, 0]
    }
}

static void usage_remove() {
    {
        // remove 删除元素; 返回新的 end; 容器原尺寸不变; 一般配合 resize  用
        // 容器新的end 后方元素不确定; 不一定是 匹配的元素
        //
        // 00027. Remove Element.cpp 的实现
        vector<int> x{ 10, 20, 30, 10, 20, 30, 10, 20, 30 };
        auto stop = remove(x.begin(), x.end(), 10);
        // cout << x.size() << endl; // 容器原尺寸不变
        x.resize(distance(x.begin(), stop));
        // cout << x << endl; // [20, 30, 20, 30, 20, 30]

        // remove 一般性用法
        vector<int> y{ 10, 20, 30, 10, 20, 30, 10, 20, 30 };
        y.resize(distance(y.begin(), remove(y.begin(), y.end(), 10)));
        // cout << y << endl; // [20, 30, 20, 30, 20, 30]
    }
    {
        // remove_if 干掉符合条件的元素
        vector<int> x { 1, 2, 3, 4, 5 };
        auto stop = remove_if(x.begin(), x.end(), [](int x) { return x & 1; });
        x.resize(distance(x.begin(), stop));
        // cout << x << endl; // [ 2, 4 ]
    }
    {
        // remove_copy 干掉指定值的元素; 把结果copy出来; 原container保留
        vector<int> x{ 10, 20, 30, 10, 20, 30, 10, 20, 30 };
        vector<int> y;
        remove_copy(x.begin(), x.end(), back_inserter(y), 10);
        // cout << x << endl; // [10, 20, 30, 10, 20, 30, 10, 20, 30]
        // cout << y << endl; // [20, 30, 20, 30, 20, 30]
    }
    {
        // remove_copy_if 干掉符合条件的元素; 把结果copy出来; 原container保留
        vector<int> x { 1, 2, 3, 4, 5 };
        vector<int> y;
        remove_copy_if(x.begin(), x.end(), back_inserter(y), [](int x) { return x & 1; });
        // cout << x << endl; // [ 1, 2, 3, 4, 5 ]
        // cout << y << endl; // [ 2, 4 ]
    }
}

void usage_unique() {
    {
        // unique 去重; 把重复元素放到容器后面; 返回新的 end; 通常与 resize配合
        // 容器新的end 后方元素不确定; 不一定是 重复的元素
        // 00026. Remove Duplicates From Sorted Array.cpp 的实现
        vector<int> x { 1, 1, 1, 2, 2, 3, 4 };
        x.resize(distance(x.begin(), unique(x.begin(), x.end())));
        // cout << x << endl; // [ 1, 2, 3, 4]
    }
    {
        // unique_copy 把去重结果返回到新的容器中; 原容器不变
        vector<int> x { 1, 1, 1, 2, 2, 3, 4 };
        vector<int> y;
        unique_copy(x.begin(), x.end(), back_inserter(y));
        // cout << x << endl; // [ 1, 1, 1, 2, 2, 3, 4 ]
        // cout << y << endl; // [ 1, 2, 3, 4 ]
    }
}

void usage_reverse() {
    {
        // reverse 翻转容器
        vector<int> x {1, 2, 3, 4, 5};
        reverse(x.begin(), x.end());
        // cout << x << endl; // [5, 4, 3, 2, 1]
    }
    {
        // reverse_copy 翻转容器; 把结果输出到另一个容器; 保留原容器
        vector<int> x {1, 2, 3, 4, 5};
        vector<int> y;
        reverse_copy(x.begin(), x.end(), back_inserter(y));
        // cout << x << endl; // [1, 2, 3, 4, 5]
        // cout << y << endl; // [5, 4, 3, 2, 1]
    }
}

void usage_rotate() {
    {
        // rotate 旋转容器
        vector<int> x { 1, 2, 3, 4, 5, 6, 7, 8 };
        rotate(x.begin(), x.begin()+3, x.end());
        // cout << x << endl; // [4, 5, 6, 7, 8, 1, 2, 3 ]
    }
    {
        // rotate_copy 旋转容器; 把结果输出到另一个容器; 保留原容器
        vector<int> x { 1, 2, 3, 4, 5, 6, 7, 8 };
        vector<int> y;
        rotate_copy(x.begin(), x.begin()+3, x.end(), back_inserter(y));
        // cout << x << endl; // [1, 2, 3, 4, 5, 6, 7, 8 ]
        // cout << y << endl; // [4, 5, 6, 7, 8, 1, 2, 3 ]
    }
}

static void usage_main() {
    usage_fill();
    usage_generate();

    usage_transform();

    usage_shuffle();

    usage_copy();
    usage_move();
    usage_swap();

    usage_replace();
    usage_remove();
    usage_unique();
    usage_rotate();
    usage_reverse();
}
