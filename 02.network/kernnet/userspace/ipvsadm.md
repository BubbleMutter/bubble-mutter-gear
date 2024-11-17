[TOC]
# ipvsadm (DNAT Load Balance)
+ ipvsadm rule
    + virtual server = DNAT match condition
    + physics server = DNAT's dip
    + scheduler algorithms
+ packet forward mode
    + `-g`, gatewaying (Direct Routing) (DNAT only) = Triangle Flow
    + `-m`, masquerading (NAT) (SNAT and DNAT) = Serial Flow
+ DPDK-IPVS = https://github.com/iqiyi/dpvs

# ipvsadm usage
```sh
# net topo
# client --> gateway:[ eth0=192.168.1.1, eth1=192.168.2.1 ] --> server[192.168.2.2~192.168.2.253]

# add virtual server [match dip:dport = 192.168.1.1:80, scheduler=rr]
ipvsadm -A -t 192.168.1.1:80 -s rr

# del virtual server [match dip:dport = 192.168.1.1:80]
ipvsadm -D -t 192.168.1.1:80

# add physics server [real ip address] on a virtual server
ipvsadm -a -t 192.168.1.1:80 -r 192.168.2.2 -w 1 -g # Direct Route
ipvsadm -a -t 192.168.1.1:80 -r 192.168.2.3 -w 2 -m # Masquerading

# del physics server
ipvsadm -d -t 192.168.1.1:80 -r 192.168.2.2
ipvsadm -d -t 192.168.1.1:80 -r 192.168.2.3

# get virtual server algorithms and weighs
ipvsadm -Ln
# get virtual server packets stat
ipvsadm -Ln --stats
# get connections
ipvsadm -lnc
```
1. virtual server id is equal to `match-condition`
2. physics server address can be accessed via gateway

# ipvsadm sysctl optimize
```sh
cat > /etc/sysctl.d/50-ipvsadm.conf <<EOF
net.ipv4.netfilter.ip_conntrack_generic_timeout = 600
net.ipv4.netfilter.ip_conntrack_icmp_timeout = 30
net.ipv4.netfilter.ip_conntrack_tcp_timeout_close = 10
net.ipv4.netfilter.ip_conntrack_tcp_timeout_close_wait = 60
net.ipv4.netfilter.ip_conntrack_tcp_timeout_established = 432000
net.ipv4.netfilter.ip_conntrack_tcp_timeout_fin_wait = 120
net.ipv4.netfilter.ip_conntrack_tcp_timeout_last_ack = 30
net.ipv4.netfilter.ip_conntrack_tcp_timeout_max_retrans = 300
net.ipv4.netfilter.ip_conntrack_tcp_timeout_syn_recv = 60
net.ipv4.netfilter.ip_conntrack_tcp_timeout_syn_sent = 120
net.ipv4.netfilter.ip_conntrack_tcp_timeout_syn_sent2 = 120
net.ipv4.netfilter.ip_conntrack_tcp_timeout_time_wait = 120
net.ipv4.netfilter.ip_conntrack_udp_timeout = 30
net.ipv4.netfilter.ip_conntrack_udp_timeout_stream = 180
EOF
sysctl -p /etc/sysctl.d/50-ipvsadm.conf
```