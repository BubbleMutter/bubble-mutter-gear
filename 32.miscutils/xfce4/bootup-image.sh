#!/bin/bash
MIRROR=http://mirrors.ustc.edu.cn
AUTOBUILD=$MIRROR/gentoo/releases/amd64/autobuilds
MINIMAL=$AUTOBUILD/latest-install-amd64-minimal.txt 
ISO=$(wget -q -O- $MINIMAL | egrep -w install-amd64-minimal | awk '{print $1}')
echo $AUTOBUILD/$ISO
