### VNC
sudo launchctl stop com.apple.RemoteDesktop.agent
sudo launchctl start com.apple.RemoteDesktop.agent

### command line
+ find  用 mdfind
+ ldd   用 otool
+ htop 监控CPU 内存读写负载
+ slurm 监控网络读写负载
+ iostat 监控磁盘读写负载
softwareupdate -i command line tools
xcode-select -s  /Library/Developer/CommandLineTools
xcode-select -s  /Applications/Xcode9.app/Contents/Developer
然后，改变 “某个文件”
使得 /usr/bin/gcc, /usr/bin/svn 等 就会去读取“某个文件”，然后执行相对应的gcc, svn工具
几乎所有 /usr/bin/gcc,svn等工具的文件大小是 18240 Bytes

### .DS_Store
是用来存储这个文件夹的显示属性的：比如文件图标的摆放位置。删除以后的副作用就是这些信息的失去。（当然，这点副作用其实不是太大）
尽管这些文件本来是为Finder所使用，但它们被设想作为一种更通用的有关显示设置的元数据存储，诸如图标位置和视图设置。
和别人交换文件（或你做的网页需要上传的时候）应该把 .DS_Store 文件删除比较妥当，因为里面包含了一些你不一定希望别人看见的信息（尤其是网站，通过 .DS_Store 可以知道这个目录里面所有文件的清单，很多时候这是一个不希望出现的问题）