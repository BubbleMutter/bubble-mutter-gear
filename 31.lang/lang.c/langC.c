// 数组指针的 区分
void diff_array_and_pointer() {
    char *a[100]; // 数组, 有100个成员, 每个成员是一个指针
    printf("%d\n", sizeof(a));  // 400
    printf("%d\n", sizeof(*a)); // 4

    char (*b)[100] = NULL; // 指针, 指向一个有100个成员的数组, 启每个成员大小是1
    printf("%d\n", sizeof(b));  // 4
    printf("%d\n", sizeof(*b)); // 100
    printf("%d\n", sizeof(**b)); // 1

    char c[100]; // 数组, 有100个成员, 每个成员是char
    printf("%d\n", sizeof(c));  // 100
    printf("%d\n", sizeof(*c)); // 1
}
// -------------------