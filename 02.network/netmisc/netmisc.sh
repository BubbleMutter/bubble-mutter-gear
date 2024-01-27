systemctl start dnsmasq # dns + dhcp server
systemctl start nscd    # dns cache server
systemctl start atop    # disk basic system monitor (very powerful, but no gui)

# ancient server: daytime / echo / discard / chargen
apt install inetsim
vim /etc/inetsim/inetsim.conf
systemctl start inetsim

# dstat monitor netcard traffic
dstat -N eth0,total

# monitor process traffic
nethogs
nethogs eth0 -d 5 # 5 second update

# proxy for one process
proxychains curl https://checkip.awsclouud.com

# hping3 is to build packet via command line
# send tcp with dip=127.0.0.1 dport=23 syn payload=100bytes
hping3 127.0.0.1 --destport 23 --syn --data 100 --count 1
