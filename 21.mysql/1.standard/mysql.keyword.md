[toc]
# 1. database { create drop use }
```sql
create database $库名;           -- 创建
drop database if exists $库名;   -- 删除
use $库名;                       -- 选择
```

# 2. table { create drop alert }
```sql
-- 创建表
create table if not exists $表名称 (
   `id` integer unsigned auto_increment, -- 自动递增
   `name` varchar(100) not null,
   `region` varchar(40) not null,
   `attributes` date,
   primary key ( `id` ) -- 指示主键
) engine=innodb default auto_increment=100 charset=utf8;

-- 删除表
drop table if exists $表名称;

-- 修改表名
alter table $表名称 rename to $表名称;

-- 新增一列到末尾
alter table $表名称 add $列名称 int $属性列表;
-- 新增一列到开头
alter table $表名称 add $列名称 int $属性列表 first;
-- 新增一列到中间
alter table $表名称 add $列名称 int $属性列表 after $列名称;

-- 修改一列的属性
alter table $表名称 modify $列名称 int $属性列表;
-- 修改一列的名称
alter table $表名称 change $列名称 $数据列新名称 int $属性列表;
```
1. `auto_increment` 标记为自动递增; `auto_increment=100` 递增基数
2. `not null`       非空属性; 即在非空时将会报错
3. alter 修改列后的默认值是 null; 除非 `$属性列表` 中声明 `not null default $默认值`

## 2.1 表属性
1. `engine=innodb`      = 数据库引擎
2. `auto_intrement=100` = 自动递增的基准值
3. `charset=utf8`       = 字符集

## 2.2 列属性
无符号(unsigned)和填充零(zerofill)，还有是否为空、默认值、主键、自动编号。
1. `unsigned`       =
2. `zerofill`       =
3. `not null`       = 
4. `deffault`       = 给默认值, 建议都给默认值, 不要用 null, null 不能使用索引查询
6. `auto_increment` = 自动递增

## 2.2 temporary 临时表
```sql
create temporary table $临时表名 ( $列定义 );
drop table if exists $临时表名; -- 临时表的主动销毁方式与普通表一样
```
1. 临时表只在当前连接可见，当关闭连接时, 表将会被自动销毁
2. 但是为了程序健壮性, 应该手动删除

# 3. data { insert select delete update }
```sql
-- 插入 多行
insert into $表名称 ( col1, col2, ..., coln ) values ( v1, v2, ..., vn ), ..., ( v1, v2, ..., vn );

-- 更新 表达式
update $表名称 set field1=value1, field2=value2 where $条件;

-- 删除 表达式
delete from $表名称 where $条件;

-- 查询 表达式
select $列名称, $列名称 from $表名称 where $条件 limit n offset m;
```

# 4. where
## 4.1 compare operator
```sql
-- 条件 表达式
select * from $表名称 where $列名称 = $数据值;
-- 条件 比较
select * from $表名称 where $列名称 = $数据值 or $列名称 >= $数据值;
select * from $表名称 where $列名称 < $数据值 and $列名称 < $数据值;

-- 条件 字符串区分大小写; 数据列是字符串类型 数据值是字符串
select * from $表名称 where binary($列名称) = $数据值;
-- 条件 字符串不区分大小写; 数据列是字符串类型 数据值是字符串
select * from $表名称 where $列名称 = $数据值;
```

## 4.2 "is null" "is not null"
```sql
-- 条件 空列判断
select * from $表名称 where $列名称 is null;
-- 条件 非空列判断
select * from $表名称 where $列名称 is not null;
```
1. null 不允许使用 `!=` `=` 判断
2. null 只能使用 `is null` `is not null` 判断

## 4.3 "like"
```sql
-- 条件 通配符 % 指示任意个任意字符, 形同 正则式 *
select * from $表名称 where col like $列名称 = 'abcd%';
-- 条件 通配符 _ 指示一个任意字符, 形同 正则式 .
select * from $表名称 where col like $列名称 = 'abcd_';
-- 条件 通配符 [] 指示范围中的一个字符
select * from $表名称 where col like $列名称 = 'abcd[0-9]';
-- 条件 通配符 [^] 指示范围中的所有字符以外的一个字符
select * from $表名称 where col like $列名称 = 'abcd[^0-9]';
-- 条件 通配符 通配符本身如何查询
```

# 5. select
## 5.1 order by
```sql
select ... order by 列名称 asc, 列名称 desc, ...
```
1. `order by` 对查询结果进行排序
2. asc, 升序, 是默认的排序方式, 指定列名称后 asc 可写可不写
3. desc, 降序排列
4. 可以补充多个列的排序方式, 在前一个排序列结果相同后, 将使用下一个排序列的排序方式
## 5.2 union
```sql
-- 结果 求并集 (不同表中 相同列中 的数据) (去重）
$查询语句 union $查询语句 order by 列名称;
-- 结果 求并集 (不同表中 相同列中 的数据) (包括重复)
$查询语句 union all $查询语句 order by 列名称;
```
1. `union` 前后查询语句 中 各自的列名称序列 完全相同
2. `union` 前后支持完整的查询语句, 包括 条件语句where
3. `order by` 不是必要的; 是为了提高结果可读性

## 5.3 group by
```sql
-- 统计用户的来访记录次数
select username, count(*) as record_count from accessrecord group by username;
```
1. 函数操作后使用 as 重命名列, 不使用 as 则列名称默认是函数操作子语句, 如 `count(*)`

## 5.4 with rollup
```sql
-- 在分组统计数据基础上再进行统计, 即最后多出一行表示总数,
select coalesce(name, 'sum'), sum(signin) from accessrecord group by username with rollup;
```
1. `with rollup` 是在已有统计基础上, 再做一次统计生成 **额外统计行**
2. `coalesce` 指示经由 `with rollup` 生成的 **额外统计行** 的 name, 否则默认是 `null`

# 6. join
1. `left join` 和 `right join` 功能和性能上没有区别, 可以完全替换, 为了完善语义设计出来
## 6.1 inner join
```sql
-- 求两张表的交集
select x.id, x.name, y.record from tblx as x, tbly as y where x.id = y.real_id;

-- 使用 inner join 表达
select x.id, x.name, y.record from tblx as x inner join tbly as y on x.id = y.real_id;
```
1. `inner join` 用以求两张表的交集
2. 交集 = 给定两张表两个同类型的列, 结果返回 在两张表列的值 相同的行
3. 交集列在两张表中的 列名称 可以不同
4. 表名称后续 跟着的字符串会被判定为是 alias

## 6.2 left join
```sql
select x.id, x.name, y.record from tblx as x left join tbly as y on x.id = y.real_id;
```
1. 返回"表名称1"的所有行 以及 与"表名称2"的交集行
2. 如果同时查询了"表名称1"和"表名称2"的列,
   对于只存在于"表名称1"的行,
   那么在结果中对应"表名称2"的列 上面的数据库为 null

## 6.3 right join
```sql
select x.id, x.name, y.record from tblx x right join tbly y on x.id = y.real_id;
```
1. 返回"表名称2"的所有行 以及 与"表名称1"的交集行
2. 如果同时查询了"表名称1"和"表名称2"的列,
   对于只存在于"表名称2"的行,
   那么在结果中对应"表名称1"的列 上面的数据库为 null

# 7. transaction
```sql
-- 用法示例 =======================
begin;    -- 开始一个事务
-- update insert delete 语句
rollback; -- 事务回滚
-- update insert delete 语句
commit;   -- 事务提交
-- ===============================

-- 存档点使用 ===================================
savepoint identifier;           -- 创建事务存档点
release savepoint identifier;   -- 删除事务存档点
rollback to identifier;         -- 回到事务存档点
-- =============================================

-- 隔离级别 =====================================
set transaction read uncommitted;
set transaction read committed;
set transaction repeatable read;
set transaction serializable;
-- ==============================================
```
1. 事务不支持 `create` 语句

# 8. index / primary key / fulltext
## 8.1 primary key
```sql
create table if not exists $表名称 (
   `id` integer unsigned auto_increment, -- 自动递增
   primary key ( `id` ) -- 指示主键
) engine=innodb default charset=utf8;
-- 添加一列的主键
alter table $表名称 add primary key($列名称);
-- 删除一列的主键
alter table $表名称 drop primary key;
```
1. 一个表只能有一个主键, 重复添加主键会失败

## 8.2 index { 创建 删除 查看 }
```sql
create index $索引名  on $表名称($行名称 列表);        -- 创建 普通索引 方法1 使用 create
alter table $表名称 add index $索引名($行名称 列表);   -- 创建 普通索引 方法2 使用 alter
create table $表名称 (
    username varchar(16) not null,
    index index_on_username(username(length))        -- 创建 普通索引 方法3 表创建时添加
);                                                  
create unique index $索引名 on $表名称($行名称 列表);  -- 创建 唯一索引 方法1 使用 create
alter table $表名称 add unique $索引名($行名称 列表);  -- 创建 唯一索引 方法2 使用 alter
create table $表名称 (
    rolename varchar(16) not null,
    unique index_on_rolename(rolename(length))       -- 创建 唯一索引 方法3 表创建时添加
);

drop index if exists $索引名 on $表名称;          -- 删除 一个索引 方法1
alter table $表名称 drop index $索引名; -- 删除 一个索引 方法2
drop index on $表名称; -- 删除 全部索引

show index from table_name; -- 查看索引信息
```
1. 普通索引，索引值可出现多次, 可以为 null
2. 唯一索引, 索引值只出现一次, 除了 null 外部

# 9. export import
```sql
-- 导出 csv 文件
select id,name,age into
    outfile '/path/to/sql'     -- 输出文件
    fields terminated by ','   -- 列终止符
    optionally enclosed by '"'
    lines terminated by '\n'   -- 行终止符
    from $表名称;

-- 导入 csv 文件
load data local 
    infile 'dump.txt' 
    into table $表名称
    fields terminated by ','
    lines terminated by '\n';

source /path/to/sql;
```

# 10. explain 显示执行计划 (主要用于 调试索引, 详见优化部分的索引)
```sql
explain select * from tbl where id = 123\g -- 示例用法, 在原有的 sql 查询命令前加 explain
```