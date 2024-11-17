#!/bin/bash
# download images
wget http://mirrors.ustc.edu.cn/kernel.org/linux/kernel/v4.x/linux-4.9.324.tar.xz
# clone git repos
git clone -b linux-5.15.y --single-branch http://mirrors.ustc.edu.cn/linux.git

# dynamic printk
# enable pr_debug for specific file
echo 'file kernel/module.c +p' > /sys/kernel/debug/dynamic_debug/control
# disable pr_debug for specific file
echo 'file kernel/module.c -p' > /sys/kernel/debug/dynamic_debug/control

# uapi version (not equal to kernel version)
/usr/include/linux/version.h
/usr/src/linux-<version>/include/generated/uapi/linux/version.h

# inherit config
cp /boot/config-xxx ./.config
# upgrage config
make oldconfig

kernel_makedeb() {
    # kernel compile tools on debian
    apt install bc libssl-dev debhelper dpkg-dev libncurses-dev rsync dkms rpm pahole
    # remove sign.key (or generate one)
    sed -i 's|debian/certs/debian-uefi-certs.pem||' .config
    # build
    make bindeb-pkg -j$(nproc)
    # check
    ls -l /usr/src/*.deb
    # linux-image-*.deb    # kernel images  /boot/vmlinuz-*
    # linux-headers-*.deb  # kernel headers /lib/modules/xxx/

    # CONFIG_MODULE_COMPRESS=n 这个选项必须关掉;
    # make bindeb-pkg 太多坑了
}

kernel_makerpm() {
    yum install openssl-devel
    yum install pahole
    # note: make binrpm-pkg will build kernel-devel-xxx.rpm
    make rpm-pkg -j4

    ls -l /usr/src/*.rpm
    # kernel-<version>.rpm:       内核二进制文件
    # kernel-devel-<version>.rpm: 内核头文件   `/lib/modules/xxx/`
    # kernel-headers-xxx.rpm:     用户态头文件 `/usr/include/linux/`
    # note: centos 的 headers 跟 debian 的 -headers- 不一样, 注意区分

<<EOF
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
    4. 修改 scripts/package/mkspec
        1. `%build` 指令使得 binrpm-pkg 也执行 (默认只在 rpm-pkg 才执行)
        2. `%build` 指令去掉 `make clean`; 避免重复编译
        3. `%files` `%build` 指令使得 "kernel-devel" 也执行 (去掉限制条件)
        4. `%install` 指令中 `make modules_install` 后, 补充 strip -S, 重签名. 处理 MODULE_COMPRESS 逻辑
EOF
}

