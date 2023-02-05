[TOC]
# 1. 列独立
1. 特性 = 索引的列在 条件表达中, 必须单独一侧, 否则将会全表查询
2. 例子
```sql
-- where 子语句中, 等号左边有表达式操作, 将会导致全表查询
select * from tbl where id+1=123; -- 全表查询, 可以通过 explain 确认
select * from tbl where id=123+1; -- 索引查询

-- where 子语句中, 等号左边有函数操作, 将会导致全表查询
select * from tbl where substring(sname, 1, 3)='abc'; -- 全表查询
select * from tbl where sname like 'abc%'; -- 索引查询
```

# 2. LIKE 左对齐
1. 特性 = 匹配模式必须要左边确定, 不能以通配符开头. 否则无效
2. 例子
```sql
select * from tbl where name like 'abc%'; -- 索引有效, 可以通过 explain 确认
select * from tbl where name like 'ab%c'; -- 索引有效, 可以通过 explain 确认
select * from tbl where name like '%abc'; -- 索引无效, 可以通过 explain 确认
select * from tbl where name like '_abc'; -- 索引无效, 可以通过 explain 确认
```

# 3. 局部变量问题
1. 特性 = where 子语句使用局部变量 导致 全表查询
2. 例子
```sql
select id from tbl where id=@id; -- 全表查询
select id from tbl with(index($索引名)) where id=$id; -- 索引查询
```
3. note: 局部变量在 SQL运行时才能解析; 查询计划在编译时进行索引选择; 因此查询计划无法选择索引
4. note: 这种场景下, 指定索引, 最终可以进行索引查询
5. warn: 不要误以为指定索引就能  必然索引查询; 其他条件不满足的情况下也是使用全表查询

# 4. 不等于运算
```sql
select * from tbl where val != 10;   -- != 或 <> 导致全表查询
```

# 5. null 值判断
```sql
select * from tbl where val is null;
```
1. null 值判断导致全表查询
2. 定义时应给出默认值, 用以指示没有数据的情况, 如 -1 或者 0

# 6. "in" 和 "not in"
```sql
-- "in" "not in" 导致全表查询
select * from tbl where id in (1, 3, 5);
-- 以下场景可以使用 exists 替代 in
select * from tblX where id in (select id from tblY); -- 全表查询
select * from tblX where exists (select 1 from tblY where id=tblX.id); -- 索引查询
```

# 7. 逻辑运算 ( OR 和 AND )
1. 特性 = 多列索引, 其匹配条件使用 `AND` 和 `OR` 组合, 需要保证 左右两边分别 索引有效
2. `AND` 如果仅仅左边索引有效, 也是可以的; 但是右边索引无效也会导致 顺序查询
3. union 替代 or
```sql
-- 可以使用 union 对没有复合索引的多列进行分别使用索引
-- 这需要单独的列本身有添加索引 or 主键
select * from tbl where id = 10 or age = 100; -- 没有创建 (id, age) 索引, 全表查询
select * from tbl where id = 10 union all
select * from tbl where age = 100; -- id, age 分别有索引, 两列索引查询再合并
```

# 8. AND 运算 (左前缀原则)
1. 定义 = `AND` 的左右两边中 如果只有一边有索引, 那么需要把带索引的放到左边, 否则无效
    + 这是针对 `AND` 两边没有复合索引的场景

# 9. 字符串-前缀索引
```sql
ALTER table $表名称 ADD INDEX `index_sname` (`sname`(1));
select * from $表名称 where sname = 'a%';
```
1. 字符串较长时, 应该使用短索引
    1. 因为索引包含列本身的内容, 使用完全索引将消耗磁盘空间
    2. 长索引的比较查找也会消耗较多的性能
2. note: 前缀索引不能触发 索引覆盖

## 9.1 前缀长度的选取
1. 策略 = 在长度短 和 辨析度高 两者间平衡
2. 理论 = 可以使用前缀索引的字符串, 满足特性, 当 前缀不断增长时, 辨析度将不再提高.
    1. 如果 前缀不断增长时, 直到全长度, 辨析度依旧可以上涨, 即不适合使用前缀索引
3. 方式 = 用样本数据建立 `辨析度 = f(前缀长度)` 的函数, 选取"接近平台"的较少"前缀长度"
4. 辨析度求算
    ```sql
    select (select count(*) from tbl) / (count(distinct substring(sname, 1, $前缀长度))) from tbl;
    ```

# 10. 字符串-全文索引 (长文本索引) ( fulltext )
1. 背景 = 解决 `%abc...` `_abc...` 的需求问题
2. 本质 = 当字符串中存在 非字母,数字的字符 分割成一个一个词时, 全文索引为每个文本词建立索引
3. 例子 = 查找文章内容 或 文章标题带有 kernel 的文章
    ```sql
    alter table articles add fulltext index  `title_content` (`title`, `content`);
    select * from articles where match(title, content) against(`kernel`); -- match against 用于全文索引的特殊函数
    select title, match(title, content) against(`kernel`) as hot from articles; -- match against 可以直接使用, 求关联度
    ```
4. warn: ?mysql 的 fulltext 不支持中文? (2021/07/27 待确认)
5. note: 全文索引支持搜索的文本词通过字典支持的,
         不是所有单词都能生效, 即存在无法搜出来的文本词,
         但是这些文本词, 实际存在于列的值中, 比如单词 `in`

# 11. 索引弃用
1. 即使理论 索引有效, 但是 mysql 可能选择不用, 即 "索引弃用"
2. 索引的底层数据结构是树状, 使用索引查询, 会导致磁盘随机 IO,
   而顺序查询是线性IO, 极端情况下, 会出现以下场景,
   "(随机IO - 线性IO)的耗时" > "(比较所有行 - 只比较索引路径)的耗时"
3. 这时 mysql 就会使用全表查询
4. 2021/07/27 ? mysql 在没有开始查询时是如何预判耗时的 ?

# 12. 索引覆盖
1. 索引覆盖 = 索引区拥有列本身的内容, 如果可能, 直接作为查询数据, 就不需要在数据区获取数据
2. 索引覆盖 的 触发条件
    1. 无 where 时, select 的 列组合 存在完全对应的 索引
    2. 有 where 时, select 的 列组合 和 条件列 存在完全对应的 索引, 且 索引有效
3. note: `explain` 的 `Extra` 会显示 `Using index`

# 13. 索引过多
1. 索引列的数值差异较少, 索引查询冲突越大, 将会触发智能弃用(即使没触发也会降低性能)
   此时, 增删数据还会继续额外消耗时间 维护索引本身, 最终使得整体性能降低
   所以, 数值差异较少的列不应该使用索引
2. 基于第一点, 针对业务需要针对足够频繁的 where 条件才选择使用索引, 避免索引过多
3. 基于第一点, 使用 `UNIQUE` 或 `PRIMARY KEY` 更优, 因为重复值越少性能越高
4. 长字符串较长时应该选用前缀索引, 避免索引过多, 但这需要确定前缀列有足够的"辨析度",

# 51. explain 执行计划的详细分析 (主要用于 调试索引)
## 51.1 查询
```sql
explain select * from tbl where id = 123\G          -- 查询
```
### 查询 (无索引) 的 explain 结果
```s
id           : 计划id
select_type  : SIMPLE  # 还有哪些
table        : tbl     # 表名称
type         : ALL     # 全表查询
possible_keys: NULL    # 可能使用的索引
key          : NULL    # 真正使用的索引
key_len      : NULL    # 索引的长度
ref          : NULL
rows         : 1000000 # 全表行数
Extra        : Using where
```
### 查询 (有索引) 的 explain 结果
```s
id           : 计划id
select_type  : SIMPLE  # 还有哪些
table        : tbl     # 表名称
type         : ref     # 索引查询
possible_keys: tbl_idx # 可能使用的索引 (命名误解, 用以区分 查询/排序)
key          : tbl_idx # 真正使用的索引
key_len      : 4       # 索引的长度
ref          : const
rows         : 1       # 需要查询的行数
Extra        : Using where
```
## 51.2 排序
```sql
explain select * from tbl order by sname limit 50\G  -- 排序
```
### 排序 (无索引) 的 explain 结果
```s
id           : 计划id
select_type  : SIMPLE  # 还有哪些
table        : tbl     # 表名称
type         : ref     # 索引查询
possible_keys: tbl_idx # 可能使用的索引 (命名误解, 用以区分 查询/排序)
key          : tbl_idx # 真正使用的索引
key_len      : 4       # 索引的长度
ref          : const
rows         : 1       # 需要查询的行数
Extra        : Using filesort #
```

### 排序 (有索引) 的 explain 结果
```s
id           : 计划id
select_type  : SIMPLE  # 还有哪些
table        : tbl     # 表名称
type         : index   # 索引查询
possible_keys: NULL    # 可能使用的索引 (命名误解, 排序不会检索索引)
key          : tbl_idx # 真正使用的索引
key_len      : 25      # 索引的长度     (sname 列的字符串长度)
ref          : NULL
rows         : 50      # 需要查询的行数  (limit 限制)
Extra        :
```

## 51.3 索引覆盖 的 explain 结果
```sql
explain select sname from tbl\G -- sname 之前已经创建了索引
explain select sname, id from tbl where sname = 'xxxx' OR id < 100\G -- sname 和 id 使用了复合索引
```
2. 结果
```s
id           : 计划id
select_type  : SIMPLE
table        : tbl
type         : index   # 索引查询
possible_keys: NULL
key          : xxxx     # 真正使用的索引
key_len      : 25   # 索引的长度     (sname 列的字符串长度)
ref          : NULL
rows         : 1000000 # 全表行数
Extra        : Using index
```