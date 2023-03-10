https://www.kernel.org/doc/html/latest/dev-tools/gcov.html
# 内核 gcov 选项
CONFIG_GCOV_KERNEL=y                // recommanded
CONFIG_GCOV_FORMAT_AUTODETECT=y     // recommanded
CONFIG_GCOV_PROFILE_ALL=y           // for the entire kernel; not suggested

GCOV_PROFILE_main.o := y   // Makefile, 指定文件
GCOV_PROFILE := y          // Makefile, 指定目录
GCOV_PROFILE_main.o := n   // Makefile, CONFIG_GCOV_PROFILE_ALL=y 时, 屏蔽单个文件
GCOV_PROFILE := n          // Makefile, CONFIG_GCOV_PROFILE_ALL=y 时, 屏蔽单个目录


# 用户态工具
```sh
# 分析的机子
lcov -c -d ./path/to/gcda/ -o xxx.info
genhtml  xxx.info -o xxx.html # 生成 html
lcov -l  xxx.info # 简单查看
```

内核Makefile: 如果是 CONFIG_GCOV_KERNEL; 编译.o文件时, 如果开启了gcov, 则加上cflags
```Makefile
ifeq ($(CONFIG_GCOV_KERNEL),y)
_c_flags += $(if $(patsubst n%,, \
		$(GCOV_PROFILE_$(basetarget).o)$(GCOV_PROFILE)$(CONFIG_GCOV_PROFILE_ALL)), \
		$(CFLAGS_GCOV))
endif

CFLAGS_GCOV	= -fprofile-arcs -ftest-coverage -fno-tree-loop-im

# 即 在模块Makefile中可以用
KFLAGS += -fprofile-arcs -ftest-coverage -fno-tree-loop-im
```

# gather gcda from /proc/sys/debug
1. 因为 procfs 技术, 直接 cp 是取不出来的; 要用 cat
```sh
mount -t debugfs nodev /proc/sys/debug/

GCOV_PROC=/proc/sys/debug/gcov/usr/src/bcm-knet-dkms/
GCOV_DEST=/root/bcm-knet

mkdir -p "$GCOV_DEST"
find $GCOV_PROC -name "*.gcda" | while read gcda; do
  cat $gcda > $GCOV_DEST/$(basename $gcda)
done

find $GCOV_PROC -name "*.gcno" | while read gcno; do
    cp -d $gcno ${GCOV_DEST}/
done
tar cf ${GCOV_DEST}.tar -C $(dirname ${GCOV_DEST}) $(basename ${GCOV_DEST})
```

## 用户态 执行gcov
CFLAGS  += -g -O0 -fprofile-arcs -ftest-coverage
LDFLAGS += -lgcov



## gather script
```sh
src="/proc/sys/debug/gcov"
dst="./gcov"

find $src -type d | while read path; do
    mkdir -p $(echo $path | sed "s|^$src/|$dst/|")
done

find $src -type f | while read path; do
    cat $path > $(echo $path | sed "s|^$src/|$dst/|")
done

find $src -type l | while read path; do
    cp -a $path $(echo $path | sed "s|^$src/|$dst/|")
done

tar cf ${dst}.tar $dst
```