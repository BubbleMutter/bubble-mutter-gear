[TOC]
# systemd rules

# directory priority
> `/etc/udev/rules.d`
> `/run/udev/rules.d`
> `/lib/udev/rules.d`
> `/usr/local/lib/udev/rules.d`
> `/usr/lib/udev/rules.d`

1. /lib, /usr/local/lib, /usr/lib 是 package 安装的, 重装包将导致覆盖
2. /etc 下是用户手动创建, 优先级最高, 即相同文件名时, 以 /etc 下为准
3. disable 某个 /lib 中的 rules: `echo " " > /etc/udev/rules.d/同名.rules`
    + 官网上用 `ln -f /dev/null` 的方式; 这个可能有风险

# key: RUN program 所在路径 /usr/lib/udev/
```log
ls -1 /usr/lib/udev/

bridge-network-interface
cdrom_id
hwclock-set
ifupdown-hotplug
```
1. 这些 scripts or elf 可以作为 RUN 的参数

# example: ifupdown when add or remove
```ini
SUBSYSTEM=="net", ACTION=="add|remove", RUN+="ifupdown-hotplug"
```
+ RUN 示例: 当网卡新增/移除, ifconfig up/down
