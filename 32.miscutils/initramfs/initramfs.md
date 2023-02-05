[TOC]
# initramfs
```sh
# update
dracat /boot/$initrd `uname -r` # method1
update-initramfs                # method2

# show content
lsinitrd /boot/$initrd          # method 1
lsinitramfs /boot/$initrd       # method 2
```
