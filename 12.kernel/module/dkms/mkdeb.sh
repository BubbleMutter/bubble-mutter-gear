#!/bin/bash
set -ex

PACKAGE_VERSION=$(bash -c 'source src/dkms.conf; echo $PACKAGE_VERSION')
PACKAGE_NAME=$(bash -c 'source src/dkms.conf; echo $PACKAGE_NAME')
PACKAGE_NAME=$(echo $PACKAGE_NAME | tr '_' '-')-dkms
ARCH=$(cat debian/control | grep -E "^Architecture:" | awk '{print $2}')

rm -f debian/changelog
debchange --create --package $PACKAGE_NAME \
          --newversion $PACKAGE_VERSION \
          --distribution stable \
          --controlmaint "automatic release by John and Jane"

if [ -d src/scripts ]; then chmod a+x src/scripts/*; fi
deb-buildchange -tc -us -uc -b \
                --buildinfo-option="-O" \
                --hook-done="mv ../%p_%v_${ARCH}.deb ./%p_%v_${ARCH}.deb"