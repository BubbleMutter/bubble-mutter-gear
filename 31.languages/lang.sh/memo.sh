#!/bin/bash

# imagemagick, convert yuv to png
convert -size 640x480 -depth 8 -sampling-factor 4:2:2 yuv:image.raw image.png

# wget ftp 文件夹
wget -r -l 9 -np -nH --cut-dirs=4 ftp://192.168.100.159/a/b/c

# most common and simpler diff
sdiff -s file1 file2

# compare two directory
# -u 输出格式
# -q 不打印 不同的内容，仅显示不同
# -r 递归
# -N 显示仅在一个目录中存在的文件
# -a 对非text文件 也是 line-by-line 地比较
diff -uarNq dir1 dir2

# ping add timestamp
ping 115.239.211.112 | awk '{ print $0"\t" strftime("%Y-%m-%d %H:%M:%S",systime())}'