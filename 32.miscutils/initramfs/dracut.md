[TOC]
# dracut
```sh
dracut -f --install "/usr/bin/strace" --kver $(uname -r)
```
1. add another file

# sign module
```conf
CONFIG_MODULE_SIG=y
CONFIG_CRYPTO_SHA256=m
CONFIG_CRYPTO_SHA512=m
```
1. when kernel config has module signature
2. sha256 and sha512 must be y,
3. otherwise they must be packed in initramfs when they are compled as module.