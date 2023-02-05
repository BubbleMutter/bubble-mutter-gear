[TOC]
# string

# strncpy 不安全
`strncpy(destination, source, len);`
1. 当 source 大于 len, 拷贝到`destination[len-1]` 后, 没有终止符
2. 需要在后面额外添加终止符, `destination[len-1] = '\0';`

# strstr 不安全
+ `char *strstr(const char *haystack, const char *needle);`
+ 因为没有传入尺寸参数, 如果 haystack needle 越界, 无法感知

# numeric convert
```c++
// 字符串转数值
int atoi(const char *nptr);
long atol(const char *nptr);
long long atoll(const char *nptr);
double atof(const char *nptr);

long int strtol(const char *nptr, char **endptr, int base);
long long int strtoll(const char *nptr, char **endptr, int base);
unsigned long int strtoul(const char *nptr, char **endptr, int base);
unsigned long long int strtoull(const char *nptr, char **endptr, int base);
double strtod(const char *nptr, char **endptr);
float strtof(const char *nptr, char **endptr);
long double strtold(const char *nptr, char **endptr);

int scanf(const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int sscanf(const char *str, const char *format, ...);

// 数值转字符串
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int dprintf(int fd, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
```