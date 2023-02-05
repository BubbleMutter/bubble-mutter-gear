mysql 是单进程多线程的架构

`mysql --help | grep my.cnf` 配置文件读取优先级
`/etc/my.cnf /etc/mysql/my.cnf ~/.my.cnf` 越往后优先级越高

mysql 框架
管理服务和工具
连接池 > SQL接口 > 解析器 > 优化器 > 缓冲 > 存储引擎 > 物理文件

mysql 5.5.8 版本之前, 默认存储引擎使用 MyISAM
mysql 5.5.8 版本开始, 默认存储引擎使用 innodb

# 引擎对比
innodb: 目标用于"在线事物处理"OLTP, 支持 行锁 / 外键 / 非锁定读
MyISAM: 不支持 事务, 表锁, 支持全文索引

# 线程结构
innodb 引擎, 单进程多线程结构
```sql
show variables like 'innodb%threads';

+---------------------------+-------+
| Variable_name             | Value |
+---------------------------+-------+
| innodb_encryption_threads | 0     |
| innodb_purge_threads      | 4     |
| innodb_read_io_threads    | 4     |
| innodb_write_io_threads   | 4     |
+---------------------------+-------+

show engine innodb status;
--------
FILE I/O
--------
I/O thread 0 state: waiting for completed aio requests (insert buffer thread)
I/O thread 1 state: waiting for completed aio requests (log thread)
I/O thread 2 state: waiting for completed aio requests (read thread)
I/O thread 3 state: waiting for completed aio requests (read thread)
I/O thread 4 state: waiting for completed aio requests (read thread)
I/O thread 5 state: waiting for completed aio requests (read thread)
I/O thread 6 state: waiting for completed aio requests (write thread)
I/O thread 7 state: waiting for completed aio requests (write thread)
I/O thread 8 state: waiting for completed aio requests (write thread)
I/O thread 9 state: waiting for completed aio requests (write thread)
```
## 1. Master Thread
1. 主线程 = 将缓冲池中的数据, 异步刷新到磁盘, 保证数据的一致性
   1. 脏页刷新
   2. 合并插入缓冲
   3. 回收 undo 页

## 2. I/O Thread = (insert buffer / log / read / write)
2. `insert buffer thread` = 插入缓冲线程
3. `log thread` = 日志线程
4. 4 个 `read thread` = 底层使用 `aio_read()`
5. 4 个 `write thread` = 底层使用 `aio_write()`

## 3. Purge Thread
6. `purge thread` 回收 undolog 的内存页
    1. 事务开始后, 写操作需要生成对应的 undolog, 用于回滚
    2. 事物提交后, undolog 的内存被回收

## 4. Page Cleaner Thread

# 缓冲池
```sql
show variables like 'innodb_buffer_pool%';

+-------------------------------------+----------------+
| Variable_name                       | Value          |
+-------------------------------------+----------------+
| innodb_buffer_pool_chunk_size       | 134217728      |
| innodb_buffer_pool_dump_at_shutdown | ON             |
| innodb_buffer_pool_dump_now         | OFF            |
| innodb_buffer_pool_dump_pct         | 25             |
| innodb_buffer_pool_filename         | ib_buffer_pool |
| innodb_buffer_pool_instances        | 1              |
| innodb_buffer_pool_load_abort       | OFF            |
| innodb_buffer_pool_load_at_startup  | ON             |
| innodb_buffer_pool_load_now         | OFF            |
| innodb_buffer_pool_size             | 134217728      |
+-------------------------------------+----------------+
```

1. read = 把磁盘数据读取到页, 放到缓冲池中, 再返回到客户端
2. write = 修改操作, 先修改缓冲池中的页, 再在一定条件下刷新回磁盘
3. 缓冲池具体存的对象
    1. data page 数据页
    2. index page 索引页
    3. insert buffer 插入缓冲
    4. adaptive hash index 自适应哈希索引
    5. lock info 锁信息
    6. data dictionary 数据字典信息
    7. redo log_buffer 重做日志缓冲
    8. innodb_additional_mem_pool_size 额外内存池
4. 缓冲池实例
    1. 默认实例是1个
    2. 多个实例时通过哈希方法分配到不同的实例中, 减少竞争, 提高并法
