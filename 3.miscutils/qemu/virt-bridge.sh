#!/bin/bash
ip link add virbr0 type bridge
ip addr add 192.168.101.1/24 dev virbr0

iptables -t nat -A POSTROUTING -s 192.168.101.0/24 -d 224.0.0.0/24 -j RETURN
iptables -t nat -A POSTROUTING -s 192.168.101.0/24 -d 255.255.255.255/32 -j RETURN
iptables -t nat -A POSTROUTING -s 192.168.101.0/24 ! -d 192.168.101.0/24 -j MASQUERADE
