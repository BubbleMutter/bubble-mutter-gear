[TOC]
# deb
```sh
# show package files
dpkg -c xxx.deb # via package filename
dpkg -L debname # via installed packname

# extract content file
dpkg -x xxx.deb $target
# extract metadata file
dpkg -e xxx.deb $target
```

# metadata
```tree
DEBIAN/
    control
    postinst
    postrm
    preinst
    prerm
```
+ DEBIAN/control
    ```yaml
    ```
+ DEBIAN/postinst
    ```sh
    ```
+ DEBIAN/postrm
    ```sh
    ```
+ DEBIAN/preinst
    ```sh
    ```
+ DEBIAN/prerm
    ```sh
    ```

# issue
```sh
sudo dpkg --configure --force-overwrite -a

# --fix-broken issue solution
sudo apt -o Dpkg::Options::="--force-overwrite" --fix-broken install
```