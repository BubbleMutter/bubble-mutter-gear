# 字符串比较 -------------------------------
# 1. 空字符串
s=""
if [ -z $s ]; then string is empty; fi
s="string"
# 2. 非空字符串
if [ -n $s ]; then string is not empty; fi

# 3. 字符串比较, 不能使用 -eq, -ne, 因为这两个只能用于 整形数值
s1="string_mini"
s2="string_mini"
s3="string_plus"
if [ $s1  = $s2 ]; then s1 and s2 are equal; fi      # 相等比较
if [ $s1 != $s3 ]; then s1 is not equal with s3; fi  # 不等比较
# 字符串比较 -------------------------------

# 字符串长度 ----------------------
s="I am a amazing string"
echo string length is ${#s}
echo string lenght is `echo -n "$s" | wc -c`
# 字符串长度 end ------------------

# trim
echo $string | tr -d '[:space:]'

# gbk2utf8
iconv -f GBK -t UTF-8 $file

# crlf2lf
sed -i -e "s|\r*$||" $file