[TOC]
RDBMS(Relational Database Management System：关系数据库管理系统

Q: 创建表格时的 a) 列的属性; b) 表的属性 完整列表
Q: drop table; drop database 怎么写存在检查?
Q: 关键字的执行顺序 ??
Q: 条件 通配符 通配符本身如何查询?
Q: 删除 数据列 数据库 数据表 如何提前判断?
Q: 需要两种类型的 sql 语句笔记, a) 完整的语法表达式 b) 实际用到的例子 ?
Q: 表间查询 跟 表内查询 性能区别在哪里? 需要看实现源码
Q: 事务保存点实际上有什么作用?
Q: 什么是全文索引? 有什么用?
Q: 唯一索引的列的值的唯一性如何保证?
Q: FUNCTION 是什么玩意
Q: 非 PRIMARY KEY 可以递增吗?
Q: 递增步进怎么设置?
Q: PRIMARY KEY 使用 ALTER 添加时如何设置递增基数
Q: sql 注入问题
Q: 导入导出分为 sql 和 csv(csv 特例,实际上是纯文本数据)
Q: 了解 sql 函数
Q: COLLATE WHEN THEN ELSE CASE BINARY 是什么东西
Q: 匹配模式 vs 正则模式
A: `PRIMARY KEY` 是一种特殊索引, 能提高查询效率
A：index 是在 where 中发挥作用, 多列 index 在 where 使用 and or 时生效
A: 数据库优化点应该归纳为 checklist 才对, 原理说明是一回事, 但是实践时需要 checklist
Q: 通配符本身如何查询?

SELECT 子句顺序 = `FROM > WHERE > GROUP BY > HAVING > ORDER BY`

语法 > 经验 > 优化 > 实现 (4个层次)
经验包括书本例题, 网站习题, 项目经验
需要一个更好的压缩方法压缩语法知识
需要一个形同 man-pages cppman 的工具查询 sql 语法

# shell utils
导入 sql `mysql -u root -p123456 < runoob.sql`
## debian package
```sh
apt install default-mysql-server # server
apt install default-mysql-client # client
apt install mycli                # client with autocomplete
apt install mysqldump            # 导出工具
apt install mysqlimport          # 导入工具
```

## mysqldump
```sh
# 导出 单个数据表的 sql
mysqldump -u root -p $数据库 $表名称 > "/path/to/sql"
# 导出 单个数据库的 sql
mysqldump -u root -p $数据库 > "/path/to/sql"
# 导出 所有数据库的 sql
mysqldump -u root -p --all-databases > "/path/to/sql"
# 导出 远程数据表的 sql (实际上可以导出整个数据库 or 所有数据库)
mysqldump -h other-host.com -P $port -u root -p $数据库 $表名称 > "/path/to/sql"
```

## mysqlimport
```sh
# 导入 
mysqlimport -u root -p --local mytbl "/path/to/sql"
# 导入 csv
mysqlimport -u root -p --local \
    --fields-terminated-by="," \
    --lines-terminated-by="\n" \
    --columns=b,c,a \
    $表名称 "/path/to/sql"
# --fields-terminated-by   指定列终止符
# --lines-terminated-by    指定行终止符
# --columns=b,c,a          变更列的顺序
```

1. 在 MySQL 中只有使用了 Innodb 数据库引擎的数据库或表才支持事务。
3. 事务用来管理 insert,update,delete 语句

事务性(ACID):
1. 原子性 (Atomicity):
    1. 一个事务中的所有操作, 要么全部完成, 要么全部不完成, 不会结束在中间某个环节.
    2. 事务在执行过程中发生错误, 将会被回滚(Rollback)到事务开始前的状态.
2. 一致性 (Consistency):
    1. 在事务开始之前和事务结束以后，数据库的完整性没有被破坏
    2. 写入的资料必须完全符合所有的预设规则
    3. 包含精确度, 串联性以及后续数据库可以自发性地完成预定的工作
3. 隔离性 (Isolation):
    1. 数据库允许多个并发事务同时对其数据进行访问和修改
    2. 隔离性可以防止多事务并发执行导致数据的不一致
    3. 事务隔离分为不同级别: 读未提交(Read uncommitted), 读提交(read committed), 可重复读(repeatable read) 和 串行化(Serializable)
4. 持久性 (Durability):
    1. 事务处理结束后，对数据的修改就是永久的 即便系统故障也不会丢失

文件详解
```sql
/var/lib/mysql/$库名称/ibdata1       -- innodb 共享表的 数据 和 索引
/var/lib/mysql/$库名称/${表名称}.frm  -- 表结构文件，仅存储了表的结构、元数据(meta)，包括表结构定义信息等
/var/lib/mysql/$库名称/${表名称}.ibd  -- innodb 独立表的 数据 和 索引
/var/lib/mysql/$库名称/${表名称}.opt  -- innodb
/var/lib/mysql/$库名称/${表名称}.MYD  -- MyISAM 表数据文件
/var/lib/mysql/$库名称/${表名称}.MYI  -- MyISAM 表索引文件

/var/lib/mysql/$库名称/$表名称#P#p0.MYD -- 分区数据
/var/lib/mysql/$库名称/$表名称#P#p0.MYI -- 分区索引
/var/lib/mysql/$库名称/$表名称#P#p1.MYD -- 分区数据
/var/lib/mysql/$库名称/$表名称#P#p1.MYI -- 分区索引
/var/lib/mysql/$库名称/$表名称.par      -- 分区索引
/var/lib/mysql/$库名称/$表名称#p#p0.ibd -- 分区文件
/var/lib/mysql/$库名称/$表名称#p#p1.ibd -- 分区文件
/var/lib/mysql/$库名称/$表名称.par      -- 分区索引
```

schema = 数据表的定义或描述
数据列的分解需要我们深入到业务中才行
SQL = Structured Query Language