[TOC]
# call_usermodehelper
内核调用用户态函数

# example
1. `kernel/kmod.c` -> `/sbin/modprobe`
    1. 根据 `/etc/modules-load.d/` 控制模块加载
2. `lib/kobject_uevent.c`