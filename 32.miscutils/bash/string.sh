#!/bin/bash
# string comparation
if [ -z $s ]; then echo empty; fi
if [ -n $s ]; then echo not empty; fi
if [ $s1  = $s2 ]; then echo equal; fi
if [ $s1 != $s3 ]; then echo not equal; fi

# convert gbk to utf8
iconv -f GBK -t UTF-8 $file
# convert utf8 to gb2312
iconv -f gb2312 -t utf-8  -c srcfile.txt > destfile.txt

# crlf2lf
sed -i -e "s|\r*$||" $file

# shell upper/lower string
echo string | tr '[a-z]' '[A-Z]' # upper
echo string | tr '[A-Z]' '[a-z]' # lower
echo string | tr -d '[:space:]'  # trim

# token
cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 32 | head -n 1
uuid             # apt install uuid
pwgen -s $length # generate password

# csv pretty print
cat /path/to/csv | column -t -s,

# hex print
printf %d 0xabcdef    # hex to oct (suggested)
printf %x 15          # oct to hex (suggested)

# sort particular rows and column
# row    <= 3 will not be sorted
# column == 4 will be sorted
awk 'NR<=3 {print;next} {print | "sort -k4" }'

# matrix transform
cat /proc/net/snmp  | sed -n '/^Ip:/p' | rs -T
