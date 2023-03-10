[TOC]
# api
getpid getppid
getenv putenv setenv unsetenv environ[]
setjmp longjmp
sysconf pathconf fpathconf
uname
ptrace
prctl
fork vfork
_exit exit atexit
wait waitpid waitid wait3 wait4
execl execlp execle execv execvp execve execvpe execveat fexecve
system clone
getrlimit setrlimit
getuid geteuid setuid seteuid getgid getegid setgid setegid getlogin
setreuid setregid getresuid getresgid setresuid setresgid
setfsuid setfsgid
getgroups setgroups initgroups
getpriority setpriority

## longjmp setlongjmp 的rollback问题
+ longjmp 的第二个参数 val 作为 setlongjmp的返回值
+ 自动变量/register 变量不会rollback
+ global/volatile/local-static 会被rollback
+ 为了保证 longjmp 后数据正确, 自动变量用 volatile
+ 而且 如果setlongjmp函数已返回 longjmp后栈上变量undefined

## longjmp 实现 try catch 例子
```c++
jmp_buf mjmp_buf;
if(setjmp(jmp_buf) {
    // 如果出错则
    longjmp(mjmp_buf, -1);
} else {

}
```

## fork
+ fork 不继承的
    + file locks 会被child清除
    + pending alarm 会被child清除
    + pending signals 会被child清除
+ fork 继承很多 值得注意的是
    + memory mappings
    + attach shm
    + O_CLOEXEC for any open file descriptors
+ vfork
    + vfork后子进程 运行在父级进程的 空间中
    + 父进程阻塞 直到 子进程exec
+ child的直接父proc退出后 (不会递归第往上的)
    + child 的直接父级 就会变成 init(pid == 1)
    + 任何没有wait的zobime将会在init-proc被wait
    + 所以 大程序懒得处理zobime 可以fork两次后用孙进程 执行业务 

### wait族
+ wait相当于 简化的waitpid `waitpid(-1, &status, 0);`
+ 理论上 zobime进程有 pid status cpu-usage 三个信息
+ 实际上 应该fork进程后 对每个pid 注册on_wait回调
    + wait 收到对应的pid则 找到对应的on_wait回调
#### wait 后的status
+ 判断方式 `if (WIFxxxx(status))`
+ WIFSTOPPED / WIFCONTINUED
+ WIFEXITED 正常退出
    + `WEXITSTATUS(status)` 获取return值
+ WIFSIGNALED
    + `if(WTERMSIG(status) == SIGxxx)` 获取退出的信号
    + `if(WCOREDUMP(status))` 检查由于 coredump导致的
#### waitpid
+ 参数1 pid
    + pid > 0 指定child
    + pid == 0  gid相等的child
    + pid == -1 所有child
    + pid < -1  gid等于 -pid的child
+ 参数3 option
    + WNOHANG 非阻塞, 无child则return 0
    + WCONTINUED
    + WUNTRACED un-traced
#### waitid(idtype, id, siginfo, options)
+ idtype: P_PID/P_PGID/P_ALL
    + P_PGID 同gid的child
+ id: pid 或 gid
+ options
    + WNOHANG 非阻塞
    + WNOWAIT 使得child保持zobime状态, 继续被wait
    + WSTOPPED WCONTINUED WEXITED 至少有一个
#### wait3 wait4
+ 获取 child 的 struct rusage
+ 用以 得到CPU消耗情况 getrusage 解析

### exec 族 ('l' VS 'v') (有无 'p') (有无'e') 共9个
+ 基本最小集 execl execlp execle execv execvp execve
    + 后来添加了3个: execvpe execveat fexecve
+ execl execlp execle execv execvp execve execvpe execveat fexecve族
    + 有'p' 指继承 path, 第一个参数如果不以 / 开头则 搜索path
    + 无'p' 则指 绝对路径 相对路径 当前路径下(不以/开头) 搜索文件
    + 'l' arg的表示形式 arg1, arg2, NULL
    + 'v' arg的表示形式 arg[] (最后元素必须是 NULL)
    + 'e' 表示临时的环境变量数组evnp[] 以NULL结尾
+ 没有execlpe的 有execevat所以共8个
+ exec没有fork 替换了进程空间的 { text/data/heap/stack } segments

### uid gid 详解 (real effective saved)
+ 读写文件时 只会看进程的 effective-uid
+ saved-uid 当S_ISUID标记时exec的进程 saved-uid会改成文件所有者
    + 否则会直接继承 effective
+ superuser才有权限 setuid setgid 影响三者
+ 普通用户 setuid setgid 要么是自己 要么是 saved-uid
+ 注意 saved-uid的程序中 如果调用system() 要在调用前设置euid
    + 否则system()的进程会继承 saved-uid (安全漏洞)
+ 例子:
    + login(1)  进程是root进程 无saved-uid 登录成功后 setuid
    + passwd(1) 是saved-uid root 有权限改 /etc/password
    + su(1)     是saved-uid root 有权限改 uid
    + sudo(1)   是saved-uid root 有权限 fork setuid execv
    + strace ./a.out   strace先fork后exec
    + $>_ ./invalid.sh shell { 先open&read 然后解析} 或 { fork后exec }

### pgid ppid sid (man 7 credentials)
+ process-group-id 即 leader-process-id (领导进程的 进程组ID == 自己pid)
  + 进程fork后, 默认地pgid 进程 parent-pgid
  + 在shell中 启动的 首个进程 pgid 会被设为该进程本身
  + pgid 可以重置 ppid 不可简单地重置(杀死父级进程 ppid 变为1)
  + 所以 pgid 存在意义之一: 用于通过PGID 群发信号 比如sh的pipe
+ 每一个shell 为一个session (sid == pid == pgid)
  + 通过login 或 xterm启动的首个shell sid == pid
  + 次级启动的shell sid != pid
  + 一个session有多个 group
  + 同一时刻  只能有一个或零个group 在foregroud (Ctrl-z Ctrl-/)
  + SIGINT (Ctrl-c) 只会send to foreground-group
  + 所以 pgid 存在意义之一: 用于shell job-controls
+ exec 之后不能再setpgid/setpgrp
+ setpgid 新的groupid 必须在同一个session中
+ bash 才支持 session; sh 不支持