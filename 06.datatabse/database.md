[TOC]
# database type
column database      = clickhouse
row database         = mysql, postgresql
time series database = prometheus, influxdb, victoria, siridb
document database    = mongodb
graph database       = neo4j

# database theory
远古时代就开始的数据库理论, 不完全是某个数据库的底层实现与使用方法

数据库（Database）是按照数据结构来组织、存储和管理数据的仓库。
每个数据库都有不同的 API 用于创建，访问，管理，搜索和复制所保存的数据。
create, access, manage, search, copy