#!/bin/bash
apt install nvidia-detect
nvidia-detect
apt install $driver_pacakge_name
echo blacklist nouveau > /etc/modprobe.d/blacklist-nouveau.conf

cat > /etc/lightdm/lightdm.sh << EOF
#!/bin/bash
xrandr --setprovideroutputsource modesetting NVIDIA-0
xrandr --auto
EOF

# set display-setup-script to /etc/lightdm/lightdm.conf
sed -i 's|^.*\(display-setup-script=\).*$|\1=/etc/lightdm/lightdm.sh|' /etc/lightdm/lightdm.conf

# rebuild dkms if neccessary
dpkg-reconfigure nvidia-kernel-dkms
