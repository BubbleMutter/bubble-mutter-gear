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

run_process_background() {
    # 1. nohup
    nohup
    # 2. setsid
    setsid
    # 3. disown
    disown
    # 4. inner
    (setsid scripts.sh &)
}

colorize() {
    # color shell text
    # \e["style";"fore_color";"back_color"m
    # fore_color { black:30, red:31, green:32, yellow:33,
    #               blue:34, magenta:35, cyan:36, white:37 }
    # back_color { black:40, red:41, green:42, yellow:43,
    #               blue:44, magenta:45, cyan:46, white:47 }
    # style      { reset:0, bold:1, light-color:2,
    #               reverse:3, baseline:4 }
    echo -e '\e[1;32;44mThis is red bold string.\e[0m'
    # reset to default color \e[37;40m finally
}
