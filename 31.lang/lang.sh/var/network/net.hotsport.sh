# 1. using networkmanager gui create a hotspot

# 2. manually connect
sudo nmcli connection up $hotspot ifname wlan0

# 3. modify configure of /etc/dnsmasq.conf

# 4. dns server
systemctl start dnsmasq.service

# 5. appendix [/etc/dnsmasq.conf]
<< EOF
log-queries
log-dhcp
interface=wlan0
dhcp-range=192.168.0.101,192.168.0.253,255.255.255.0,24h
dhcp-option=option:router,192.168.0.1

dhcp-boot=pxelinux.0,pxeserver,192.168.0.1
pxe-service=x86PC, "Install Debian", pxelinux
enable-tftp
tftp-root=/usr/lib/debian-installer/images/10/amd64/text
EOF