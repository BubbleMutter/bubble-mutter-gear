#!/bin/bash
#
# tcpdump ipv6 支持还不完善(2022/12/24), 需要用 偏移 过滤

tcpdump -i wlan0 'icmp6 && ip6[40] == 128' # echo-request
tcpdump -i wlan0 'icmp6 && ip6[40] == 129' # echo-reply
tcpdump -i wlan0 'icmp6 && ip6[40] == 133' # router-solicitation
tcpdump -i wlan0 'icmp6 && ip6[40] == 134' # router-advertisement
tcpdump -i wlan0 'icmp6 && ip6[40] == 135' # neighbor-solicitation
tcpdump -i wlan0 'icmp6 && ip6[40] == 136' # neighbor-advertisement