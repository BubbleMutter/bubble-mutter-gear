[TOC]
# table 复制
```sql
-- 复制表的属性
CREATE TABLE $新表名称 LIKE $旧表名称;
-- 复制表的数据
INSERT INTO $新表名称 SELECT * FROM $旧表名称;

-- 复制表的属性和数据, ?这个语法是正确的吗?
CREATE TABLE $新表名称 (SELECT * FROM $旧表名称);
```

# 重复数据
## 1. 防止重复数据
```sql
-- 防止重复数据 方法1: 设置主键
CREATE TABLE person_tbl (
    first_name CHAR(20) NOT NULL,
    last_name CHAR(20) NOT NULL,
    PRIMARY KEY (last_name, first_name)
);

-- 防止重复数据 方法2: 设置单一索引(如果这个索引没啥用)
CREATE TABLE person_tbl (
    first_name CHAR(20) NOT NULL,
    last_name CHAR(20) NOT NULL,
    UNIQUE (last_name, first_name)
);
```

## 2. 统计重复数据
```sql
SELECT COUNT(*) as repetitions, last_name, first_name FROM person_tbl GROUP BY (last_name, first_name) HAVING repetitions > 1;
```

## 3. 过滤重复数据
```sql
-- 读取 不重复的数据 (这里是组合列不重复, 单一列可以重复) 方法1
SELECT DISTINCT last_name, first_name FROM person_tbl;
-- 读取 不重复的数据 (这里是组合列不重复, 单一列可以重复) 方法2
SELECT last_name, first_name FROM person_tbl GROUP BY (last_name, first_name);
```

# 其他
## Q: 商品表 Goods 和 分类表 Category 查同类商品数量
1. "商品表"Goods    = Goods_id, goods_name, cat_id(分类id)
2. "分类表"Category = cat_id(分类id), cat_name(分类名称)
3. question = 求同一类别商品的数量, 返回 { 分类ID 分类名称 商品数量 }
4. answer   = 
```sql
Select c.*, count(g.goods_id) as goods_count from category as c left join goods as g ON c.cat_id=g.cat_id group by c.cat_id;
```
5. Note: 因为存在分类ID 没有任何商品, 所以应该展示"分类表"的所有行
6. 优化: 使用 join 操作十分耗性能
   1. 分类表新增一列 goods_count, 
   2. 增删商品时, 事务性维护 goods_count
   3. `select * from Category;` 即可

## Q: 
   

