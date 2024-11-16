# debian install ppa ----------------
# 1. add http://url/inside/ppa/webpages to /etc/apt/source.list
# 2. query keys from server. The key is inside ppa pages:
#    find something like `Fingerprint: $uuid'
# example
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys  43D3A9F60C58A7169778E6FB8771ADB0816950D8
apt update
# 4. finally you can install package on ppa
# -----------------------------------

apt_mark_tips() {
    # control package auto or manual
    apt-mark auto $package
    apt-mark manual $package
    apt-mark showauto
    apt-mark showmanual
}

apt install xxx-dkms # update initramfs manaully for incorrect dkms
update-initramfs -u
update-grub2
# -----------------------------------

# docker repo
cat > /etc/apt/sources.list.d/docker.list <<EOF
deb [signed-by=/etc/apt/keyrings/docker.gpg] http://mirrors.ustc.edu.cn/docker-ce/linux/debian buster stable
EOF

# apt remain conf
apt -o Dpkg::Options::=--force-confdef -o Dpkg::Options::=--force-confold install $package

# auto resolve lower version's dependence
aptitude -o Dpkg::Options::=--force-confdef -o Dpkg::Options::=--force-confold \
    -o Aptitude::ProblemResolver::SolutionCost=broken-holds -y \
    install $package=$low_version

apt_aarch64_arm64() {
    dpkg --add-architecture arm64
    apt update -o APT::Architecture="arm64,amd64" -o APT::Architectures="arm64,amd64"
    apt download dpkg:arm64
}

make_linux_image_deb() {
    # debian/control
    #   Version: $pkgversion
    #   Package: linux-image-$release
    version=6.1.0 # kernel upstream version code
    arch=$(dpkg --print-architecture)
    release=rain  # custom release code (snow/storm/wind)
    make KDEB_PKGVERSION=$version KERNELRELEASE=$version-$arch-$release bindeb-pkg -j`nproc`
}

modify_deb() {
    # extract with metadata
    dpkg-deb -R ./$package_$version.deb ./$package/
    # archive with metadata
    fakeroot dpkg-deb -Zgzip -b $package .
}

apt_purge() {
    # apt purge
    for d in $(dpkg -l | awk '/^rc/{print $2}'); do sudo apt remove --purge -y $d; done

    # apt trace all out-of-repo package
    for d in $(dpkg -l | awk '/^ii/{print $2}'); do
        if apt-cache policy $d | sed -n '/ \*\*\* /{N;p}' | grep -qw /var/lib/dpkg/status; then
            echo $d
        fi
    done
}

# brief show package version
for d in $(find . -name *.deb); do dpkg -f $d Package Version | cut -d: -f2 | sed 'N;s|\n | |'; done
