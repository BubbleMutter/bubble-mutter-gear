[TOC]
# 2. datatype
1. 整形数值类型 { INTEGER SMALLINT DECIMAL NUMERIC }
2. 近似数值类型 { FLOAT REAL DOUBLE PRECISION }
3. 扩展数值类型 { INYINT MEDIUMINT BIGINT } (mysql特有; 非标准sql)
4. 日期时间类型 { DATE TIME DATETIME TIMESTAMP YEAR }
5. 字符串类型
    1. CHAR VARCHAR
    2. BINARY VARBINARY
    3. BLOB TINYBLOB MEDIUMBLOB LONGBLOB
    4. TEXT TINYTEXT MEDIUMTEXT LONGTEXT

## 2.1 数值
| type | size | range (signed) | range (unsigned) |
| :-: | :-: | :-: | :-: |
| TINYINT   | 1 | (-128, 127) | (0，255) |
| SMALLINT  | 2 | (-32768,32767) | (0, 65535) |
| MEDIUMINT | 3 | (-8388608, 8388607) | (0，16777215) |
| INT       | 4 | (-2147483648，2147483647) | (0，4294967295) |
| BIGINT    | 8 | (-2^63, 2^63-1) | 2^64 |
| FLOAT     | 4 | | |
| DOUBLE    | 8 | | |
| DECIMAL   | max(M, N)+2 | | |

## 2.2 日期时间

| type | size | range | format |
| :-: | :-: | :-: | :-: |
| DATE      | 3 | (1000-01-01, 9999-12-31) | YYYY-MM-DD |
| TIME      | 3 | (-838:59:59, 838:59:59) | HH:MM:SS |
| YEAR      | 1 | (1901, 2155) | YYYY |
| DATETIME  | 8 | (1000-01-01 00:00:00, 9999-12-31 23:59:59) | YYYY-MM-DD HH:MM:SS |
| TIMESTAMP | 4 | (19700101 000000, 20380119 031407) | YYYYMMDD HHMMSS|

## 2.3 字符串
| type | size |
| :-: | :-: |
| CHAR       | (0, 255)   |
| VARCHAR    | (0, 65535) |
| BINARY     |            |
| VARBINARY  |            |
| BLOB       | (0, 65535)      |
| TINYBLOB   | (0, 255)        |
| MEDIUMBLOB | (0, 16777215)   |
| LONGBLOB   | (0, 4294967295) |
| TEXT       | (0, 65535)      |
| TINYTEXT   | (0, 255)        |
| MEDIUMTEXT | (0, 16777215)   |
| LONGTEXT   | (0, 4294967295) |

1. BLOB 和 TEXT
   1. BLOB 存储二进制, 可变大小
   2. TEXT 存储文本串, 可变大小
   3. BLOB 和 TEXT 区别: 底层 排序 和 索引 机制不同
2. CHAR 和 VARCHAR
   1. 定义时指定 size
   2. CHAR 和 VARCHAR 区别: 底层 保存 和 索引 机制不同
   3. 字符串涉及字符集的配置: 不区分大小写相对比较耗时
3. BINARY 和 VARBINARY
   1. 形同 CHAR 和 VARCHAR
   2. 二进制的指的是没有字符集, 因此排序基于二进制数值
   3. 与 CHAR VARCHAR 区别: 排序 方式不同
