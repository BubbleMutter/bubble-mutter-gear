[TOC]
# aptly
1. 搭建 apt 源有很多方案 https://wiki.debian.org/DebianRepository/Setup
2. 选择 aptly 简单易用

# initialize
```sh
# 1. download (https://www.aptly.info/download)
wget -qO - https://www.aptly.info/pubkey.txt | sudo apt-key add -
apt update
apt install aptly

# 2. run server
( aptly serve -listen=:18081 & )

# 3. create gpg key on server
# show keys
gpg --list-keys
# gen a key (remember the username)
gpg --gen-key
# export public key
gpg --export -a $username > /root/.aptly/public.txt

# 4. create a repo
aptly repo create cust
aptly repo add cust *.deb
aptly snapshot create cust00 from repo cust
aptly publish snapshot -distribution="cust" cust00

# 5. import gpg key on client
cat public.txt | apt-key add -
# 6. import repo on client
echo http://server.com:18081/ cust main
apt update
```

# issue: missing control.tar.gz part when aptly repo add
```sh
package=xxx.deb
workdir=$(mktemp -d)

dpkg-deb -R $package $workdir
rm $package
fakeroot dpkg-deb -Zgzip -b $workdir .
rm -rf $workdir

aptly repo add cust $package
```
