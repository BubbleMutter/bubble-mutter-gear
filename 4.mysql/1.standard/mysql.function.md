# 1. 字符函数
    | ASCII(s) | 返回字符串 s 的第一个字符的 ASCII 码 
1. 计算
    + FIELD(s, s1, s2...) = 返回字符串 s 在字符串列表(s1,s2...) 中出现的位置
    + FIND_IN_SET(s1, s2) = 返回在字符串s2中与s1匹配的字符串的位置
    + LOCATE(s1,s)        = 从字符串 s 中获取 s1 的开始位置
    + POSITION(s1 IN s)   = 从字符串 s 中获取 s1 的开始位置
    + STRCMP(s1,s2)       = 形同 `strcmp`
    + CHAR_LENGTH(s)      = `strlen`
    + CHARACTER_LENGTH(s) =	`strlen`
2. 拼接
    + CONCAT(s1, s2, ..., sn)  = 字符串拼接
    + CONCAT_WS(x, s1,s2...sn) = 字符串拼接, 中间插入分隔符 x
    + LPAD(s1,len,s2) = 在字符串 s1 的开始处拼接字符串 s2, 使字符串长度达到 len
    + RPAD(s1,len,s2) = 在字符串 s1 的结尾处拼接字符串 s2, 使字符串的长度达到 len
3. 替换
    + REVERSE(s) = 翻转字符串
    + LCASE(s)   = 将字符串转换为小写
    + LOWER(s)   = 将字符串转换为小写
    + UCASE(s)   = 将字符串转换为大写
    + UPPER(s)   = 将字符串转换为大写
    + REPLACE(s,s1,s2)    = 将字符串 s2 替代字符串 s 中的字符串 s1
    + INSERT(s1,x,len,s2) = 字符串 s2 替换 s1 的 x 位置开始长度为 len 的字符串
    + FORMAT(x,n) = 将数字 x 进行格式化 "###, ###, ###.##", 将x保留到小数点后n位, 最后一位四舍五入
4. 提取
    + LTRIM(s)   = 去掉字符串 s 开始处的空格
    + RTRIM(s)   = 去掉字符串 s 结尾处的空格
    + TRIM(s)    = 去掉字符串 s 开始和结尾处的空格
    + LEFT(s,n)  = 返回字符串 s 的前 n 个字符
    + RIGHT(s,n) = 返回字符串 s 的后 n 个字符
    + MID(s,n,len) = 从字符串 s 的 n 位置截取长度为 len 的子字符串
    + SUBSTR(s, start, length)    = 同于 MID
    + SUBSTRING(s, start, length) = 同于 SUBSTR
    + SUBSTRING_INDEX(s, delimiter, number) = 返回从字符串 s 的第 number 个出现的分隔符 delimiter 之后的子串, number 是负数则反向计算
5. 生成
    + SPACE(n)    = 返回 n 个空格
    + REPEAT(s,n) = 将字符串 s 重复 n 次

# 2. 数值函数
1. 基础运算
    + ABS(x)       = 求绝对值
    + n DIV m      = 同于 `n / m`
    + MOD(x, y)    = 同于 `x % y`
    + SQRT(x)      = 求 x 的平方根
    + RAND()       = 返回 0 到 1 的随机数(浮点数)
    + SIGN(x)      = 返回 x 的符号，x 是负数、0、正数分别返回 -1、0 和 1
    + PI()         = 圆周率
    + RADIANS(x)   = 将角度转换为弧度
    + DEGREES(x)   = 将弧度转换为角度
    + GREATEST(expr1, expr2, expr3, ...) = 返回列表中的最大值
1. 三角函数
    + ACOS(x)     = 求反余弦值(参数是弧度)
    + ASIN(x)     = 求反正弦值(参数是弧度)
    + ATAN(x)     = 求反正切值(参数是弧度)
    + ATAN2(n, m) = 求反正切值(参数是弧度)
    + COS(x)      = 求余弦值(参数是弧度)
    + COT(x)      = 求余切值(参数是弧度)
    + SIN(x)      = 求正弦值(参数是弧度)
    + TAN(x)      = 求正切值(参数是弧度)
2. 对数
    + LN(x)        = 求 x 的自然对数, 同于 `LOG(e, x)`
    + LOG(x)       = 求 x 的自然对数, 同于 `LOG(e, x)`
    + LOG(base, x) = 求 x 以 base 为底的对数
    + LOG10(x)     = 求 x 以 10 为底的对数, 同于 `LOG(10, x)`
    + LOG2(x)      = 求 x 以  2 为底的对数, 同于 `LOG( 2, x)`
3. 指数
    + EXP(x)       = 求自然常数 e 的 x 次方
    + POW(x, y)    = 返回 x 的 y 次方
    + POWER(x, y)  = 返回 x 的 y 次方
4. 取整
    + CEIL(x)        = 求大于或等于 x 的最小整数
    + CEILING(x)     = 求大于或等于 x 的最小整数
    + FLOOR(x)       = 求小于或等于 x 的最大整数
    + ROUND(x)       = 求 x 的四舍五入的整数
    + TRUNCATE(x, y) = 求 x 保留到小数点后 y 位的值(不进行 四舍五入)
5. 聚合函数
    + COUNT(expr)  = 求查询结果总数，expr 可以是列名称, 或者 `*` 号
    + AVG(expr)    = 求表达式的平均值, expr 可以是列名称, 或列名称和函数共同构成的表达式
    + MAX(expr)    = 求表达式的最大值, expr 可以是列名称, 或列名称和函数共同构成的表达式
    + MIN(expr)    = 求表达式的最小值, expr 可以是列名称, 或列名称和函数共同构成的表达式
    + SUM(expr)    = 求表达式的加法和, expr 可以是列名称, 或列名称和函数共同构成的表达式

# 3. 日期函数
1. 获取当前时间 / 日期
    + CURTIME()        = 当前时间
    + CURRENT_TIME()   = 当前时间
    + CURDATE()        = 当前日期
    + CURRENT_DATE()   = 当前日期
    + CURRENT_TIMESTAMP() = 返回当前日期和时间
    + LOCALTIME()         = 返回当前日期和时间
    + LOCALTIMESTAMP()    = 返回当前日期和时间
    + NOW()               = 返回当前日期和时间
    + SYSDATE()           = 返回当前日期和时间
2. 时间日期加减法
    + ADDTIME(t, delta) = 时间 t 加上时间 delta
    + SUBTIME(t, delta) = 时间 t 减去时间 delta
    + ADDDATE(d, n)     = 日期 d 加上 n 天的日期 (n 只能是天数)
    + SUBDATE(d, n)     = 日期 d 减去 n 天的日期 (n 只能是天数)
    + DATE_ADD(d, INTERVAL expr type) = 从日期 d 加上 expr 单位 的日期 (日期单位可以是 日/月/周等)
    + DATE_SUB(d, INTERVAL expr type) = 从日期 d 减去 expr 单位 的日期 (日期单位可以是 日/月/周等)
    + PERIOD_ADD(period, number)      = 日期 年-月 加上 number 月
    + DATEDIFF(d1, d2)                = 日期 d1 与 d2 的天数差
    + TIMEDIFF(time1, time2)          = 时间 t1 与 t2 的时间差
    + PERIOD_DIFF(period1, period2)   = 日期 年-月 period1 与 period2 的月数差
3. 时间日期解析
    + HOUR(t)   = 时间 t 中的小时值
    + MINUTE(t) = 时间 t 中的分钟值
    + SECOND(t) = 时间 t 中的秒钟值
    + TIME_TO_SEC(t) = 时间 t 一共的秒钟值
    + MICROSECOND(t) = 时间 t 中的微秒值
    + YEAR(d)   = 日期 d 中的年份值
    + MONTH(d)  = 日期 d 中的月份值
    + DAY(d)    = 日期 d 中的日数值 (该月的第几天)
    + DAYOFYEAR(d)  = 日期 d 是本年的第几天
    + QUARTER(d)    = 日期 d 是本年的第几季; 1表示春季; 2表示夏季
    + DAYOFMONTH(d) = 日期 d 是本月的第几天
    + WEEKDAY(d)    = 日期 d 是本周的第几天; 0表示星期一; ~ 1表示星期二
    + DAYOFWEEK(d)  = 日期 d 是本周的第几天; 1表示星期天; ~ 7表示星期六
    + WEEK(d)       = 日期 d 是本年的第几周;
    + WEEKOFYEAR(d) = 同 WEEK(d)
    + YEARWEEK(d, mode) = 日期 d 的年份和第几周，mode 中 0 表示周天，1表示周一
    + EXTRACT(type FROM d) = 从日期 d 中获取指定的值，type 指定返回的值
    + DAYNAME(d)    = 日期 d 是本周的第几天的名称; 如 Monday / Tuesday;
    + MONTHNAME(d)  = 日期 d 是本年的第几月的名称; 如 November;
    + TO_DAYS(d)    = 日期 d 距离 0000 年 1 月 1 日的天数
4. 时间日期生成
    + LAST_DAY(d)   = 生成日期 d 同月份的最一天日期
    + FROM_DAYS(n)  = 生成从 0000 年 1 月 1 日开始 n 天后的日期
    + MAKEDATE(year, day-of-year)    = 生成日期; 年份 和 本年的第几天
    + MAKETIME(hour, minute, second) = 生成时间
    + SEC_TO_TIME(s) = 通过 s 秒生成时间 t
    + DATE(expression)  = 提取传入表达式的日期部分
    + TIME(expression)  = 提取传入表达式的时间部分
    + DATE_FORMAT(d, format) = 格式化显示日期 d
    + TIME_FORMAT(t, format) = 格式化显示时间 t
    + STR_TO_DATE(string, format) = 将日期字符串 格式化 为日期
    + TIMESTAMP(expression, interval) = 单个参数时，函数返回日期或日期时间表达式；有2个参数时，将参数加和

## 3.1 type 说明
MICROSECOND
SECOND
MINUTE
HOUR
DAY
WEEK
MONTH
QUARTER
YEAR
SECOND_MICROSECOND
MINUTE_MICROSECOND
MINUTE_SECOND
HOUR_MICROSECOND
HOUR_SECOND
HOUR_MINUTE
DAY_MICROSECOND
DAY_SECOND
DAY_MINUTE
DAY_HOUR
YEAR_MONTH

# 4. 其他函数
BIN(x) = 求二进制编码
BINARY(s) = 转化为二进制字符串; 用于区分大小写的字符串比较
CASE WHEN THEN ELSE END = 分支语句
CAST(x AS type) = 强制类型转换
COALESCE(expr1, expr2....) = 返回参数中的从左往右的第一个非空表达式

CONV(x,f1,f2) = 返回 f1 进制数变成 f2 进制数; `select CONV('af', 16, 10);`
CONVERT(s USING cs) = 函数将字符串 s 的字符集变成 cs

IF(expr,v1,v2)   = 如果表达式 expr 成立，返回结果 v1；否则，返回结果 v2
IFNULL(v1, v2)   = 如果 v1 的值不为 NULL，则返回 v1，否则返回 v2。
NULLIF(s1, s2)   = 如果字符串 s1 与 s2 相等 返回 NULL，否则返回 s1
ISNULL(expr)     =  判断表达式是否为 NULL

LAST_INSERT_ID() = 返回最近生成的 AUTO_INCREMENT 值
CURRENT_USER()   = 返回当前用户
SESSION_USER()   = 返回当前用户
SYSTEM_USER()    = 返回当前用户
USER()           = 返回当前用户
CONNECTION_ID()  = 本次连接 uuid
VERSION()        = 返回数据库的版本号
DATABASE()       = 返回当前数据库名

## 4.1 字符集
utf-8
gbk