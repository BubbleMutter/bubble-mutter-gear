#!/bin/bash

# imagemagick, convert yuv to png
convert -size 640x480 -depth 8 -sampling-factor 4:2:2 yuv:image.raw image.png

# fix corupt zsh_history
cat .zsh_history > zsh_history
strings zsh_history > .zsh_history
rm zsh_history

# change ntfs fs volume label
ntfslabel /dev/sdc1 new_volume_name

# change ext2/3/4 fs volume label
e2label /dev/sdc1 new_volume_name

# gentoo lsb sucks
cd /lib
ln -s ld-linux-x86-64.so.2 ld-lsb-x86-64.so.2
ln -s ld-linux-x86-64.so.2 ld-lsb-x86-64.so.3

# `parted` is the only one util to resize disk partition
# sfdisk, cfdisk, gdisk, fdisk, do not support

:<<! qtcreator fcitx-sogou stuff
# need platforminputcontexts/libfcitxplatforminputcontextplugin.so
# in gentoo,        $emerge fcitx-qt5
# in debianLike,    $apt-get install fcitx-frontend-qt5
# needed .so-dir is under <qt5_libs_top>/plugins/platforminputcontexts/
# cp platforminputcontexts/ to <qtcreator_libs_top>/plugins/
# in gentoo, under /usr/lib,  qt5/ and qtcreator/
# in debian, under /usr/lib/<!-- <arch_compiler> -->, qt5/ and qtcreator/
!
# Qt 打包 on Windows7/8/10
windeployqt -release . --no-quick-import  --no-plugins

# shell upper/lower string
echo string | tr '[a-z]' '[A-Z]' # upper
echo string | tr '[A-Z]' '[a-z]' # lower
echo string | tr -d '[:space:]'  # trim

# color shell text
# \e["style";"fore_color";"back_color"m
# fore_color { black:30, red:31, green:32, yellow:33,
#               blue:34, magenta:35, cyan:36, white:37 }
# back_color { black:40, red:41, green:42, yellow:43,
#               blue:44, magenta:45, cyan:46, white:47 }
# style      { reset:0, bold:1, light-color:2,
#               reverse:3, baseline:4 }
echo -e '\e[1;32;44mThis is red bold string.\e[0m'
# remember to set default color \e[37;40m finally

# android img
# make_ext4fs with -s generate Android-sparse-image
make_ext4fs -l 512M -s new_rootfs.img src_dir # 512M means size
make_ext4fs -l 3G -s -L linux new_rootfs.img src_dir # for linux rootfs
# simg2img convert Android-sparse-img into ext4
simg2img sparse.img new_ext4.img

# mv files with a specific suffix in a dir
#   and keeping the dir structure
# for example, *.c files in src/, need to mv to dst/
#   src/ and dst/ are in the same directory
cd src/
find . -depth -name "*.c" | xargs -I {} cp --parents {} ../dst

# cpio with "-u" will allow to overwrite existed files
find . -depth | cpio -Bvo > backup.cpio # backup current dir
cpio -Bvmdi < backup.cpio # extract into current dir
cpio -Bvmdi -D annother_dst_dir -F backup.cpio # more convinient
cpio -Bvdt -F backup.cpio # print the files in the backup.cpio

# generate random number in specific range [21, 50)
echo "`head -c99 /dev/urandom | md5sum \
    | sed 's|[^0-9]||g'` % (50-21) + 21" | bc

# get a random string, digit and alphabet
TOKEN=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 32 | head -n 1)

# mount .img, but not the android-spare-img
partx -av ./file.img
mount /dev/loop2p1 /mnt/one_point
mount /dev/loop2p2 /mnt/two_point
# delete
umount /dev/loop2p1
umount /dev/loop2p2
partx -d /dev/loop2
rm /dev/loop2

# diamond, a fpga develop environmet for lattice
DIAMOND_DIR=/usr/local/diamond/3.8_x64
SO_CONF_FILE=20diamond-x86_64-pc-linux.conf
find ${DIAMOND_DIR} -name "lib"     >   ${SO_CONF_FILE}
find ${DIAMOND_DIR} -name "lin64"   >>  ${SO_CONF_FILE}
mv ${SO_CONF_FILE} /etc/ld.so.conf.d

google-chrome-stable --proxy-server="socks5://127.0.0.1:1080"

CXX_HEADER=/usr/lib/gcc/x86_64-pc-linux-gnu/4.9.3/include/g++-v4
ls ${CXX_HEADER}

# build cross-reference database with cscope
cscope -qkRv

# convert utf-8 to gb2312
iconv -t utf-9 -f gb2312 -c srcfile.txt > destfile.txt

# allow_writeable_chroot=YES in /etc/vsftpd/vsftpd.conf

# ssh proxy server
ssh -fCNR "3000:127.0.0.1:22" server_user@server_ip # local host
ssh -fCNL "*:4000:127.0.0.1:3000" "server_user@127.0.0.1" # proxy server
ssh -p 4000 local_host_user@server_ip # access from outside

# add qt5 include to .clang_complete for vim,
# working with syntastic and clang_complete
find /usr/include/qt5 -maxdepth 1 \
    | sed -e "s|^|-I|; 1i-fPIC" > .clang_complete

# test or check a command whether exist
hash checked_cmd 2>/dev/null && echo cmd exists || echo cmd not exist

# 看一个内网段有多少个ip
ping 192.168.11.255 -c 3 | grep -e 'from *[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}' | awk '{print $4}'| sort | uniq
nmap -sV 192.168.21.186 # 返回http服务器后端类型，lighttpd，apache，nginx
nslookup www.baidu.com  # 查找ip地址
ss -tudlpa # monitor network port

# rsync.password must be chmod 600 rsync.password
chmod 600 rsync.password

# 实现将命令的stdout/stderr 重定向到log.txt
$~（do something 2>&1）>  log.txt

# 稍微分析一下文件的 存储空间分布
du -h -d 2 | sort -h

# wget ftp 文件夹
wget -r -l 9 -np -nH --cut-dirs=4 ftp://192.168.21.159/a/b/c

# compare two directory
diff -uarNq dir1 dir2
# -u 输出格式
# -q 不打印 不同的内容，仅显示不同
# -r 递归
# -N 显示仅在一个目录中存在的文件
# -a 对非text文件 也是 line-by-line 地比较

# for i do  不加in $var, 默认读取的是 $1 $2

# ssh without inactive password
sshpass -v -p "password" ssh -o stricthostkeychecking=no username@hostname "exec cmds"
sshpass -v -f  passfile  scp -o stricthostkeychecking=no args

# tmux with multi-predefiend-commands
pip install smux.py # (not smux) a scripts

# high light cat
apt install source-highlight highlight supercat
spc $file
source-highlight-esc.sh $file
highlight -O ansi $file

astyle -A14 $file # --style=google
astyle -A16 $file # --style=mozilla

function abspath() {
  local base=`basename $1`
  local dir=`cd $(dirname $1); pwd`
  echo $dir/$base
}

# pretty print csv
cat m.csv | column -t -s,

# tmpfs
mount -t tmpfs -o size=512m tmpfs /home/admin/tmp_workspace

# sort specific rows and column
# row <= 3 will not be sorted; column == 4 will be sorted
awk 'NR<=3 {print;next} {print | "sort -k4" }'
ovsdb-client dump Port | awk 'NR<=3 {print;next} {print | "grep \"ge1_3_2\\\"\" " }'

# most common and simpler diff
sdiff -s file1 file2

# convert CRLF to LF
find . -type f | while read file; do sed -i "s|\r$||" $file; done

# 研究了好久的命令
# 实现1. 本地脚本远端执行 2. 远端输出重定向到远端文件 3. 远端输出回显到本地
export ssh_options=" -o ConnectTimeout=1 -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o LogLevel=ERROR"
sshpass -p $pass ssh $ssh_options $user@$addr "sh -x - 2>&1 | tee -a $log" < $task_script

# chroot
mount -t proc proc $chrootfs/proc
mount -t devtmpfs devtmpfs $chrootfs/dev
mount -t devpts devpts $chrootfs/dev/pts

# 批量重命名后缀 .log -> .txt
find . -type f -name "*.log" | while read src; do mv $src $(echo $src | sed "s|\.log$|\.txt|"); done


# ping add timestamp
ping 115.239.211.112 | awk '{ print $0"\t" strftime("%Y-%m-%d %H:%M:%S",systime())}' 

# 获取 thread 的名字; thread 的name 通过 prctl() 设置
ls /proc/$(pidof a.out)/task -1 | while read tid; do cat /proc/$tid/status| grep Name: | awk '{print $2}'; done


# 16 进制
printf %d 0xabcdef    # hex to oct (suggested)
echo $((16#abcdef))   # hex to oct
printf %x 15          # oct to hex (suggested)
echo "obase=16;15"|bc # oct to hex

# 文件合并
dd if=i.img of=o.img oflag=append conv=notrunc

# 构造指定大小的空白文件; 如 1024 bytes
truncate -s 1024 file.txt # 某些文件系统会对空文件进行优化的, 所以这样子不能占大小的

# 构造指定大小的非空文件;
dd if=/dev/urandom of=./file.txt bs=32M count=6 # urandom 在我的系统上是 32M 为block的, 不能调这个值

# 使能笔记本触控板
xinput --enable $(xinput | grep "AT Translated .* keyboard" | sed 's|.*id=\([0-9]*\).*$|\1|')
xinput --enable $(xinput | grep "SynPS/2 .* TouchPad" | sed 's|.*id=\([0-9]*\).*$|\1|')

# telnet 实时日志重定向 ##########
# 永久
( (sleep 1 && echo "dlanrecover" && sleep 2 && echo "set 61 0xffffffff" && while true; do sleep 30; done  ) | telnet 127.0.0.1 1981) > telnet.log.txt
# 限定30秒的
( (sleep 1 && echo "dlanrecover" && sleep 2 && echo "set 61 0xffffffff" &&  sleep 30  ) | telnet 127.0.0.1 1981)  > telnet.log.txt
###################################


uuid             # 生成uuid
pwgen -s $length # 生成密码