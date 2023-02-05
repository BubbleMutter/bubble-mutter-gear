+ 信号 / 回调
+ 注册回调 同步等待 异步等待 发送 中断阻塞IO
+ 注册 sigaction signal(这个只是一个简单封装)
+ 发送 raise kill alarm abort killpg tgkill
    + sigqueue 发送携带一个sigval_t数据段
    + pthread_kill pthread_sigqueue 针对线程级的
+ 异步等待 pause sigsuspend
+ 同步等待 sigwaitinfo/sigtimedwait/sigwait signalfd
+ sigaltstack
+ sigreturn
+ 信号mask: pthread_sigmask sigprocmask sigpending
+ sigset_t: fsigvec sigmask sigblock sigsetmask siggetmask
+ sigset_t: sigemptyset sigfillset sigaddset sigdelset sigismember
+ siglongjump sigsetjump 比原始的多了一个参数 为了jump后保留sigmask
+ signal描述: psignal psiginfo strsignal sig2str str2sig

### Basic
+ 信号最小粒度是线程
+ 进程出错(如内存溢出 强行执行text段) 内核通过信号终止进程
+ fork会继承(mask 和 action) execve会重置(mask 和 action)
+ fork会重置 pending信号
+ threads
    + 每个threads有其自身的 signal-action map
    + 对于process级的信号 内核优先选择un-sig-blocked的线程
    + 当多个threads un-sig-blocked 则随机发送
    + 但 同一process中 一个信号只会被一个thread接受
+ threads 独立mask

### Adavanced
+ map signal-num to signal-string (a portable way)
+ WIFSTOPPED WIFCONTINUED 如何触发
+ tell_wait_init/tell_parent/tell_child/wait_child/wait_parent
    + 通过SIGUSR1 SIGUSR2 进行通讯
+ system() ignore SIGINT and SIGQUIT and block SIGCHLD

### Actions (Term/Ign/Core/Stop/Cont)
+ Core指 Term后 dump core

### signals (30+)
+ SIGKILL SIGSTOP 这两个信号不能捕捉 不能注册回调
##### 按动作分类
+ Term: SIGHUP SIGINT SIGKILL(9) SIGTERM
    + 一般会自动接: SIGALRM(14) SIGUSER1 SIGUSER2 SIGPOLL
    + SIGVTALRM: 虚拟闹钟
+ Core: SIGQUIT SIGILL SIGABRT(6) SIGFPE SIGSEGV
    + SIGBUS: bad memory access
    + SIGSYS: bad system call
    + SIGTRAP: trace/breakpoint trap (ptrace gdb要用它)
    + SIGXCPU: setrlimit后cpu超时
    + SIGXFSZ: setrlimit后 文件大小超出了
+ Ign: SIGCHLD
    + SIGURG: socket的urgent数据
+ Cont: SIGCONT 
+ STOP: SIGSTOP SIGTSTP

### 中断阻塞IO
+ 信号回调设置了 SA_RESTART 则会使得部分 阻塞IO重启
    + 否则 errno == EINTR
+ 阻塞IO (可以通过SA_RESTART重启 )
    + read readv write writev ioctl
    + open(fifo)
    + wait waitpid waitid wait3 wait4
    + accept connect recv recvform send sendto recvmsg sendmsg
    + flock fcntl(F_SETLKW F_OFD_SETLKW)
    + pthreads 除了spin外的锁
+ 阻塞IO (不可重启 必返回EINTR)
    + semaphore: sem_wait sem_timedwait
    + pause sigsuspend sigtimedwait sigwaitinfo
    + multiplex-io: epoll_wait poll select
    + IPC-信息队列: msgrcv msgsnd
    + sleep clock_nanosleep nanosleep usleep
    + read一个 inotify的fd

### 注册详解
```c++
int sigaction(int signum, const struct sigaction *act,
                struct sigaction *oldact);
struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask;
    int        sa_flags;
    void     (*sa_restorer)(void);
};
```
+ sa_handler 和 sa_sigaction有时是union
    + sa_handler 可传入 SIG_DFL SIG_IGN 而不是函数
+ sa_restorer 不一定有 配合sigreturn
+ sa_flags
    + SA_SIGINFO: 声明后才能用 sa_sigaction 否则只能用 sa_handler
    + SA_NODEFER: 避免多重信号中断 (在一个回调中 异步跳转到另一个回调)
    + SA_NOCLDSTOP: 仅SIGCHLD 不接受child的信号(SIGSTOP SIGCONT)
    + SA_NOCLDWAIT: 仅SIGCHLD 不处理zombies进程 直接销毁 即wait不了
    + SA_NODEFER: (SA_NOMASK废弃)
    + SA_ONSTACK: 跟sigaltstack一起用
    + SA_RESETHAND: (SA_ONESHOT废弃)
    + SA_RESTART: 重新开启大部分 阻塞IO
+ sa_mask 使得该handler运行时 不会被指定的信号 中断 (阻隔信号嵌套)
    + 这个值必须初始化 sigemptymask 否则安全隐患很严重的
    + 无法阻隔 SIGKILL(kill -9) SIGSTOP
```c++
siginfo_t {
    int      si_signo;     /* Signal number */
    int      si_errno;     /* An errno value */
    int      si_code;      /* Signal code */
    sigval_t si_value;     /* sigqueue 发送信号携带 数据段 */
    /* more and more */
};
union sigval {
    int   sival_int;
    void *sival_ptr;
};
```

### 发送详解
+ `alarm(0);` 清楚pending alarm
    + 与setitimer共用一个进程的全局timer
+ killpg 发送信号到进程组中的 每个进程
    + 传入1会报错 因为1是init进程
    + init进程为1是了区分 fork() == 0

### 信号mask详解
+ 每个进程有个全局的 mask (sigprocmask获取SIG_SETMASK)
+ 当收到信号时 检测mask是否有, 没有则传递到进程中
+ sigaction sa_mask指 sigaction调用前 先标记sa_mask
    + 从而使得该回调期间 实现屏蔽 其他信号黑名单

### 实现Jobs控制
+ SIGCHLD SIGCONT SIGSTOP SIGTSTP SIGTTIN 


[TOC]
+ resource:
    1. man 7 signal

A child created via fork(2) inherits a copy of its parent's signal dispositions.
 During an execve(2), the dispositions of  handled  signals  are reset to the default; the dispositions of ignored signals are left unchanged.

1. default dispositions (5个)
    1. Term  干掉            Default action is to terminate the process.
    2. Ign   无动作          Default action is to ignore the signal.
    3. Core  干掉并生成core  Default action is to terminate the process and dump core (see core(5)).
    4. Stop  暂停            Default action is to stop the process.
    5. Cont  继续如果已经暂停 Default action is to continue the process if it is currently stopped.
2. `fork()` 和 `exec()`
    1. `fork()` 创建的子进程继承父母进程的 dispositions
    2. `exec()` 创建 elf 时, 重置为默认的 dispositions
    3. `clone()` 如果指定了 CLONE_SIGHAND 才会使得子进程共享父进程的信号回调
3. 信号要么被捕捉; 要么被重新设置其他default dispositions
4. 不允许被捕捉的信号: SIGSTOP SIGKILL SIGBUS
    1. 应用: 一般进程内部捕获 SIGTERM 被正常干掉; 进程间先SIGTERM礼貌性干掉对方; 如果干不掉, 那就用SIGKILL/SIGBUS强行干掉

常用的 "标准信号" 解析
SIGHUP        1       Term    Hangup detected on controlling terminal or death of controlling process
SIGINT        2       Term    终端终止进程 Ctrl-c
SIGQUIT       3       Core    Quit from keyboard
SIGILL        4       Core    Illegal Instruction
SIGABRT       6       Core    Abort signal from abort(3)
SIGFPE        8       Core    Floating-point exception
SIGKILL       9       Term    Kill signal
SIGSEGV      11       Core    Invalid memory reference
SIGPIPE      13       Term    write 到 close 的 fd; 实际上这个可以ign; 根据 write 调用的返回值进行错误处理
SIGALRM      14       Term    Timer signal from alarm(2)
SIGTERM      15       Term    Termination signal
SIGUSR1   30,10,16    Term    User-defined signal 1
SIGUSR2   31,12,17    Term    User-defined signal 2
SIGCHLD   20,17,18    Ign     子进程暂停 or 挂掉; 场景: 通过该信号监管子进程
SIGCONT   19,18,25    Cont    Continue if stopped
SIGSTOP   17,19,23    Stop    终端停止进程 Ctrl-z
SIGTSTP   18,20,24    Stop    Stop typed at terminal
SIGTTIN   21,21,26    Stop    Terminal input for background process
SIGTTOU   22,22,27    Stop    Terminal output for background process