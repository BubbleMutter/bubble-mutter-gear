MIRROR=http://mirrors.ustc.edu.cn
AUTOBUILD=$MIRROR/gentoo/releases/amd64/autobuilds
ISO=$(wget -q $AUTOBUILD/latest-install-amd64-minimal.txt -O- | egrep -w install-amd64-minimal | awk '{print $1}')
wget $AUTOBUILD/$ISO
