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

// array pointer issues about add and sub
void array_pointer_add_and_sub() {
    {
        printf("question %d\n", 1);
        char arr[] = {0, 1, 2, 3, 4};
        // 0, 0, 5
        printf("%d,%d,%d\n",  arr[0], strlen(arr), sizeof(arr));
        // 0 (0x100 只会取低8bit)
        printf("0x%d\n", arr[0] += 0x100);
    }
    {
        printf("question %d\n", 2);
        char arr[] = {0, 1, 2, 3, 4};

        /* p 始终指向 &arr[] 的末尾; 无论arr有多少成员
         * &arr 是一个指针; 指向一个 char 数组
         * &arr + 1 就是指针值加上 sizeof(arr)
         * 所以指向数组的末尾 */
        int *p = (int *)(&arr + 1);
        /* p 是 int 指针;
         * p - 1 就是指针值减去 sizeof(*p)
         * 所以即减去4 */
        printf("0x%08x\n", *(p - 1)); // 0x04030201
    }
    {
        printf("question %d\n", 3);
        char arr[] = {0, 1, 2, 3, 4};
        /* p 指向 &arr[1]
         * arr 是一个指针; 指向一个 char 成员
         * arr + 1 就是指针值加上 sizeof(*arr)
         * 即指向数组的第二成员 */
        int *p = (int *)(arr + 1);

        /* 栈非法访问;
         * p-1实际上是 p指针-4;
         * 求得数组前面两个 bytes 的指针 */
        printf("0x%08x\n", *(p - 1));
    }
    return 0;
}