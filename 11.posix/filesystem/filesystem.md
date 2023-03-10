[TOC]
# api
mount umount umount2
statvfs fstatvfs
stat lstat fstat fstatat
utime utimes futimes lutimes utimensat futimens
chown fchown fchownat lchown
chmod fchmod fchmodat
access ( F_OK/R_OK/W_OK/X_OK ) 检查
umask
inotify_init inotify_add_watch inotify_rm_watch

## futimens utimensat (传入`timespec[2]`)
    + 第一个元素表示 access time
    + 第二个元素表示 modified time

lstat 不会 follow symbolic-link 而是读取link本身

## stat/fstat/lstat/fstatat (struct stat)
+ lstat 不会follow symbolic-link 而是读取link本身
## struct stat
+ st_mode   即permission & file type
    + 检查类型 S_ISREG/S_ISDIR/S_ISCHR/S_ISBLK/S_ISFIFO/S_ISLINK/S_ISSOCK
    + `((st_mode & S_IFMT) == S_ISUID)`
    + 权限: S_I + {R/W/X} + {USR/GRP/OTH}
+ st_nlink  硬链接的个数 默认最少为1(即自己)
+ st_size   长度
+ st_uid, st_gid
+ st_ino/st_dev/st_rdev
+ st_atim   last access, 包括open/read
+ st_mtim   last 修改时间
+ st_ctim   last file status change

## 特殊权限 S_ISUID S_ISGID
+ S_ISUID/S_ISGID 使得任何用户执行 该文件时
    + 进程的 effective-uid/gid 继承文件, 而非用户
    + 例如 /usr/bin/passwd 任何用户执行都有权限修改 /etc/password
    + ls -l 时 S_ISUID 会使得第一个x变成s 即chmod u+s
    + ls -l 时 S_ISGID 会使得第二个x变成s 即chmod g+s
+ S_ISVTX (sticky-bit) /tmp 目录的权限, 仅对目录有用

## umask
+ 使得进程创建文件时 去掉具体权限
+ 命令行工具umask 加参数表示默认文件权限的mask
    + umask -S 表示symbolic文件权限

## inotify
+ 原理 + 应用 + 限制 + 注意 + 可以监听那些事件 + 不可监听哪些事件
+ limit:
  size: fd限制, 事件buffer大小限制, 不会递归监听文件夹
  无法分辨 是 当前进程触发的 or 其他进程触发的
  无法监听mmap(2), msync(2), and munmap(2)
  每一个监听文件都是基于fd的, 需要手动维护 filename:fd 的映射
  只能监听用户态的
  fallocate(2) 3.19 才有
+ inotify本身就是以fd作为句柄
  epoll poll select可以监听 inotify句柄
+ event:
  exec
  file content read/write/truncate
  meta (setxattr chmod chown)
  signal-driven io(O_ASYNC F_SETSIG F_SETOWN)
  fd的直接修改 (ioctl fcntl)
  rename() IN_MOVE_FROM IN_MOVE_TO
+ rename() 接口需要特别注意
+ 递归监听文件夹
  若子文件夹 重命名, 需要close重命名前递归监听的fd