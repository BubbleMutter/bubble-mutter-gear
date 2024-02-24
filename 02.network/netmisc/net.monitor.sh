ss -apn     | sed -n '1p;/sshd/p'                # brief stats
ss -apntime | sed -n '1p;/pid=2046/{N;p}'    # details on sk
ss -apntime | sed -n '1p;/sshd/{N;p}'        # details on sk
ss -apntime -o state established 'sport 443' # https

cat /proc/net/snmp | grep Tcp:  | rs -T | column -t
cat /proc/net/snmp | grep Ip:   | rs -T | column -t
cat /proc/net/snmp | grep Icmp: | rs -T | column -t
cat /proc/net/snmp | grep Udp:  | rs -T | column -t
cat /proc/net/snmp6
cat /proc/net/netstat | grep TcpExt: | rs -T | column -t

netstat -st
nstat -az
watch -n 1 -d 'nstat -a | grep Tcp | grep -E xxx'

conntrack -E -s 19 # conntrack event listener