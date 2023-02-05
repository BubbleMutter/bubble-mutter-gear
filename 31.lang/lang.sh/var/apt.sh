# debian install ppa ----------------
# 1. add http://url/inside/ppa/webpages to /etc/apt/source.list
# 2. query keys from server. The key is inside ppa pages:
#    find something like `Fingerprint: $uuid'
# example
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys  43D3A9F60C58A7169778E6FB8771ADB0816950D8

# 3. then apt update
# 4. finally you can install package on ppa
# -----------------------------------

# control package auto or manual
apt-mark auto $package
apt-mark showauto $package
apt-mark manual $package
apt-mark showmanual $package
# -----------------------------------

# update initramfs after install driver
update-initramfs -u
# -----------------------------------

# update grub2
update-grub2
# -----------------------------------