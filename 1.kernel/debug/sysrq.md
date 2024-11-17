[TOC]
# sysrq-trigger
https://www.kernel.org/doc/html/v4.10/admin-guide/sysrq.html

CONFIG_MAGIC_SYSRQ

# /proc/sys/kernel/sysrq
```sh
/proc/sys/kernel/sysrq
echo 1 > /proc/sys/kernel/sysrq # enable all functions of sysrq
echo 0 > /proc/sys/kernel/sysrq # disable sysrq completely
```

# sysrq-trigger command keys
+ [sysrq-trigger command](https://www.kernel.org/doc/html/v4.10/admin-guide/sysrq.html#what-are-the-command-keys)
    + b = reboot without any sync
    + c = crash dump by null pointer
    + l = show backtrace in all CPUs
    + q = show hrtimers in all CPUs
    + d = show held locks in all CPUs
    + e = send SIGTERM to all processes, except for init.
    + i = send SIGKILL to all processes, except for init.