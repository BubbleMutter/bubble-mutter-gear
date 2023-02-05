[TOC]
# sets
1. 集合对象在 SADD 方法首次调用一个KEY上后, 自动创建

# 示例
```redis
SADD myset "Hello"
SCARD myset

SISMEMBER myset "Hello"
SMISMEMBER myset "member1" "member2" "memberN"
SMOVE source destination "member"
SPOP myset
SPOP myset 3
SRANDMEMBER myset 
SRANDMEMBER myset 3
SREM myset "member1" "member2" "memberN"

SDIFF myset subset1 subset2 subsetN
SDIFFSTORE destination myset subset1 subt2 subsetN
SINTER myset subset1 subset2 subsetN
SINTERSTORE destination myset subset1 subt2 subsetN
SUNION myset1 myset2 myset3
SUNIONSTORE destination myset subset1 subt2 subsetN
```

# 说明
SADD 添加一个 member
SCARD 获取集合 member 个数
SISMEMBER 判断一个 member 是否存在
SMEMBERS 获取集合 所有 member; 结果以数组形式返回 
SMISMEMBER 判断多个 member 是否存在; 结果以数组形式返回, 1表示存在
SMOVE 移动一个 member; 从第一参数 移到 第二参数, 第三参数指示 member
SPOP 随机删除并返回 一个(不带参数) 或 多个(带参数) member; 以数组形式返回
SRANDMEMBER 随机返回 一个(不带参数) 或 多个(带参数) member; 以数组形式返回
SREM 删除指定 member
SSCAN
SDIFF 获取集合 的补集; 只存在于首个集合, 不存在于后续所有集合的 member
SDIFFSTORE 功能同于SDIFF, 但把结果存到第一参数, 并返回 member 个数
SINTER 获取集合 的交集; 所有集合的交集
SINTERSTORE 功能同于SINTER, 但把结果存到第一参数, 并返回 member 个数
SUNION 取得集合 的并集; (结果也是集合,即元素单一化)
SUNIONSTORE 功能同于SUNION, 但把结果存到第一参数, 并返回 member 个数