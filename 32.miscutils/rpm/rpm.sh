#!/bin/bash
# install epel-release at first
yum install --enablerepo=extras epel-release
# epel repo server may not support https
sed -i "s|=https://|=http://|" /etc/yum.repos.d/epel.repo

yum install kernel-debug-devel dkms

# remove package without dependencies
# note: yum/rpm/dnf will autoremove by default
rpm -e --nodeps $package

# display files inside file
rpm -qlp $package

# search package
rpm -q $regexpr

# extract package
rpm2cpio $package | cpio -idmv

# download source
yum install yum-utils rpm-build
yumdownloader --source redis
