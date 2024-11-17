[TOC]
# systemd udevadm
```sh
#!/bin/bash

# test a device pass through rules hypothetically
udevadm test -a add /sys/class/net/eth0
udevadm test -a add /sys/bus/pci/devices/0000:01:00.0

# show the matching key-value in rules
udevadm info -a --path=/sys/bus/pci/devices/0000:01:00.0
udevadm info -q property -p /sys/bus/pci/devices/0000:01:00.0
# reload rules
udevadm control --reload

udevadm trigger -c add /sys/class/net/eth0
udevadm trigger action=add subsystem-match=net --type=devices

SYSTEMD_LOG_LEVEL=debug udevadm test-builtin net_setup_link /sys/class/net/eth0
SYSTEMD_LOG_LEVEL=debug udevadm test-builtin net_id /sys/class/net/eth0
```

# override device's kmodule
```ini
SUBSYSTEM=="pci", ACTION=="add", ATTRS{vendor}=="0x15b3", ATTRS{device}=="0x101*|0xA2D*", \
	RUN{builtin}+="kmod load custom_module", \
	RUN{program}+="/bin/sh -c 'echo %s{vendor} %s{device} > /sys/bus/pci/drivers/custom_module/new_id'"
```
1. place in /etc/udev/rules.d/99-custom-kmodule-backports.rules

# manual
https://www.freedesktop.org/software/systemd/man/systemd.link.html
https://www.freedesktop.org/software/systemd/man/systemd.network.html

# netcard rename priority
`kernel < /lib/systemd/network/*.link < net.ifnames=0 < /{lib,etc}/udev/rules.d/*.rules`