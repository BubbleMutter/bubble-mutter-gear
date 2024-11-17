# get public ip address on internet
curl -s https://checkip.amazonaws.com

# trust https
wget --no-check-certificate https://
git -c http.sslVerify=false clone https://

systemctl start dnsmasq # dns + dhcp server
systemctl start nscd    # dns cache server
systemctl start atop    # disk basic system monitor (very powerful, but no gui)

# ancient server: daytime / echo / discard / chargen
apt install inetsim
vim /etc/inetsim/inetsim.conf
systemctl start inetsim

bandwidth() {
    # monitor netcard traffic
    dstat -N eth0,total

    # monitor process bandwidth
    nethogs

    # restrict wlan0 download 1024KB/s, upload 64KB/s
    wondershaper wlan0 1024 64

    # restrict commandline
    # -u upload KB/s
    # -d download KB/s
    trickle -u 1024 -d 1024 wget xx://xx
    trickle -u 1024 -d 1024 scp xx@xx
}

# hping3 is to build packet via command line
# send tcp with dip=127.0.0.1 dport=23 syn payload=100bytes
hping3 127.0.0.1 --destport 23 --syn --data 100 --count 1

ntpme() {
    timedatectl list-timezones
    timedatectl set-timezone Asia/Shanghai
    timedatectl set-ntp true
    apt install ntp
    systemctl status ntp
    # /usr/sbin/ntpd -p /var/run/ntpd.pid -g
    ntpq -p # print ntp record
}

# subnet calculate
ip -4 -br addr show scope 0 dev eth0 | awk '{print $NF}'
ip -6 -br addr show scope 0 dev eth0 | awk '{print $NF}'

ps --no-headers -wwf -p$(pidof nginx)
ps --no-headers -wwf -p$(pidof iperf)

netdevic_pci_issues() {
    # brief show ifname with pci
    ls -l /sys/class/net/*/device
    # find ifname via pci
    ls -l /sys/class/net/*/device | awk "/$pci/"'{print $(NF-2)}' | awk -F/ '{print $(NF-1)}'
    # find pci via ifname
    ls -l /sys/class/net/*/device | awk -F/ '/eth0/{print $NF}'
}

# fix ifname via /lib/systemd/network/80-eth0.link
fix_ifname() {
	local ifname="$1"
	pci=$(cat /lib/systemd/network/80-$ifname.link | awk -F- '/Path=/{print $2}'
	if [ -z "$pci" ]; then return; fi
	curname=$(ls -l /sys/class/net/*/device | awk "/$pci/"'{print $(NF-2)}' | awk -F/ '{print $(NF-1)}')
	ip link set name $ifname dev $curname
}
fix_ifname eth0

ruby -run -ehttpd /path/to/directory -p8000

ethtool_memo() {
    ethtool -g $dev
    ethtool -G $dev rx $queue_depth # 上调收包队列深度
    ethtool -G $dev tx $queue_depth # 上调发包队列深度; 一般调整意义不大, 因为上面有tc qdisc

    ethtool -l $dev
    ethtool -L $dev rx $number
    ethtool -L $dev tx $number
    ethtool -L $dev combined $number
    # overview information
    ethtool $dev
    # check using driver and firmware
    ethtoo -i $dev
}

tcping() {
    apt install tcptraceroute
    wget http://www.vdberg.org/~richard/tcpping
    chmod a+x tcpping
    tcpping 127.0.0.1 22
}


traceroute -m10 $dip
traceroute -m10 --udp $dip
traceroute -m10 --tcp $dip