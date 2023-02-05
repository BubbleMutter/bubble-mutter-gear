[TOC]
# systemd link
1. mechanism: match and assign
2. match with section **Match**
3. assign via section **Link**

# directories priority
> `/etc/systemd/network`
> `/usr/local/lib/systemd/network`
> `/run/systemd/network`
> `/usr/lib/systemd/network`

# debug
```sh
sudo SYSTEMD_LOG_LEVEL=debug udevadm test-builtin net_setup_link /sys/class/net/eth0
```

# example: wlan0
```ini
[Match]
# match via pci
Path=pci-0000:8.00.0

[Link]
# iface name
Name=wlan0
# ethool -G $dev tx 4096
TransmitQueues=4096
# ethtoo -G $dev rx 4096
ReceiveQueues=4096

# RCO
ReceiveChecksumOffload=true
# TCO
TransmitChecksumOffload=true
# GSO
GenericSegmentationOffload=true
# GRO
GenericReceiveOffload=true
# LRO
LargeReceiveOffload=true
```
