# dynamic printk
```sh
# enable pr_debug for specific file
echo 'file kernel/module.c +p' > /sys/kernel/debug/dynamic_debug/control
# disable pr_debug for specific file
echo 'file kernel/module.c -p' > /sys/kernel/debug/dynamic_debug/control
```

# kernel archive
```sh
# download images
wget 'http://mirrors.ustc.edu.cn/kernel.org/linux/kernel/v4.x/linux-4.9.324.tar.xz'
```

# build kernel on debian
```sh
# install tools
apt install bc libssl-dev debhelper dpkg-dev libncurses-dev rsync dkms rpm

cd /usr/src
# download
wget 'http://mirrors.ustc.edu.cn/kernel.org/linux/kernel/v4.x/linux-4.14.289.tar.gz'
tar xf linux-4.14.289.tar.xz
cd linux-4.14.289
cp /boot/config-xxx ./.config
# upgrage config
make oldconfig
# remove sign.key (or generate one)
sed -i 's|debian/certs/debian-uefi-certs.pem||' .config
# build
make bindeb-pkg -j4
# check
ls -l /usr/src/*.deb
```
1. linux-image-xxx.deb: 内核镜像文件
2. linux-headers-xxx.deb: 内核头文件 `/lib/modules/xxx/`
+ `CONFIG_MODULE_COMPRESS=n` 这个选项必须关掉; `make bindeb-pkg` 太多坑了
    + 如果开启, 则需要在 模块安装后, 先根据压缩算法解压, strip -S, 重签名, 再根据压缩算法加压
    + 这套逻辑没有有效实践 (2022/08/30)

# build kernel on centos
```sh
# install tools
yum install openssl-devel

# build kernel
# NOTE: make binrpm-pkg will build kernel-devel-xxx.rpm
make rpm-pkg -j4
```
1. kernel-xxx.rpm: 内核二进制文件
2. kernel-devel-xxx.rpm: 内核头文件 `/lib/modules/xxx/`
3. kernel-headers-xxx.rpm: 用户态头文件 `/usr/include/linux/`
    + centos 的 headers 跟 debian 的 headers 不一样, 注意区分

## scripts/package/Makefile; scripts/package/mkspec
1. 原生的 binrpm-pkg 的坑 (rpm-pkg 坑更多了, 不要用)
    1. version 号总是递进
    2. 不编译 kernel-devel 包
    3. 总是单核编译
2. 基本运行机制
    1. scripts/package/Makefile 的 binrpm-pkg 项
    2. 调用 scripts/package/mkspec 生成 .spec
    3. 执行 rpmbuild 解析执行 .spec
3. 修改 scripts/package/Makefile
```make
binrpm-pkg: FORCE
	$(MAKE) KBUILD_SRC=                          # <-- del this line
    rm -rf $(objtree)/.tmp_* $(objtree)/.version # <-- add this line
```
4. 修改 mkspec
    1. `%build` 指令使得 binrpm-pkg 也执行 (默认只在 rpm-pkg 才执行)
    2. `%build` 指令去掉 `make clean`; 避免重复编译
    3. `%files` `%build` 指令使得 "kernel-devel" 也执行 (去掉限制条件)
    4. `%install` 指令中 `make modules_install` 后, 补充 strip -S, 重签名. 处理 MODULE_COMPRESS 逻辑
