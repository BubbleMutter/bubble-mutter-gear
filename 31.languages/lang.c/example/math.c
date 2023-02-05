// 整倍数 ----------------------------------------------
// 参考 linux/bitops.h,  BITS_TO_LONG
// 倍数向上取整
// 比如说: 给定申请的内存大小; 页大小4K; 求实际申请的内存
int power_ceil(int n, int power) {
    return ((n + power - 1) / power)*power;
}

// 倍数向上取整; 求倍数
// 比如说: 给定申请的内存大小, 求4K页数
int power_ceil_of_n(int n, int power) {
    return (n + power - 1) / power;
}

// 2 的指数向上取整; base-2 exponent's ceil
uint32_t exp2_ceil(uint32_t n) {
    int m = n, r = n;
    while (m) {
        r = m;
        m &= m-1;
    }
    return r == n ? r : r*2;
}
// 2 的指数向下取整; base-2 exponent's ceil
uint32_t exp2_floor(uint32_t n) {
    int m = n, r = n;
    while (m) {
        r = m;
        m &= m-1;
    }
    return r;
}
// ----------------------------------------------------