[TOC]
# 1. 算术运算符 (5种)
```sql
-- 5种算术运算符 + - * / % 加减乘除 取模
select (4-3+1)*5 / 2 % 3;
```
1. 算术运算符返回数值

# 2. 比较运算符
```sql
select 1 = 1;  -- 等于
select 1 < 2;  -- 小于
select 2 > 1;  -- 大于
select 1 <= 2; -- 不大于
select 2 >= 1; -- 不小于
select 1 <> 2; -- 不等于
select 1 != 2; -- 不等于
select 5 between 1 and 10; -- 在左右闭合的区间中
select 5 in (1,2,3,4,5,6); -- 在给定的集合中
select 5 not in (1,2,3,4); -- 不在给定集合中
select null is NULL;       -- 空值判断
select 'a' is NOT NULL;    -- 非空判断
select 'str' LIKE '1%3_4[a-d][^e-h]'; -- 模糊匹配
select 'str' REGEXP 's.*r'; -- 正则匹配(支持完整的正则式)
```
1. 比较运算符返回 1 或者 0

# 3. 逻辑运算符
```sql
select 2 and 1; select 2 && 1; -- 与
select 2 or  1; select 2 || 1; -- 或
select not 1;   select !1;     -- 否
select 1 xor 1; select 1^1;    -- 异或
```
1. 逻辑运算符返回 1 或者 0

# 4. 位运算符
```sql
select 1 & 2;  -- 按位与
select 1 | 2;  -- 按位或
select 1 ^ 2;  -- 按位异或
select ~1;     -- 按位取反
select 2 >> 1; -- 按位右移
select 2 << 1; -- 按位左移
```