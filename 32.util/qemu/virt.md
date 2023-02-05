# virsh create a vm
```sh
# provide /usr/share/OVMF/OVMF_CODE.fd for uefi vm
apt install ovmf

# initailize bridge network
./virt-bridge.sh

export IMAGE_NAME=debian-buster
export IMAGE_FILE=debian-buster-uefi.qcow2
```

# virsh manage vm
```sh
virtsh shutdown debian-buster # shutdown
virsh undefine debian-buster  # remove
virsh console debian-buster   # attach console
```

# manage disk.qcow2
```sh
# create an empty images
qemu-img create -f qcow2 -o preallocation=metadata newdisk.qcow2 60G

# clone and expand old image to new image with specific partition
virt-resize --expand /dev/sda2 olddisk.qcow2 newdisk.qcow2

# convert image from preallocation to autoexpand
qemu-img convert -f qcow2 -O qcow2 -o preallocation=off olddisk.qcow2 newdisk.qcow2

# display images partition
virt-filesystems --long -h --all -a disk.qcow2

# display qcow2 info
qemu-img info disk.qcow2
# resive virtual size of disk
qemu-img resize disk.qcow2 +10G
```

# partion expand inside vm
```sh
growpart /dev/sda 1
resize2fs /dev/sda1
```
1. 一般来说系统盘的镜像是压缩过的, 然后系统本身会有很大空间
2. 为了使得运行时 rootfs 把剩余空间利用起来, 先扩容到最大
