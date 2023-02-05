```c++
long  f1() { printf("f1\n"); return 0; }
int   f2() { printf("f2\n"); return 0; }
short f3() { printf("f3\n"); return 0; }

#define printf_sizeof(expr) \
        printf("sizeof(" #expr ") is %zd\n", sizeof(expr));
int main(void) {
    printf_sizeof(f1);   // 1 2021/05/06 未知?
    printf_sizeof(f2);   // 1 2021/05/06 未知?
    printf_sizeof(f3);   // 1 2021/05/06 未知?

    printf_sizeof(&f1);  // 8 函数指针大小
    printf_sizeof(&f2);  // 8 函数指针大小
    printf_sizeof(&f3);  // 8 函数指针大小

    printf_sizeof(f1()); // 8 函数返回值大小
    printf_sizeof(f2()); // 4 函数返回值大小
    printf_sizeof(f3()); // 2 函数返回值大小

    return 0;
}
```