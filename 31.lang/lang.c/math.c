// 整倍数 ----------------------------------------------
// 参考 linux/bitops.h,  BITS_TO_LONG
// 倍数向上取整
// 比如说: 给定申请的内存大小; 但是最后只能以页大小4K为单位;
//         求实际申请的内存
int power_ceil(int n, int power) {
    return ((n + power - 1) / power)*power;
}

// 倍数向上取整; 求倍数
// 比如说: 给定申请的内存大小; 但是最后只能以页大小4K为单位;
//        求页数
int power_ceil_of_n(int n, int power) {
    return (n + power - 1) / power;
}
// ----------------------------------------------------