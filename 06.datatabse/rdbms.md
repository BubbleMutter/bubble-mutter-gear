RDBMS 指的是 在"关系模型"基础上, 借助"集合代数"等数学概念和方法 处理数据的数据库

RDBMS
数据以表格的形式出现
表格行 = 各种记录名称
表格列 = 名称对应的数据域
许多的行和列组成一张表单
若干的表单组成 database

database = { table = column * row }

RDBMS的一些术语

```s
database = 关联 table 的集合
table    = data matrix
row      = 一组相关的数据 (形同 tuple)
column   = 数据元素 包含了相同类型的数据
header   = 表头; 每一列的名称
index    = 用以快速查询表中特定信息; 针对一列或多列的排序结构 (平衡树吗?)
redundance = 存储两倍数据，降低写时效率，提高数据的安全性
primary_key = 主键; 一个 table 只有一个; 由一列或多列组合;
              每行的 primary key 的值必须是唯一的
foreign_key = 外键, 用于关联两个表
参照完整性 = 参照的完整性要求关系中不允许引用不存在的实体。
实体完整性 = 是关系模型必须满足的完整性约束条件，目的是保证数据的一致性。
```