/* size_t 用 %zu 打印 */
printf("%zu\n", sizeof(int));

printf("%02hhx\n", value_uint8_t);   /* uint8_t */
printf("%04hx\n", value_uint16_t);   /* uint16_t */
printf("%08x\n", value_uint32_t);    /* uint32_t */
printf("%016jx\n", value_uintmax_t); /* uintmax_t */

/* uint64_t */
uint64_t u = 0x1122334455667788;
int64_t  i = 0x8877665544332211;
printf("%"PRIu64"\n", u);
printf("%"PRId64"\n", i);
printf("%"PRIx64"\n", i);
printf("%"PRIx64"\n", u);
// Note: uint64_t 并不等于 uintmax_t
//       不该用 %jx 转换 uint64_t/int64_t

/* time_t */
printf("%lu\n", (unsigned long)time(NULL));
// Note: time_t 不是标准类型, 建议 cast;
//       time_t 可能是 uint32_t 或者 uint64_t