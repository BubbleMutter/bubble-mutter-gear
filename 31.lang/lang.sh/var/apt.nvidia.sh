#!/bin/bash

# 1.
apt install nvidia-detect
# 2. detect driver package name
nvidia-detect
# 3. install drivers
apt install $driver_pacakge_name
# 4. blacklist nouveau (optional)
echo blacklist nouveau > /etc/modprobe.d/blacklist-nouveau.conf
# 5. copy /etc/X11/xorg.conf from Debian Optimus Tutorial
vim /etc/X11/xorg.conf
# 6. copy /etc/lightdm/display-setup.sh from Debian Optimus Tutorial
vim /etc/lightdm/display-setup.sh # aka `xrandr'
# 7. edit "display-setup-script=" point to display-setup.sh
vim /etc/lightdm/lightdm.conf
# 8. rebuild dkms if nessecary (e.g. after install broadcom-sta)
dpkg-reconfigure nvidia-kernel-dkms
