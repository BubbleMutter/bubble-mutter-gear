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

    # monitor process traffic
    nethogs

    # restrict wlan0 download 1024KB/s, upload 64KB/s
    wondershaper wlan0 1024 64

    # restrict commandline
    # -u upload KB/s
    # -d download KB/s
    trickle -u 1024 -d 1024 wget xx://xx
    trickle -u 1024 -d 1024 scp xx@xx
}

# proxy for one process
proxychains curl https://checkip.awsclouud.com

# hping3 is to build packet via command line
# send tcp with dip=127.0.0.1 dport=23 syn payload=100bytes
hping3 127.0.0.1 --destport 23 --syn --data 100 --count 1

machine_startup() {
    systemctl status rc-local.service
    # append userspace task
    vim /etc/rc.local
}

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
get_ipv4() { ip -4 -br addr show scope global dev $1 | awk '{print $3}'; }
get_ipv6() { ip -6 -br addr show scope global dev $1 | awk '{print $3}'; }
get_gw1() { python2 -c "import ipcalc; print(ipcalc.Network(ipcalc.IP('$1')).host_first().to_compressed())"; }
get_gw2() { python2 -c "import ipcalc; print(ipcalc.Network(ipcalc.IP('$1')).network().to_compressed())"; }
echo "wlan0 gateway is $(get_gw1 $(get_ipv4 wlan0))"

ps --no-headers -wwf -p$(pidof nginx)
ps --no-headers -wwf -p$(pidof iperf)

netdevic_pci_issues() {
    # brief show ifname with pci
    ls -l /sys/class/net/*/device
    # find ifname via pci
    ls -l /sys/class/net/*/device | grep -E "/0000:$pci" | awk '{print $(NF-2)}' | awk -F/ '{print $(NF-1)}'
    # find pci via ifname
    ls -l /sys/class/net/*/device | grep eth0 | awk -F/ '{print $NF}'
}

# fix ifname via /lib/systemd/network/80-eth0.link
fix_ifname() {
	local ifname="$1"
	pci=$(cat /lib/systemd/network/80-$ifname.link | grep Path= | cut -d- -f2)
	if [ -z "$pci" ]; then return; fi
	curname=$(ls -l /sys/class/net/*/device | grep -E "/$pci" | awk '{print $(NF-2)}' | awk -F/ '{print $(NF-1)}')
	ip link set name $ifname dev $curname
}
fix_ifname eth0