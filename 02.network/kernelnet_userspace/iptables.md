[TOC]
# iptables
+ iptables tables = filter / raw / mangle / nat
+ iptables chains = PREROUTING / INPUT / FORWARD / OUTPUT / POSTROUTING

# iptables tables command
```sh
iptables -t filter -S
iptables -t raw    -S
iptables -t mangle -S
iptables -t nat    -S

# add
iptables -N OUTPUT # 创建 chain
# del
iptables -X OUTPUT # 删除 chain
# set
iptables -P OUTPUT <ACCEPT | DROP>  # 设置默认 policy
iptables -E OUTPUT $NEW_NAME        # rename chain
```

# iptables rules command
```sh
# add
iptables -A OUTPUT $rule          # 插入 rule 到尾部
iptables -I OUTPUT $rulenum $rule # 插入 rule 到指定 rulenum 的前面

# del
iptables -D OUTPUT $rule    # 删除指定 rule 描述
iptables -D OUTPUT $rulenum # 删除指定 rule 行号（iptables -nL 显示行号)
iptables -F OUTPUT          # 删除所有 rule

# get
iptables -S OUTPUT # 显示 rule 设置时的具体命令
iptables -L OUTPUT # 显示 rule 另一种展示形式
iptables -L OUTPUT --line-numbers # 显示 rule 行号
```

# iptables match
```sh
# match ipv4
iptables -A OUTPUT -4
# match ipv6
iptables -A OUTPUT -6

# match protocol: tcp, udp, udplite, icmp, icmpv6, esp, ah, sctp, mh
iptables -A OUTPUT -p tcp
iptables -A OUTPUT -p udp

# match source address
iptables -A OUTPUT -s 192.168.0.1/255.255.255.0 -j ACCEPT
# match destination address
iptables -A OUTPUT -d 192.168.0.9/255.255.255.0 -j ACCEPT

# match input network interface card
iptables -A OUTPUT -i eth0 -j ACCPET
# match output network interface card
iptables -A OUTPUT -o eth1 -j ACCEPT

# match source mac
# only on PREROUTING / FORWARD / INPUT; 只能用于匹配源 mac
# 目的mac 在 arp表 与 目的ip 一一对应; 要是有需要也是匹配 目的ip
iptables -A OUTPUT --mac-source aa:bb:cc:dd:ee:ff -j ACCEPT

# match mark, which is set by -j MARK
iptables -A OUTPUT -m mark --mark 0x0001/0x0001 -j DROP

# match iprange
iptables -A PREROUTING -m iprange --src-range 192.168.1.1-192.168.2.254 -j DROP
iptables -A PREROUTING -m iprange --dst-range 10.245.10.1-10.245.20.254 -j DROP

# match socket state
iptables -A PREROUTING -m conntrack --ctstate SNAT -j DROP         # packet has been SNAT
iptables -A POSTROUTING -m conntrack --ctstate DNAT -j DROP        # packet has been DNAT
iptables -A PREROUTING -m conntrack --ctstate INVALID -j DROP      # packet can't found socket
iptables -A PREROUTING -m conntrack --ctstate ESTABLISHED -j DROP  # packet match tcp ESTABLISHED
iptables -A PREROUTING -m conntrack --ctstate NEW -j DROP
iptables -A PREROUTING -m conntrack --ctstate RELATED -j DROP
iptables -A PREROUTING -m conntrack --ctstate UNTRACKED -j DROP

# match link layer pkt-type (mac-address)
iptables -A PREROUTING -m pkttype --pkt-type unicast
iptables -A PREROUTING -m pkttype --pkt-type multicast
iptables -A PREROUTING -m pkttype --pkt-type broadcast

# match cpu
iptables -t nat -A PREROUTING -m cpu --cpu 0 -p tcp --dport 80 -j # cpu 0

# match tcp
# --tcp-flags <mask> <value>; 即用报文的 flags 与 <mask> 与操作, 判断是否等于 <value>
iptables -A INPUT -m tcp --tcp-flags SYN,ACK,FIN,RST SYN  # match packet only syn
iptables -A INPUT -m tcp --tcp-flags SYN SYN   # match packet with syn
iptables -A INPUT -m tcp --tcp-option 1        # match packet has option-id 1
iptables -A INPUT -m tcp --sport 8080 -j DROP
iptables -A INPUT -m tcp --dport 8080 -j DROP
```

# iptables target
+ -j ACCEPT: 通过该 chain 所在最上级 chain, 即 PREROUTING / INPUT / FORWARD / OUTPUT / POSTROUTING
+ -j DROP: 丢弃该 packet
+ -j RETURN: 返回到上一级 chain 的下一条 rule, 如果已经是最上级 chain, 则根据 policy 决定流向

```sh
# set notrack (reserve storage of session table)
# CT --ontrack is equivalent to NOTRACK
iptables -j CT --notrack # -m conntrack --ctstate UNTRACKED
iptables -j NOTRACK      # -m conntrack --ctstate UNTRACKED

# use ipset
iptables -j SET --add-set $ipsetname dst,src # 把包的目的ip 和 源ip 添加到指定 ipset 集合
iptables -m set --match-set $setname dst     # 只匹配, 允许没有 action, 仅用于统

# -j NETMAP 映射整个网段 (地址映射)
# 可用于 nat 各个钩子点
iptables -t nat -A POSTROUTING -s 192.168.0.0/16 -j NETMAP --to 10.245.0.0/16

# -j SNAT (地址映射)
# 仅用于 nat INPUT POSTROUTING
iptables -t nat -A INPUT -s 10.248.0.0/16 --to-source 192.168.10.101-192.168.10.200:10000-20000

# -j DNAT (地址映射)
# 仅用于 nat PREROUTING OUTPUT
iptables -t nat -A OUTPUT -s 10.248.0.0/16 --to-destination 10.245.1.100-10.245.1.200:10000-20000

# -j MASQUERADE 网关代理动态 SNAT (地址映射) 仅用于 nat POSTROUTING
# --to-ports $from-$to 指定端口映射区间
iptables -t nat -A POSTROUTING -o $netdev -j MASQUERADE
iptables -t nat -A POSTROUTING --src $private_ip -j SNAT --to-source $server_pub_ip

# -j REDIRECT (端口映射)
# 仅用于 nat PREROUTING OUTPUT
# --to-ports $from-$to 指定端口重定向区间
iptables -A OUTPUT -j REDIRECT -d 10.246.0.0/16 --to-ports 2046
iptables -A OUTPUT -j REDIRECT -d 10.246.0.0/16 --to-ports 1000-2000

# -j MARK 设置 packet MARK
# 用于 match 的 `-m mark --mark` 项
iptables -A OUTPUT -j MARK --set-xmark 0x0001/0x0001 # xor setting
iptables -A OUTPUT -j MARK --set-xmark 0x0001/0x0001 # or  setting

# -j REJECT
# DROP 后, 回发 icmp 到发送端
# 实际上对于部分入口报文, 显式通知发送端, 有利于发送端业务处理
# 回发 icmp 报文类型:
#   icmp-net-unreachable (default)
#   icmp-host-unreachable
#   icmp-port-unreachable
#   icmp-proto-unreachable
#   icmp-net-prohibited
#   icmp-host-prohibited
#   icmp-admin-prohibited
iptables -A PREROUTPING -s 10.245.0.0/16 -j REJECT
iptables -A PREROUTPING -s 10.245.0.0/16 -j REJECT --reject-with icmp-net-unreachable

# -j TOS
# 仅用于 mangle
iptables -t mangle -A OUTPUT -j TOS --set-tos 16 # Minimize-Delay
iptables -t mangle -A OUTPUT -j TOS --set-tos  8 # Maximize-Throughput
iptables -t mangle -A OUTPUT -j TOS --set-tos  4 # Maximize-Reliability
iptables -t mangle -A OUTPUT -j TOS --set-tos  2 # Minimize-Cost
iptables -t mangle -A OUTPUT -j TOS --set-tos  0 # Normal-Service
```
1. 映射: SNAT DNAT NETMAP MASQUERADE REDIRECT