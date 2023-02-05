# Performance
+ http://www.brendangregg.com/linuxperf.html

# cpu 占用
top -d 1
## usr: 用户态 cpu 占用
## sys: 内核态 cpu 占用
## nic: 不懂
## idle: 空闲

## io:  磁盘读写
1. 一般来说频繁往磁盘写日志
2. 基于于文件的数据库频繁访问
3. 内核的频繁调用 printk
4. 内核硬件操作等待时间长
99. 误区: 内核频繁收包, 不会直接导致 io 高的
98. 误区: "io 高" 与 "cpu 高" 本质上不关联
          存在 io 高, 但是 cpu 很低的场景
          存在 cpu高, 但是 io  很低的场景

## irq:  硬中断
1. 硬件中断频繁触发, 比如网卡收包

## sirq: 软中断
1. 内核频繁收包
2. 存在软中断任务, 锁卡太久了

# 监控平台
https://gitlab.com/k3oni/pydash
https://gitlab.com/k3oni/pydash-django-app
+ python 官方有个 pydash 是 js 库 lodash 的实现; 注意区分
+ pydash-django 平台期望修改一下, 可以通过 ssh 上去远端机子拿数据