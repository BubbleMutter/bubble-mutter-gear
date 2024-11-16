[TOC]
# dkms
```sh
# make tar dkms driver
dkms mktarball driver/0.0.1 --source-only

# load tar dkms driver
dkms ldtarball driver-0.0.1-source-only.dkms.tar.gz

# install
dkms install driver/0.0.1 -k $kernelver

# -c specify path to dkms.conf
dkms -c path-to-dkms.conf
```

# dkms mktarball --dkmstree
```sh
tmpdir=$(mktemp -d)
srcdir=$tmpdir/$module/$version/source

mkdir -p $srcdir
cp dkms.conf $srcdir
cp Makefile $srcdir
cp src-xxx  $srcdir

dkms mktarball $module/$version --source-only --dkmstree=$tmpdir --archive=./$module-$version.dkms.tar.gz

rm -rf $tmpdir
```
1. 使用临时目录构造 tarball
2. 必须符合 `$module/$version/source` 路径, 其中
	1. dkms.conf Makefile 是必须的
	2. src-xxx 是驱动实现代码的目录

# dkms install modules for all built kernel
```sh
#!/bin/bash
dkms --version || exit 1
for kernelver in $(ls -1 /lib/modules/); do
	echo $kernelver
	dkms install $module/$version -k $kernelver
done
```

# dkms.conf
```sh
BUILT_MODULE="$dkms_tree/$module/$module_version/$kernelver/$arch/module"
```
1. dkms.conf 将会以 shell 脚本的形式被解析
2. `BUILT_MODULE` 是自定义变量
	+ 使用内建变量构造已构造的 ko 路径
	+ 实际上的 ko 在 `$BUILT_MODULE/*.ko` 中

# module sign
```sh
# signature algorithm must be =y
cat .config | grep -E "CRYPTO_SHA1=|CRYPTO_SHA512=|CRYPTO_SHA256="
```
1. dkms may using variant algorithm to sign module
2. the sign-algorithm must be builtin to support checking
3. so, make all sign-algorithm builtin is a good idea
