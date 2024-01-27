[TOC]
+ quick start: https://www.tecmint.com/bcc-best-linux-performance-monitoring-tools
+ website: https://iovisor.github.io/bcc

# require
1. 内核配置如上; 确认方法 `cat /boot/config-$(uname -r) | grep`
   ```ini
   CONFIG_BPF=y
   CONFIG_BPF_SYSCALL=y
   # [optional, for tc filters]
   CONFIG_NET_CLS_BPF=m
   # [optional, for tc actions]
   CONFIG_NET_ACT_BPF=m
   CONFIG_BPF_JIT=y
   CONFIG_HAVE_BPF_JIT=y
   # [optional, for kprobes]
   CONFIG_BPF_EVENTS=y
   ```
2. 内核版本 4.1 以上
3. 安装 `apt install bpfcc-tools libbpfcc-dev`
4. 用例: `/usr/sbin/*-bpfcc`
    1. `opensnoop-bpfcc -d 1`; trace `open()`
    2. `execsnoop-bpfcc`; trace `exec()`
    3. `biolatecncy-bpfcc`; summarized distribution of disk I/O latency
    4. `ext4slower-bpfcc`; trace the ext4 filesystem slow operation
    5. `biosnoop-bpfcc`; show per disk I/O each second
    6. `cachestat-bpfcc`; trace page cache hit/miss ratio
    7. `tcpconnect-bpfcc`; trace tcp `connect()`

# usage
1. 实现事件回调: 按照例子程序, c 源码
2. 使用 wrapper 安装事件钩子, wrapper 有 c++/lua/python 封装, 用其中之一即可
3. 使用 wrapper 跟踪插桩采样, 打印统计信息
4. c++ 使用:
    1. https://github.com/iovisor/bcc 的 examples/cpp 例子
    2. `#include<bcc/BPF.h>`