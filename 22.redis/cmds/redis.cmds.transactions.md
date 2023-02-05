[TOC]
# transactions
事务支持

## 示例
```redis
MULTI
WATCH mykey1 mykey2
/* multi commands */
EXEC

MULTI
WATCH mykey1 mykey2
/* multi commands */
DISCARD
```

## 说明
MULTI   事务开始
EXEC    提交事务
DISCARD 放弃提交
WATCH   检测key的变化
UNWATCH 不再检测 key 的变化; 实际上 DISCARD / EXEC 本身就会调用 UNWATCH