```sql
show engines; -- 显示支持的 引擎

SELECT @@global.sql_mode; -- 对应 `my.cnf` sql-mode=

-- 查询缓存 --------------------------------------------
show variables like 'query_cache%';           -- 显示所有 query_cache 相关
SELECT @@global.query_cache_limit;            -- 默认: 1048576
SELECT @@global.query_cache_min_res_unit;     -- 默认: 4096
SELECT @@global.query_cache_size;             -- 默认:  0, 不能其用缓存; 参考值 32*1024*1024
SELECT @@global.query_cache_type;             -- 默认: ON, 表示启用缓存;
SELECT @@global.query_cache_wlock_invalidate; -- 默认: OFF
-- 即当 cache_size 足够大 和 cache_type = ON 缓存才能开启

set @@global.$变量名=$变量值;

-- 慢查询详细日志
select @@global.slow_query_log;      -- 默认: 0 表示关闭; 1 表示开启
                                     -- 开启后  超过时间阈值即打印详细查询详细日志
select @@global.slow_query_log_file; -- 慢查询文件
select @@global.long_query_time;     -- 慢查询阈值(单位 秒), 即超过这个时间才算是慢查询

show database;
show tables;
```