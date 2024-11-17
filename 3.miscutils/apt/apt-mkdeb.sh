#!/bin/bash
set -ex
cd $(dirname $0)
export ARCH=${ARCH:-$(dpkg-architecture -q DEB_BUILD_ARCH)}
export PACKAGE=$(cat debian/control | grep -w Package | awk '{print $2}')
source VERSIONS # env VERSION indicates version code
export VERSION=$VERSION+byted

if [ "$ARCH" = "arm64" ]; then
	export CC="aarch64-linux-gnu-gcc"
fi

rm -f debian/changelog
debchange --create --package $PACKAGE --newversion $VERSION --distribution stable --controlmaint release
dpkg-buildpackage --host-arch=$ARCH --build=binary --no-sign \
	--buildinfo-option="--version" --changes-option="-u./output"
	--hook-done="rm -f ../%p_%v_*.changes"
