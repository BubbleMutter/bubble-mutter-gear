[TOC]
# module
1. bus 检测插入设备
    + systemd-udevd.service 根据 `/usr/lib/udev/rules.d/80-drivers.rules`
    + 匹配 `/lib/modules/$(uname -r)/modules.alias.bin` 的 `$bus$vendor...` 加载对应模块
2. `/etc/modules-load.d/modules.conf` 手动添加加载模块
   由 systemd-modules-load.service 解析加载

# 1. 入口 出口
```c++
#include <linux/init.h>

static const char * __initdata mod_name = "mod_name";

static int __init mod_init(void) {
    return 0;
}
static void __exit mod_exit(void) {

}

module_init(mod_init);
module_exit(mod_exit);
```
1. `__init` 和 `__exit` macro 用于区分 linked 和 module 两种形态的编译产物
2. linked 形态:
    1. `__init` 在 boot 阶段执行一次, 形同 initcall 函数
    2. `__exit` 在 整个内核运行阶段不执行, 即不会编译到内核中

# 2. 编译
1. Makefile
    ```Makefile
    CONFIG_MODULE_SIG=n # 避免签名问题
    obj-m:=hello.o
    hello-objs:=helloworld.o

    KERNEL_ROOT=/lib/modules/`uname -r`/build/
    CUR_DIR=$(shell pwd)

    default:
            $(MAKE) -C $(KERNEL_ROOT) M=$(CUR_DIR) modules

    clean:
            rm -rf *.o *.ko *.mod.c .*.cmd .*.cmd.c modules.* Module.* .tmp_versions
    ```
2. 简单编译命令 `make -C $KENNEL_DIRECTORY M=$MODULE_DIRECTORY modules`

# 3. 用户态操作
```s
insmod
rmmod
lsmod
modprobe

ls  /sys/module
cat /proc/modules
```
1. `/proc/modules`

# 4. 传参
```c++
#include <linux/moduleparam.h>
module_param(variable, type, perm);
module_param_array(name,type,num,perm);

static char *whom = "world";
static int howmany = 1;
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);
```
```sh
insmod your_module whom="heaven" howmany=100
```
1. 对应 `/sys/modules/$module/$param`

# 5. 模块间依赖