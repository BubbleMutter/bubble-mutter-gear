简单介绍 https://blog.csdn.net/xyh930929/article/details/83510608
引擎对比 https://yinwu.blog.csdn.net/article/details/111089281


独立表 vs 共享表

删除行, 删除表 造成的磁盘碎片化问题

# my.cnf 配置文件
```conf
[mysqld]
innodb_file_per_table=1 # 1: 使用独立表空间; 0: 使用共享表空间
innodb_data_home_dir="/var/lib/mysql"        # 数据库文件存放目录 (Innodb 不创建目录, 系统管理者确保目录创建)
innodb_log_group_home_dir="/var/lib/mysql"   # 日志文件存放目录    (Innodb 不创建目录, 系统管理者确保目录创建)
innodb_data_file_path=ibdata1:10M:autoextend # 共享表的数据文件   (/var/lib/mysql/ibdata1, 不从属具体数据库)
```

```sql
set global innodb_file_per_table=1; -- 1: 使用独立表空间; 0: 使用共享表空间
```
