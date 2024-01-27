#!/bin/bash
iptables -t nat -A PREROUTING -i eth0 -p tcp --dst $server_pub_ip --dport 443 -j DNAT --to-destination 172.67.135.83
iptables -t nat -A POSTROUTING -o eth0 -p tcp --src 172.67.135.83 --sport 443 -j SNAT --to-source $server_pub_ip

