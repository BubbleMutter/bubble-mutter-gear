[TOC]
# kdump crash
1. kernel config
```conf
CONFIG_DEBUG_INFO=y
CONFIG_CRASH_DUMP=y
CONFIG_PROC_VMCORE=y
```

2. minimal usage
```sh
apt install kdump-tools
apt install `uname -r`-dbg
crash /path/vmlinux /var/crash/$time_id/dump.file
```
