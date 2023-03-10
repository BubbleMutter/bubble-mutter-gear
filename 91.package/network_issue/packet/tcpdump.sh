#!/bin/bash
<< EOF
-D 显示有多少张可用网卡
-n 以数字显示ip及端口
-i eth0 指定网卡
-i any  所有网卡
-c $count 包数量
–w filename.log         输出文件
–r google.cap <过滤器>  输入文件 读取过滤器部分
-A 封包的內容以 ASCII 顯示，通常用來捉取 WWW 的網頁封包資料
-x 十六進位 (hex) 以及 ASCII 的封包內容，對於監聽封包內容很有用
-e 显示链路层的头
-d, -dd, -ddd  可阅读的格式, C语言的格式, 十进制数字的格式

过滤器的关键字类型:
类型关键字 ho9t net port
方向关键字 src dst
协议关键字 fddi arp rarp ip tcp udp http ftp smtp
其他关键字 gateway broadcast
运算关键字 not and or less greater
EOF

# 例子
# 210.27.48.1 的主机收到的和发出的所有的数据包
tcpdump host 210.27.48.1

# 210.27.48.1 和 { 210.27.48.2 | 210.27.48.3 }的通信
# 即 210.27.48.1 和 210.27.48.2 的通信
# 即 210.27.48.1 和 210.27.48.3 的通信
tcpdump host 210.27.48.1 and \(210.27.48.2 or 210.27.48.3 \)

# 210.27.48.1 和 除了210.27.48.2 的通信ip包
tcpdump ip host 210.27.48.1 and not 210.27.48.2

# 210.27.48.1接收或发出的telnet包
tcpdump tcp port 23 host 210.27.48.1

# 123 为ntp的服务端口
tcpdump udp port 123

# 监听 from 210.27.48.1 的包
tcpdump -i eth0 src host hostname
# 监听   to 210.27.48.1 的包
tcpdump -i eth0 dst host hostname

# 指定网关的包
tcpdump -i eth0 gateway Gatewayname

# tcpflag
tcpdump  "src host 200.200.95.50 and dst host 200.200.95.52 and tcp and tcp[tcpflags] & (tcp-syn|tcp-rst) != 0"

# '-A' convert to ascii
tcpdump -A -s0 port 80

# '-w' write to file
tcpdump -i eth0 -s0 -w test.pcap

# specify port via text
tcpdump -nn -v port ftp or ftp-data

# ipv6
tcpdump -nn ip6 proto 6

# vlan
tcpdump -i eth0 vlan 1

#####################################################################
# http scenario
# '-l' line mode, useful for many special scenario
sudo tcpdump -nn -A -s1500 -l | grep "User-Agent:"

<< EOF
((tcp[12:1] & 0xf0) >> 2)        := header's the number of bytes
tcp[((tcp[12:1] & 0xf0) >> 2):4] := 4 bytes after header
EOF
# match 'GET' request
tcpdump -s 0 -A -vv 'tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x47455420'
# match 'POST request
tcpdump -s 0 -A -vv 'tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x504f5354'

# extract POST/GET/Host:
tcpdump -s0 -v -n -l | egrep -i "POST /|GET /|Host:"
# Extract HTTP Passwords in POST Requests
tcpdump -s0 -A -n -l | egrep -i "POST /|pwd=|passwd=|password=|Host:"
# Cookie
tcpdump -s0 -A -nn -l | egrep -i 'Set-Cookie|Host:|Cookie:'
#####################################################################

#####################################################################
# icmp scenario

# not standard icmp packet
tcpdump 'icmp[icmptype] != icmp-echo and icmp[icmptype] != icmp-echoreply'
#####################################################################

tcpdump 'ip[2:2]'
tcpdump 'ether[0:2]'

tcpdump -i eth0 'ether[12:2] = 0xcc99' # 第 12个byte 开始 2个bytes, == 0xcc99 的以太网帧
tcpdump -i eth0 ether proto 0xc996
tcpdump -i eth0 ether src aa:bb:cc:dd:ee:ff and ether dst aa:bb:cc:dd:ee:ff

tcpdump -vv -x -X -s 1500 -i eth1 'port 80'
# -vv：更详细的输出信息。
# -x：在解析和打印时，作为打印每一包的头信息的补充(默认情况下 TCPDump 只打印每一包的头信息)，将每一包的数据内容也打印出来。
# -X：在解析和打印时，作为打印每一包的头信息的补充，将每一包的内容信息也以十六进制和 ASCII 码的方式打印出来。这样对于解析新协议是很方便的。
# -s 1500：粗略地显示每一包的信息而不是默认的 68。这在查看大量信息时是很有用的。
# -i eth1：监控 eth1 接口。

tcpdump -n
# don't resolve hostname (it will be faster)