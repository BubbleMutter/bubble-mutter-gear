[TOC]
# hashes
1. 哈希表在任意 `H*SET*` 方法首次调用时, 自动创建
2. 当所有 field 删除后, 哈希表不会自动销毁, 变成 empty hash
## 示例
```redis
HSET myhash filed "symphony"
HGET myhash field1
HMSET myhash filed1 "system" filed2 "network" filed3 "languages"
HMGET myhash field1 field2 nofield

HSETNX myhash field "Hello"

HEXISTS myhash field1
HSTRLEN myhash field1
HDEL myhash field1

HLEN myhash
HKEYS myhash
HVALS myhash
HGETALL myhash

HINCRBY myhash field 1
HINCRBY myhash field -2

HINCRBYFLOAT mykey field 0.1
HINCRBYFLOAT mykey field -5.2
HINCRBYFLOAT mykey field 2.0e2
```
## 说明
HSET   赋值一个 field, 如果没有则创建
HGET 获取一个 field 的值; 存在则返回 其值, 否则返回 nil
HMSET  赋值多个 field, 如果没有则创建
HMGET  获取多个 field, 以数组形式返回, 存在则返回 其值, 否则返回 nil
HSETNX 赋值一个 field, 如果已经存在, 则不在替换 (形同 O_CREAT | O_EXCL)
HEXISTS 判断一个 field 是否存在; 存在则返回 1, 否则返回 0
HSTRLEN 获取字符串型 field 的字符串长度
HDEL 删除一个 field
HLEN 获取整个 hash 的 field 个数
HKEYS 获取整个 hash 的所有 field 名, 以数组形式返回, 否则返回 nil
HVALS 获取整个 hash 的所有 value 值, 以数组形式返回, 否则返回 nil
HGETALL 获取整个 hash; 以数组的形式返回, 奇数行指示 field 名, 偶数行指示 value 值
HINCRBY 累加数值型 field; 参数支持正数 和 负数, 参数必须是整型
HINCRBYFLOAT 累加数值型 field; 参数支持浮点数, 科学表示法
HSCAN
HRANDFIELD