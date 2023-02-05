[TOC]
# strncpy 不安全
`strncpy(destination, source, len);`
1. 当 source 大于 len
   拷贝 len 个字符到 destination 后
   没有终止符 `destination[len-1] = '\0';`
2. 需要进程额外添加终止符

# strstr 不安全
`char *strstr(const char *haystack, const char *needle);`
因为没有传入尺寸参数, 如果 haystack needle 越界, 无法感知