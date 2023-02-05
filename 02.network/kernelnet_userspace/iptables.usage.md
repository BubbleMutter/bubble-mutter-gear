[TOC]
# iptables usage
# iptables rate limit
```sh
# 限速示例: 先把约定速率的报文匹配, 然后把超速报文 DROP
iptables -N ICMP_RATE
iptables -A ICMP_RATE -p icmp -m limit --limit 5/minute --limit-burst 10 -j ACCEPT
iptables -A ICMP_RATE -p icmp -j DROP
iptables -A INPUT -j ICMP_RATE
```

# iptables fwmark with iprule
```sh
# 匹配域名的 ip 走策略路由, 必须在 mangle chain 中
iptables -t mangle -A OUTPUT -m set --match-set mirror dst -j MARK --set-xmark 0x1000/0x1000
ip rule add from all fwmark 0x1000/0x1000 lookup 50
ipset create mirror hash:ip
ipset add mirror x.x.x.x
ipset add mirror x.y.e.d
```

# iptables security on public node
```sh
# 1.禁止入方向的 ping 。
-A INPUT -p icmp --icmp-type echo-request -j DROP

# 2. hide from traceroute
iptables -A OUTPUT -p icmp --icmp-type port-unreachable -j DROP
iptables -A OUTPUT -p icmp --icmp-type host-unreachable -j DROP

# 3.禁止伺服器回覆 no listen port 的 tcp reset 訊息（標誌位爲 rst,ack ）
iptables -A OUTPUT -p tcp --tcp-flags ALL RST,ACK -j DROP

# 4. 禁止 从外往内 发起新连接, 但允许 从内向外 发起新连接
iptables -t raw -A PREROUTING -i wlan0 -m conntrack --ctstate INVALID,NEW -j DROP
iptables -t raw -A OUTPUT -j CT --notrack
```

# iptables stats on ip
```sh
# 网关统计目标ip发起的连接数, 注意: 该规则没有 -j 指定 action
iptables -t raw -A POSTROUTING -m set --match-set target_ip dst
# 网关统计源  ip发起的连接数, 注意: 该规则没有 -j 指定 action
iptables -t raw -A PREROUTING  -m set --match-set source_ip src
```

# iptables setup transparent proxy
```sh
##########################################################
#                                                        #
# client... <-> proxy 192.168.1.1 <-> real 192.168.1.2   #
#                                                        #
##########################################################

# 在 proxy 进行如下重定向 [1] + { [2] or [3] }
# [1] 重定向目的地址 -j DNAT --to-destination
iptables -t nat -A PREROUTING -d 192.168.1.1 -p tcp -m tcp --dport 80 -j DNAT --to-destination 192.168.1.2
# [2] 重定向源  地址 -j SNAT --to-source       (如此一来 real 只认识 proxy )
iptables -t nat -A POSTROUTING -d 192.168.1.2 -p tcp -m tcp --dport 80 -j SNAT --to-source 192.168.1.1
```

# iptables setup router
```sh
# pub ... <-> wlan0[192.168.110.22] PC ethX[192.168.50.1] <-> clinet*N [192.168.50.*]

# solution1: SNAT
iptables -t nat -A POSTROUTING --src 192.168.50./24 -j SNAT --to-source 192.168.110.22
# solution2: MASQUERADE
iptables -t nat -A POSTROUTING -o wlan0 -j MASQUERADE
```