#!/bin/bash
ls -1 *.deb | while read package; do
    workdir=$(mktemp -d)

    dpkg-deb -R $package $workdir
    rm $package
    fakeroot dpkg-deb -Zgzip -b $workdir .
    rm -rf $workdir
done
