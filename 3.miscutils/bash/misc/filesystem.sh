#!/bin/bash

# tmpfs
mount -t tmpfs -o size=512m tmpfs /run/workspace

# cpio with "-u" will allow to overwrite existed files
find . -depth | cpio -Bvo > backup.cpio # backup current dir
cpio -Bvmdi < backup.cpio               # extract into current dir
cpio -Bvmdi -D target/ -F backup.cpio   # extract into target dir
cpio -Bvdt -F backup.cpio               # ls backup.cpio

# android img
# make_ext4fs with -s generate Android-sparse-image
make_ext4fs -l 512M -s        rootfs.img rootfs/ # 512M means size
make_ext4fs -l 3G -s -L linux rootfs.img rootfs/ # for linux rootfs
simg2img sparse.img new_ext4.img # convert Android-sparse-img into ext4

# filesystem label
ntfslabel $label /dev/xxx # change ntfs fs volume label
e2label   $label /dev/xxx # change ext2/3/4 fs volume label
swaplabel $label /dev/xxx

# 文件合并
dd if=i.img of=o.img oflag=append conv=notrunc
# construct file with content
dd if=/dev/urandom of=output.img bs=32M count=6 # urandom 就是 32M 为block的, 不能调这个值
# construct file without content
truncate -s 1024 file.txt