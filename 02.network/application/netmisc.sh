systemctl start dnsmasq # dns + dhcp server
systemctl start nscd    # dns cache server
systemctl start atop    # disk basic system monitor (very powerful, but no gui)

# dstat monitor netcard traffic
dstat -N wlan0
dstat -N eth0

# monitor process traffic
nethogs

# proxy for one process
proxychains curl https://checkip.awsclouud.com
