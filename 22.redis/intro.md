# 数据类型
string, hash, list, set, zset
1. string: 增, 删, 改, 查
    1. 可以设置任意类型的二进制数据
    2. 可以直接把图片流放进去
2. hash: 增加 删除 遍历 增加成员 删除成员 设置成员 读取成员
3. list: 增加 删除 遍历 增加成员 删除成员 查询部分成员
4.  set: 增加 删除 遍历 增加成员 删除成员
5. zset: 增加 删除 遍历 增加成员 删除成员

# 

```redis
SET server:name "fido"
GET server:name
EXISTS server:name
```