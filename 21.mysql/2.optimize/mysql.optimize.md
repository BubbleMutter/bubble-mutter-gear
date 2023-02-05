设计：存储引擎，字段类型，范式与逆范式
功能：索引，缓存，分区分表。
架构：主从复制，读写分离，负载均衡。
合理SQL：测试，经验。

https://www.cnblogs.com/sharpest/p/10390035.html    优化技巧总结 blog
https://www.cnblogs.com/binbinyouni/p/6070715.html  优化技巧总结 blog

优化形成具体的 优化 checklist
详细的用于优化的技术, 拆分到应该放的地方


innodb 的 `PRIMARY KEY` 降低插入性能, 需要二分查找确定位置, `PRIMARY KEY` 底层是经过排序
MyISAM 的 `PRIMARY KEY` 是无序插入的
innodb擅长处理并发的。因为它使用了行级锁定, 其它行没有锁。

1. 加锁场景 = 保证操作的隔离性(多个客户端操作不能互相影响), 需要加锁
2. 锁的类型
    1. 读锁 = S-Lock, 阻塞其他客户端的写操作, 但允许读操作 (共享锁)
    2. 写锁 = X-Lock, 阻塞其他客户端的读写操作 (独占锁, 互斥锁)
3. 锁的粒度
    1. 行锁 = 并发性较好, 锁本身开销大, 每一行都要额外的变量存储, 每一行操作时都要判断锁
    2. 表锁 = 并发性不好, 锁本身开销小

字段类型应该要满足需求，尽量要满足以下需求。
尽可能小(占用存储空间少), 尽可能定长(占用存储空间固定), 尽可能使用整数.

关于INT(11)存放时间戳的优点如下:
用 INT 替换 DATETIME 来保存时间戳的优点
1. INT 占 4 bytes, DATETIME 占 8 bytes
2. INT 存储索引的空间比 DATETIME 小, 查询快, 排序效率高
3. INT 在计算机时间差等范围问题, 效率更高
Q: 如何使用 INT(11) 存放时间戳, 


字符集校对规则
utf8_general_ci 表示校对时不区分大小写
utf8_general_cs 表示校对时区分大小写
同样地 gbk_chinese_ci 和 gbk_chinese_cs

短文本定长用 char, 变长用 varchar, 长文本用 text

# PRIMARY KEY 和 UNIQUE 的区别在于
1. NULL = 主键索引不能为空值，唯一索引允许空值
2. 个数 = 主键索引在一张表内只能创建一个, 唯一索引可以创建多个
3. 关系 = 主键索引肯定是唯一索引, 但唯一索引不一定是主键索引

# QUERY
查询缓存区只能识别 "完全一样" 的查询语句, 包括大小写, 空格, 列顺序等
缓存删除 = 1) 缓存空间不够; 2) 表的数据更改;

分区分表 = 数据表过大, 导致查询和插入性能低下; 使用分区 和 分表技术提高性能

# 分区
1. 特性
    1. 数据分段划分在多个文件存放, 可以是同一块磁盘也可以在不同的机器。
    2. sql 形式上是一张表，但数据拆分到多个文件
    3. sql 增删改查操作的是大表的 表名称, 
    4. mysql "逻辑层"维护分区的逻辑 (这个与 引擎无关)
2. 分区算法
    ```sql
    --  key 整数取余, 参数必须是整型
    create table $表名称() engine=innodb partition by key (id) partitions $分区个数;
    -- hash 整数取余, 参数必须是整型
    create table $表名称() engine=innodb partition by hash (month(birthday)) partitions $分区个数;

    -- list 列表, 指定每个分区的存储条件
    create table $表名称() engine=innodb partition by list (month(birthday)) (
        partition p0 VALUES IN (3,  4,  5), -- 春天
        partition p1 VALUES IN (6,  7,  8), -- 夏天
        partition p2 VALUES IN (9, 10, 11), -- 秋天
        partition p3 VALUES IN (12, 1,  2)  -- 冬天
    );
    -- range 列表, 指定范围
    create table $表名称() engine=innodb partition by range (year(birthday)) (
        partition p1990 VALUES LESS THAN (1990),
        partition p2000 VALUES LESS THAN (2000),
        partition p2010 VALUES LESS THAN (2010),
        partition p2020 VALUES LESS THAN (2020),
        partition p_end VALUES LESS THAN MAXVALUE,
    )

    -- 增加分区数量 基于 key hash 算法
    alter table add partition partitions 5;
    -- 减少分区数量 key hash 算法
    alter table COALESCE partition 7;

    -- 增加分区数量 基于 list range 算法
    alter table $表名称 add partition(
        partition p2030 VALUES less than (2030),
    );
    -- 增加分区数量 基于 list range 算法
    alter table $表名称 drop partition p2010;
    ```
4. ?取模算法的分区个数如何选择?
5. 增删分区之后 ? 是否会导致分区数据丢失 ?
    1. 删除取模算法 ( key hash )   的分区, 不会导致已有分区数据的丢失. 因为会重新分配数据到新的分区.
    2. 删除条件算法 ( list range ) 的分区, 会导致分区数据丢失. 添加分区不会.

# 分表
1. 水平分表 = 手动把数据拆分到不同的表中, 由上层业务 选择插入 + 分别查询 + 整合结果
2. 垂直分表 = 拆分列, 通过 id 同步

# server framework
1. 主从复制 = 一个主服务器负责写数据; 多个从服务器 负责读数据; 通过配置 msyql, 自动完成主从复制 (from 主 to 从)
2. 负载均衡
    1. 在主从复制架构的基础上, 新增一个 mysql proxy 服务器
    2. 代理服务器根据操作指令, 把写操作重定向到 主服务器, 把读操作均衡到各个 从服务器
    3. 客户端只需要与代理服务器连接, 主从服务器对于客户端是透明的


# 其他
1. 并发性的 SQL = 少用(不用)多表操作, 而是将复杂的SQL拆分多次执行
2. 大量数据插入 = 先关闭约束及索引, 完成数据插入, 再重新生成索引及约束
    ```sql
    alter table $表名称 disable keys; -- MyISAM; 禁用
    Alter table $表名称 enable keys; -- MyISAM; 启用
    
    drop index, drop constraint; -- innodb 禁用
    begin transaction|set autocommit=0; -- 开启事务; 禁用自动提交
    -- 大量数据插入
    commit;                             -- 提交事务; 
    add index, add constraint;   -- innodb 禁用
    ```
3. 存储过程 和 触发器 的 开始时设置 `SET NOCOUNT ON` 结束时设置 `SET NOCOUNT OFF`
    + 避免 存储过程 和 触发器 的每个语句执行后向 client 发送 DONE_IN_PROC 消息
4. 尽量避免大事务操作, 提高系统并发能力
5. 尽量避免返回大数据量到客户端, 否则应该考虑重新设计与实现
6. 尽量避免向客户端返回大数据量, 若数据量过大, 应该考虑相应需求是否合理
7. 尽量使用 集的方法 替代 "游标方法" 和 "临时表方法"
    1. 小型数据(千条量级) FAST_FORWARD 游标表现较 逐行比较 效率高, 特别是跨表查询时

# 临时表优化
1. 避免频繁 创建 和 删除 "临时表", 可以的话使用 "表变量" 来代替
2. "临时表" 使用场景 = 重复引用大型表或常用表中的某个数据集
3. 新建"临时表", 插入大量数据时, 优化方式:
   使用 `select into` 替代 `create table` 可以避免大量 log (2021/07/29 待考证),
4. 新建"临时表", 插入少量数据时, 先 `create table` 再 `insert`
5. 必须在储存过程最后 显式删除 "临时表",
    ```sql
    truncate table $表名称;
    drop table $表名称
    ```

# 其他
1. 尽可能的使用 varchar/nvarchar 代替 char/nchar
   因为 a) 变长字段存储空间小, 可以节省存储空间
        b) 查询一个相对较小的字段, 搜索效率显然要高些
2. 不要返回多余的字段, 即 select 的业务确实关注的字段