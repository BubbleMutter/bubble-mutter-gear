[TOC]
vfs 层的抽象是必要: 否则 syscall 需要适配不同 fs 类型, 导致难以维护
# framework
user > glibc > syscall > vfs > { fs1, fs1, fs2, .. } > block > drivers > devices

# filesystem type
## disk filesystem
ext2 ext3 ext4
reiserfs xfs jffs
sysv ufs minix
vxfs vfat ntfs
iso9660 cd-rom udf-dvd
hrfs hfs affs adfs
## network filesystem
bfs coda afs cifs ncp
## inner filesystem
procfs sysfs debugfs devtmpfs tmpfs devpts
## memory filesystem
squashfs overlayfs

# file model
1. 对象关系: object::super_block -> object::inode -> object::dentry -> object::file
2. 方法层次a): syscall -> file_operations -> super_operations
    + 访问文件数据
4. 方法层次b): syscall -> inode_operations -> super_operations
    + 访问文件属性
3. 方法层次c): syscall -> dentry_operations