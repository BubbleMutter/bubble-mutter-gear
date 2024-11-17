[TOC]
# kernel module sign
```sh
# convert private key to DER format (kernel module sign)
openssl req -new -x509 -key /var/lib/dkms/mok.key -out mok.der -outform DER -days 36500 -subj "/CN=dkms module sign key"

# maunal sign module
# (/usr/src/linux-$ver/signing_key.x509 in kernel 4.3.3 or lower)
perl /usr/src/linux-$ver/scripts/sign-file sha512 /usr/src/linux-$ver/kernel-signkey.priv /usr/src/linux-$ver/kernel-signkey.x509 xxx.ko
# (/usr/src/linux-$ver/certs/signing_key.pem in kernel 4.3.3 or higher)
/usr/src/linux-$ver/scripts/sign-file sha512 /usr/src/linux-$ver/certs/signing_key.pem /usr/src/linux-$ver/certs/signing_key.x509 xxx.ko

# check module signature
# will be end up with a string "~Module signature appended~"
hexdump -C xxx.ko | tail

# remove module signature
# NOTE: according to signature mechanism,
#       the signature placed in elf::debug::section
strip --strip-debug xxx.ko
```
1. reference
    1. https://wiki.gentoo.org/wiki/Signed_kernel_module_support
    2. https://wiki.ubuntu.com/UEFI/SecureBoot/DKMS
2. experience
    1. dkms sign kernel module causes frustrating issue.
       Removing dkms module signature may be a good idea.
3. for manually create key, reference `certs/Makefile`
