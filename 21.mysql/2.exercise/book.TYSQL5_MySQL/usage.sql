-- 查询
SELECT * FROM Products;
SELECT vend_id FROM Products;
SELECT prod_name FROM Products;
SELECT prod_id,prod_name,prod_price FROM Products;
-- 去重
SELECT DISTINCT vend_id FROM Products;
-- 限制
SELECT prod_name FROM Products LIMIT 5 OFFSET 3;
-- 排序
SELECT prod_name FROM Products ORDER BY prod_name; 
SELECT prod_id, prod_price, prod_name FROM Products ORDER BY prod_price, prod_name;
SELECT prod_id, prod_price, prod_name FROM Products ORDER BY prod_price DESC;
-- 条件 (等于)
SELECT prod_name, prod_price FROM Products WHERE prod_price = 3.49;
-- 条件 (不等)
SELECT prod_name, prod_price FROM Products WHERE vend_id != 'DLL01';
-- 条件 BETWEEN AND
SELECT prod_name, prod_price FROM Products WHERE prod_price BETWEEN 5 AND 10;
-- 条件 NULL
SELECT * FROM Customers WHERE cust_email IS NULL;
-- 条件 IN
SELECT prod_name, prod_price FROM Products WHERE vend_id IN ('DLL01', 'BRS01') ORDER BY prod_name;
-- 条件 NOT
SELECT prod_name FROM Products WHERE NOT vend_id = 'DLL01' ORDER BY prod_name; -- 全表查询
SELECT prod_name FROM Products WHERE vend_id != 'DLL01' ORDER BY prod_name; -- 全表查询
-- 条件 LIKE % 匹配多个任意字符
SELECT prod_id, prod_name FROM Products WHERE prod_name LIKE 'Fish%';
SELECT prod_id, prod_name FROM Products WHERE prod_name LIKE '%bean bag%'; -- 全表查询
SELECT prod_name FROM Products WHERE prod_name LIKE 'F%y';
SELECT prod_name FROM Products WHERE prod_name LIKE '%'; -- 不能匹配 NULL
-- 条件 LIKE _ 匹配一个任意字符
SELECT prod_id, prod_name FROM Products WHERE prod_name LIKE '__ inch teddy bear'; -- 全表查询
-- 条件 LIKE [] 匹配其中一个字符
SELECT cust_contact FROM Customers WHERE cust_contact LIKE '[JM]%' ORDER BY cust_contact; -- 全表查询
-- 条件 LIKE [^] 匹配以外一个字符
SELECT cust_contact FROM Customers WHERE cust_contact LIKE '[^JM]%' ORDER BY cust_contact; -- 全表查询
-- 逻辑运算 AND
SELECT prod_id, prod_price, prod_name FROM Products WHERE vend_id = 'DLL01' AND prod_price <= 4;
-- 逻辑运算 OR
SELECT prod_name, prod_price FROM Products WHERE vend_id = 'DLL01' OR vend_id = 'BRS01';
-- 逻辑运算 AND 优先级 高于 OR; 所以如果为了使 OR 更优先, 要加括号
SELECT prod_name, prod_price FROM Products
WHERE (vend_id = 'DLL01' OR vend_id = 'BRS01') AND prod_price >= 10;
-- 运算 算术
SELECT prod_id, quantity, item_price, quantity*item_price AS expanded_price FROM OrderItems WHERE order_num = 20008;
-- 运算 字符串 拼接
SELECT CONCAT(vend_name, ' (', vend_country, ')') FROM Vendors ORDER BY vend_name;
-- 运算 字符串 去空格
SELECT CONCAT(vend_name, ' (', TRIM(vend_country), ')') FROM Vendors ORDER BY vend_name;
-- 运算 字符串 大写
SELECT vend_name, UPPER(vend_name) as vend_name_upcase FROM Vendors ORDER BY vend_name;
-- 运算 使用别名
SELECT CONCAT(vend_name, ' (', TRIM(vend_country), ')') AS vend_title FROM Vendors ORDER BY vend_name;
-- 运算 日期 提取年份
SELECT order_num FROM Orders WHERE YEAR(order_date) = 2020;
-- 聚合 AVG 平均值; AVG 会忽略 NULL 行
select AVG(prod_price) AS avg_price FROM Products; -- 所有商品的平均价格
select AVG(prod_price) AS avg_price FROM Products WHERE vend_id = 'DLL01'; -- 厂商 DLL01 商品的均价
-- 聚合 COUNT 统计行数
SELECT COUNT(*) AS num_cust FROM Customers; -- 统计所有客户
SELECT COUNT(cust_email) AS num_cust FROM Customers; -- 统计具有电子邮件的客户
-- 聚合 MAX
SELECT MAX(prod_price) AS max_price FROM Products; -- 最贵的商品
-- 聚合 MIN
SELECT MIN(prod_price) AS min_price FROM Products; -- 最便宜商品
-- 聚合 SUM
SELECT SUM(quantity) AS items_ordered FROM OrderItems WHERE order_num = 20005; -- 订单20005 的商品数量总和
SELECT SUM(quantity*item_price) AS total_price FROM OrderItems WHERE order_num = 20005; -- 订单总金额
-- 聚合 混合用法
SELECT COUNT(*) AS num_items,
    MIN(prod_price) AS price_min,
    MAX(prod_price) AS price_max,
    AVG(prod_price) AS price_avg
    FROM Products;
-- 分组
SELECT vend_id, COUNT(*) AS num_prods FROM Products GROUP BY vend_id;
-- 分组过滤; WHERE 在分组前起作用; HAVING 在分组后起作用
SELECT cust_id, COUNT(*) AS orders FROM Orders GROUP BY cust_id HAVING COUNT(*) >= 2;
SELECT vend_id, COUNT(*) AS num_prods FROM Products GROUP BY vend_id HAVING COUNT(*) >= 2;
-- 分组过滤; 价格不小于 4; 数量在两个以上的商品
SELECT vend_id, COUNT(*) AS num_prods FROM Products
    WHERE prod_price >= 4 GROUP BY vend_id HAVING COUNT(*) >= 2;
-- 分组过滤; 分组后排序
SELECT order_num, COUNT(*) AS items FROM OrderItems
    GROUP BY order_num HAVING COUNT(*) >= 3
    ORDER BY items, order_num;
-- 子查询; 子查询性能不好, 建议使用 JOIN
SELECT cust_id FROM Orders WHERE order_num IN
    (SELECT order_num FROM OrderItems WHERE prod_id = 'RGAN01');  -- ref 1
SELECT cust_name, cust_contact FROM Customers WHERE cust_id IN
    (SELECT cust_id FROM Orders WHERE order_num IN 
    (SELECT order_num FROM OrderItems WHERE prod_id = 'RGAN01')); -- ref 2
-- 子查询; 作为计算字段
SELECT cust_name, cust_state,
    (SELECT COUNT(*) FROM Orders 
    WHERE Orders.cust_id = Customers.cust_id) AS orders
    FROM Customers ORDER BY cust_name;
-- 联接 两个表
SELECT vend_name, prod_name, prod_price
    FROM Vendors INNER JOIN Products
    ON Vendors.vend_id = Products.vend_id;
SELECT Orders.cust_id FROM Orders INNER JOIN OrderItems 
    ON Orders.order_num = OrderItems.order_num
    WHERE OrderItems.prod_id = 'RGAN01'; -- ref 1
-- 联接 多个表
SELECT prod_name, vend_name, prod_price, quantity
    FROM OrderItems, Products, Vendors
    WHERE Products.vend_id = Vendors.vend_id
    AND OrderItems.prod_id = Products.prod_id
    AND order_num = 20007;
SELECT cust_name, cust_contact FROM Customers, Orders, OrderItems
    WHERE Customers.cust_id = Orders.cust_id
    AND OrderItems.order_num = Orders.order_num
    AND prod_id = 'RGAN01'; -- ref 2
-- 组合 UNION 去重
SELECT cust_name, cust_contact, cust_email
    FROM Customers
    WHERE cust_state IN ('IL', 'IN', 'MI')
    UNION
    SELECT cust_name, cust_contact, cust_email
    FROM Customers
    WHERE cust_name = 'Fun4All';
-- 组合 UNION 包含重复行
SELECT cust_name, cust_contact, cust_email
    FROM Customers
    WHERE cust_state IN ('IL', 'IN', 'MI')
    UNION ALL
    SELECT cust_name, cust_contact, cust_email
    FROM Customers
    WHERE cust_name = 'Fun4All';
-- 组合 UNION 再排序
SELECT cust_name, cust_contact, cust_email
    FROM Customers
    WHERE cust_state IN ('IL', 'IN', 'MI')
    UNION ALL
    SELECT cust_name, cust_contact, cust_email
    FROM Customers
    WHERE cust_name = 'Fun4All'
    ORDER BY cust_name, cust_contact;
-- 拷贝一个表
CREATE TABLE IF NOT EXISTS CustCopy AS SELECT * FROM Customers;
-- 删除一个表
DROP TABLE IF EXISTS CustCopy;
-- 新增一列
ALTER TABLE Vendors ADD IF NOT EXISTS vend_phone CHAR(20);
-- 删除一列
ALTER TABLE Vendors DROP IF EXISTS vend_phone;
-- 新增主键
ALTER TABLE Vendors ADD PRIMARY KEY (vend_id);
ALTER TABLE Vendors DROP PRIMARY KEY;

-- Q: 从 Customers 表中检索所有的 cust_id
SELECT cust_id FROM Customers;

-- Q: 从 OrderIterms 中列出已订购的产品清单(不重复); 并最终显示 7 行
SELECT DISTINCT prod_id FROM OrderItems LIMIT 7;

-- Q: 从 Customers 表中检索所有的顾客名; 并降序排列
SELECT cust_name FROM Customers ORDER BY cust_name DESC;

-- Q: 从 Orders 表中检索 顾客ID 和 订单号; 并先按照顾客ID排序 再按照订单日期倒序排
SELECT cust_id, order_num FROM Orders ORDER BY cust_id, order_date DESC;

-- Q: 从 Orders 表中检索所有的列按照商品 数量由多到少 价格由高到低的排序
SELECT * from OrderItems ORDER BY quantity DESC, item_price DESC;

-- Q: 从 Products 表中检索产品ID 和 产品名称 返回价格 9.49 的产品
SELECT prod_id, prod_name from Products where prod_price = 9.49;

-- Q: 从 Products 表中检索产品ID 和 产品名称 返回价格为 9 或 更高的产品
SELECT prod_id, prod_name from Products where prod_price >= 9.49;

-- Q: 从 OrderItems 表中检索出所有不同的订单号, 数量包含 100个 或者 更多的产品
SELECT DISTINCT order_num from OrderItems where quantity >= 100;

-- Q: 从 Products 表中检索价格在 3 到 6 之间的产品名称和价格, 并按照价格排序
SELECT prod_name, prod_price FROM Products
    WHERE prod_price BETWEEN 3 AND 6 ORDER BY prod_price;

-- Q: 从 Vendors 表中检索供应商名称; 返回美国 加利福尼亚州的供应商
SELECT vend_name FROM Vendors WHERE vend_country = 'USA' AND vend_state = 'CA';

-- Q: 从 OrderItems 表中检索 订单号, 产品ID, 数量, 返回总量至少100的 BR01, BR02, BR03 的订单
SELECT order_num, prod_id, quantity FROM OrderItems
    WHERE quantity >= 100 AND  prod_id IN ('BR01', 'BR02', 'BR03');

-- Q: 从 Products 表中检索产品名称 和 价格, 返回价格在3~6之间, 要求使用 AND, 并对价格排序
SELECT prod_name, prod_price FROM Products
    WHERE prod_price >= 3 AND prod_price <= 6 ORDER BY prod_price;

-- Q: 从 Products 表中检索产品名称 产品描述, 返回描述中包含 toy 的产品
select prod_id, prod_desc FROM Products
    WHERE prod_desc LIKE '%toy%';

-- Q: 从 Products 表中检索产品名称 产品描述, 返回描述中包含 toy 的产品, 并对名称结果排序
select prod_id, prod_desc FROM Products
    WHERE prod_desc LIKE '%toy%' ORDER BY prod_desc;

-- Q: 从 Products 表中检索产品名称 产品描述, 返回描述中同时包含 toy 和 carrots 的产品
select prod_id, prod_desc FROM Products
    WHERE prod_desc LIKE '%toy%' AND prod_desc LIKE '%carrots%';

-- Q: 从 Products 表中检索产品名称 产品描述, 返回描述中以先后顺序出现 toy carrots 的产品
select prod_id, prod_desc FROM Products
    WHERE prod_desc LIKE '%toy%carrots%';

-- Q: 从 Products 表中检索产品ID 产品价格 促销价格(10%折扣)
SELECT prod_id, prod_price, prod_price*0.9 as sale_price FROM Products;

-- Q: 从 Customers 表中返回 顾客ID, 顾客名称, 登陆名 user_login; 
--    登陆名全是大写, 由顾客联系人前两个字符 和 所在城市前三个字符组成
SELECT cust_id, cust_name, UPPER(CONCAT(LEFT(cust_contact, 2), LEFT(cust_city, 3))) AS user_login FROM Customers;

-- Q: 从 Orders 表中检索订单号 订单日期, 返回 2020年1月的订单
SELECT order_num, order_date FROM Orders
    WHERE year(order_date) = 2020 AND month(order_date) = 1;
SELECT order_num, order_date FROM Orders
    WHERE order_date BETWEEN '2020-01-01' AND '2020-01-31';

-- Q: 从 OrderItems 表中求出已售出产品的总数
SELECT SUM(quantity) FROM OrderItems;

-- Q: 从 OrderItems 表中求出商品ID BR01 的产品总数
SELECT SUM(quantity) FROM OrderItems WHERE prod_id = 'BR01';

-- Q: 从 Products 表中价格不超过 10 的最贵的产品价格, 重命名为 max_price
SELECT MAX(prod_price) AS max_price FROM Products WHERE prod_price <= 10;

-- Q: 从 OrderItems 表中返回每个订单号各有多少行order_lines, 结果按照行数排序
SELECT order_num, COUNT(*) AS order_lines FROM OrderItems
    GROUP BY order_num ORDER BY order_lines;

-- Q: 从 Products 表中返回每个供应商最便宜的产品, 并按照从低到高对供应商进行排序
SELECT vend_id, min(prod_price) AS cheapest_item FROM Products
    GROUP BY vend_id ORDER BY cheapest_item;

-- Q: 从 OrderItems 中返回至少有 100项的所有订单 的订单号 (其实是最佳顾客指标, 即订单最大的)
SELECT order_num, sum(quantity) AS sum_quan FROM OrderItems
    GROUP BY order_num HAVING sum_quan >= 100;

-- Q: 从 OrderItems 中返回总加至少为1000的所有订单的 订单号, 按订单号排序
SELECT order_num, sum(quantity*item_price) AS sum_price FROM OrderItems
    GROUP BY order_num HAVING sum_price >= 1000 ORDER BY order_num;

-- Q: 返回购买价格为 10 以及以上产品的顾客列表;
--    OrderItems 表包含订单号和产品价格;
--    Orders 表包含订单号和顾客ID
SELECT cust_id FROM Orders WHERE order_num IN
    (SELECT order_num FROM OrderItems WHERE item_price >= 10);

-- Q: 返回产品ID 是 BR01 的顾客ID 和日期;
--    OrderItems 表检索产品ID 的 订单号
--    Orders 表包含顾客ID 和 日期
SELECT cust_id, order_date FROM Orders WHERE order_num IN
    (SELECT order_num FROM OrderItems WHERE prod_id = 'BR01')
    ORDER BY order_date; -- ref 3

-- Q: 返回产品ID 是 BR01 的顾客 email
--    OrderItems 表检索产品ID 的 订单号
--    Orders 表包含顾客ID
--    Customers 表包含顾客 email
SELECT cust_email FROM Customers WHERE cust_id IN
    (SELECT cust_id FROM Orders WHERE order_num IN
    (SELECT order_num FROM OrderItems WHERE prod_id = 'BR01')); -- ref 4

-- Q: 返回顾客ID 以及他们订购的总金额
--    OrderItems 表包含订单ID, 产品数量 和 产品金额
--    Orders 表包含用户ID和订单ID
SELECT cust_id, 
    (SELECT SUM(quantity*item_price) FROM OrderItems
     WHERE OrderItems.order_num = Orders.order_num) AS total_ordered
    FROM Orders ORDER BY total_ordered;

-- Q: 返回 产品名称 和 售出总量
--    OrderItems 表包含 产品ID, 产品数量
--    Products 表包含 产品ID, 产品名称
SELECT prod_name,
    (SELECT SUM(quantity) FROM OrderItems
     WHERE OrderItems.prod_id = Products.prod_id) AS quant_sold
    FROM Products;

-- Q: 返回 顾客名称 和 相关订单号; 先按照顾客名称再按照订单号排序
SELECT cust_name, order_num FROM Customers
    INNER JOIN Orders ON Customers.cust_id = Orders.cust_id
    ORDER BY cust_name, order_num;

-- Q: 返回 顾客名称, 相关订单号, 每个订单的总价 order_total; 先按照顾客名称再按照订单号排序
SELECT cust_name, Orders.order_num, SUM(quantity*item_price) AS order_total FROM Customers 
    INNER JOIN Orders ON Customers.cust_id = Orders.cust_id
    INNER JOIN OrderItems ON Orders.order_num = OrderItems.order_num
    GROUP BY OrderItems.order_num
    ORDER BY cust_name, order_num;

-- Q: 返回产品ID 是 BR01 的顾客ID 和日期;
--    OrderItems 表检索产品ID 的 订单号
--    Orders 表包含顾客ID 和 日期
SELECT cust_id, order_date FROM Orders
    INNER JOIN OrderItems ON Orders.order_num = OrderItems.order_num
    WHERE prod_id = 'BR01'
    ORDER BY order_date; -- ref 3

-- Q: 返回产品ID 是 BR01 的顾客 email
--    OrderItems 表检索产品ID 的 订单号
--    Orders 表包含顾客ID
--    Customers 表包含顾客 email
SELECT cust_email FROM Customers
    INNER JOIN Orders ON Customers.cust_id = Orders.cust_id
    INNER JOIN OrderItems ON Orders.order_num = OrderItems.order_num
    WHERE prod_id = 'BR01'; -- ref 4

-- Q: 返回 顾客名称 和 总价至少为1000的所有订单的 订单号, 按订单号排序
SELECT Customers.cust_name, sum(OrderItems.quantity*OrderItems.item_price) AS sum_price
    FROM Customers
    INNER JOIN Orders ON Customers.cust_id = Orders.cust_id
    INNER JOIN OrderItems ON OrderItems.order_num = Orders.order_num
    GROUP BY OrderItems.order_num
    HAVING sum_price >= 1000 ORDER BY OrderItems.order_num;

-- Q: 检索每个顾客名称 和 其所有的订单号, 去掉没有订单的顾客
SELECT Customers.cust_name, Orders.order_num FROM Customers
    INNER JOIN Orders ON Customers.cust_id = Orders.cust_id;

-- Q: 检索每个顾客名称 和 其所有的订单号, 包含没有订单的顾客
SELECT Customers.cust_name, Orders.order_num FROM Customers
    LEFT JOIN Orders ON Customers.cust_id = Orders.cust_id;

-- Q: 列出供应商 及其 可供产品的种类
SELECT Vendors.vend_id, COUNT(*) FROM Vendors
    LEFT JOIN Products ON Vendors.vend_id = Products.vend_id
    GROUP BY vend_id;

-- Q: 从 OrderItems 表中检索产品ID和数量, 过滤数量100的行
--    结合 OrderItems 表中检索产品ID和数量, 过滤 产品ID BNBG 开头的
SELECT prod_id, quantity
    FROM OrderItems WHERE quantity = 100
    UNION
    SELECT prod_id, quantity
    FROM OrderItems WHERE prod_id LIKE 'BNBG%';
SELECT prod_id, quantity FROM OrderItems
    WHERE quantity = 100 OR prod_id LIKE 'BNBG%'; -- 非 UNION 版本

-- Q: 组合 Products 表的产品名称 和 Customers 表中的顾客名称
--    按照 产品名称 排序
SELECT prod_name FROM Products UNION
    SELECT cust_name FROM Customers ORDER BY prod_name;

-- Q: 