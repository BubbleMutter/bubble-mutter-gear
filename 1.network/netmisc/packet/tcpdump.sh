#!/bin/bash

# enable user to use tcpdump
sudo gpasswd -a tcpdump $(whoami)
sudo setcap cap_net_raw,cap_net_admin=eip $(which tcpdump)

# '-w' write to file
tcpdump -i eth0 -s0 -w test.pcap

# filter host
tcpdump -ni eth0 host 210.27.48.1
tcpdump -ni eth0 src host hostname
tcpdump -ni eth0 dst host hostname

# filter port
tcpdump -ni eth0 port 80
tcpdump -ni eth0 port ftp or ftp-data # specify port via text
tcpdump -ni eth0 ip6 proto 6             # ipv6
tcpdump -ni eth0 vlan 1              # vlan

# 指定网关的包
tcpdump -i eth0 gateway Gatewayname

# filter tcpflag
tcpdump  "src host 200.200.95.50 and dst host 200.200.95.52 and tcp and tcp[tcpflags] & (tcp-syn|tcp-rst) != 0"

# filter http usefully
# '-A' convert to ascii
# '-l' line mode, useful for many special scenario
sudo tcpdump -ni eth0 -A -l -s1500 port 443 or port 80 | grep "User-Agent:"
<< EOF
((tcp[12:1] & 0xf0) >> 2)        := header's the number of bytes
tcp[((tcp[12:1] & 0xf0) >> 2):4] := 4 bytes after header
EOF

# filter http 'GET' request
tcpdump -s 0 -A -vv 'tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x47455420'
# filter http 'POST' request
tcpdump -s 0 -A -vv 'tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x504f5354'

# filter http POST/GET/Host:
tcpdump -s0 -v -n -l | egrep -i "POST /|GET /|Host:"
# filter http headers (Cookie, e.g.)
tcpdump -s0 -A -n -l | egrep -i "POST /|pwd=|passwd=|password=|Host:"
tcpdump -s0 -A -nn -l | egrep -i 'Set-Cookie|Host:|Cookie:'

# filter icmp type
tcpdump 'icmp[icmptype] != icmp-echo and icmp[icmptype] != icmp-echoreply'

# filter ethternet protocol
tcpdump -i eth0 'ether[12:2] = 0xcc99' # 第 12个byte 开始 2个bytes, == 0xcc99 的以太网帧
tcpdump -i eth0 ether proto 0xc996
tcpdump -i eth0 ether src aa:bb:cc:dd:ee:ff and ether dst aa:bb:cc:dd:ee:ff

# -vv：更详细的输出信息。
# -x：在解析和打印时，作为打印每一包的头信息的补充(默认情况下 TCPDump 只打印每一包的头信息)，将每一包的数据内容也打印出来。
# -X：在解析和打印时，作为打印每一包的头信息的补充，将每一包的内容信息也以十六进制和 ASCII 码的方式打印出来。这样对于解析新协议是很方便的。
# -s 1500：粗略地显示每一包的信息而不是默认的 68。这在查看大量信息时是很有用的。
# -i eth1：监控 eth1 接口。
tcpdump -vv -x -X -s 1500 -i eth1 'port 80'

# don't resolve hostname (it will be faster)
tcpdump -n

