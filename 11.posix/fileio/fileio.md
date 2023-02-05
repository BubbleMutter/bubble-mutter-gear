[TOC]
# api
open  close  read  write  truncate  lseek fsync fdatasync sync
fopen fclose fread fwrite ftruncate fseek fflush
fseek ftell rewind fgetpos fsetpos fseeko ftello
clearerr feof ferror fileno
dup dup2 dup3
fileno fdopen
fcntl ioctl
setbuf setbuffer setline setvbuf
pread pwrite
readv writev

rewind 形同 fseek(fp 0L, SEEK_SET)
fileno 返回 FILE 的 fd, 该功能可用来检测FILE是否有效

## open flags
+ creat => O_CREAT/O_WRONLY/O_TRUNC
+ O_RDONLY/O_WRONLY/O_RDWR/O_TRUNC/O_APPEND
+ O_CREAT/O_EXCL 当不存在则创建
    + O_CREAT  当不存在则创建
    + O_EXCL   与O_CREAT一起 如果不能创建就报错 作为原子操作(线程安全)
    + 使用access + O_CREAT 不能保证 原子操作
+ 阻塞 O_NONBLOCK/O_NDELAY/O_SYNC/O_DSYNC
+ 目录 链接 O_PATH/O_DIRECTORY/O_NONFOLLOW
+ O_DIRECTORY 不是目录即失败 配合fdopendir使用
+ O_NOFOLLOW 不跟随 link, 遇到 link 则失败
+ O_CLOEXEC 该fd exec后则关闭 => fcntl(fd, FD_SET, FD_CLOEXEC)
    + 这是原子操作 (open + fcntl 不是原子操作)
## fopen-mode 对应 open-flag
+ w a 自带 O_CREAT
"r" : O_RDONLY
"w" : O_WRONLY | O_CREAT | O_TRUNC
"a" : O_WRONLY | O_CREAT | O_APPEND
"r+": O_RDWR
"w+": O_RDWR | O_CREAT | O_TRUNC
"a+": O_RDWR | O_CREAT | O_APPEND
## dup dup2 dup3
+ dup 拷贝fd 共享offset/flags (不共享 FD_CLOEXEC close-on-exec)
+ dup不指定 newfd, 系统自动返回lowest的
    + 返回newfd
+ dup2 指定 newfd, 如果 newfd 已打开 则原子性 关掉再开
    + close + dup 已打开 newfd 不是原子性的
    + oldfd invalid 失败; 返回 -1
    + oldfd == newfd 不操作 return oldfd
+ dup3 等同dup2
    + 强制设置 O_CLOEXEC (这是原子操作 fcntl则不是)
    + oldfd == newfd 返回错误
+ 可以用 dup2(fd, STDOUT_FILENO) 使得printf重定向到fd
    + 原因是 stdout == 2 指向了虚拟文件 shell把虚拟设备文件重定向到console
    + dup2后 2指向了fd
    + 而printf底层实现write(2, ) 从而使得 printf得到重定向
    + 但是 printf有自己的缓存
      fsync(STDOUT_FILENO)是没用的 
      fflush(stdout)才行
## fsync fdatasync sync 的区别
1. 文件的 metadata 属于文件系统信息 (如: st_time/st_atime 由stat获取)
2. fsync 同步 metadata 形同 fflush() 但 stdio 有它的缓存
3. fdatasync 不同步 metadata

# usage notes
## dup2 实现 shell 调用时 输出重定向
1. open 重定向输出的文件得到 fd
2. `dup2(fd, STDOUT_FILENO)`
3. fork() 或 直接exec
## dup2 实现 shell 调用时 输入重定向
1. open 重定向输入的文件得到 fd
2. `dup2(fd, STDIN_FILENO)`
3. fork() 或 直接exec
## lseek 获取偏移 获取当前文件大小
SEEK_SET / SEEK_CUR / SEEK_END
+ lseek(fd, 0, SEEK_CUR) 获取当前offset
+ lseek(fd, 0, SEEK_END) 获取文件长度
## fnctl <sys/fnctl.h>
+ 复制文件描述符, cmd：F_DUPFD、F_DUPFD_CLOEXEC
+ 文件描述符标志位, cmd：F_GETFD、F_SETFD
+ 文件状态标志, cmd：F_GETFL、F_SETFL
+ 文件建议锁, cmd：F_SETLK, F_SETLKW, and F_GETLK

## 错误控制 clearerr feof ferror
+ clearerr
+ feof 是否到结尾 getter
+ ferror 返回错误码 可由clearerr重置

# 目录IO
+ `<dirent.h>`
+ DIR 
+ struct dirent
#### DIR
{ ino_t d_ino; char d_name[] }
#### struct dirent
+ posix 下只有 d_ino 和 d_name 是必须的
```c++
struct dirent {
    ino_t          d_ino;       /* Inode number */
    char           d_name[256]; /* Null-terminated filename */

    off_t          d_off;       /* Not an offset; see below */
    unsigned short d_reclen;    /* Length of this record */
    unsigned char  d_type;      /* Type of file; not supported
                                    by all filesystem types */
};
```
#### scandir 用法
```c++
DIR *dirp = opendir("path");
struct dirent **result;
int num = scandir(dirp, &result, NULL, alphasort)
```
+ 深度搜索 不使用ftw.h (本质上是前序遍历)
    1. opendir + readdir 遍历path
    2. npath = path + '/' + d_name
    3. stat 判断 npath `(mstat.st_mode & S_IFMT) == S_IFDIR)` 是否为目录
    4. 目录则递归npath (实现深度搜索)
    5. 文件则访问npath
+ 广度搜索不好做的
#### nftw/ftw
+ typeflag 作为遍历函数的参数
    + FTW_F                 一般文件
    + FTW_D/FTW_DNR/FTW_DP  目录相关
    + FTW_NS/FW_SL/FW_SLN   软链接相关
+ ftp 相当于 nftw 的 flags参数为0
+ flags nftw的标记
    + FTW_MOUNT 不cross filesystem
    + FTW_PHYS  不follow symbolic
    + FTW_DEPTH 做后续遍历
    + FTW_CHDIR 对于每个目录FTW_CHDIR