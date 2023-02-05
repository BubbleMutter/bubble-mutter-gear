
[TOC]
# option
```sh
-p # pid
-u # user
-t # show pid only
```

# network example
```sh
losf -i            # means LISTENING & ESTABLISHED
lsof -i TCP:1-1024 # tcp port range 1-1024
lsof -i TCP:22     # tcp specific port
lsof -i 4          # ipv6 socket
lsof -i 6          # ipv4 socket

# filter inet socket (ip, tcp, udp, sctp)
lsof -Pan -i  -p $(pgrep sshd)
lsof -Pan -i4 -p $(pgrep sshd)       # ipv4
lsof -Pan -i6 -p $(pgrep sshd)       # ipv6
lsof -Pan -iUDP -p $(pgrep sshd)     # udp
lsof -Pan -p -iTCP -i4 $(pgrep sshd) # ipv4 and tcp
lsof -Pan -p -iTCP -i6 $(pgrep sshd) # ipv6 and tcp

# filter tcp-socket
lsof -Pan -iTCP -p $(pgrep sshd)
lsof -Pan -iTCP -sTCP:LISTEN -p $(pgrep sshd)

# filter unix-socket
lsof -Pan -U -p $(pgrep sshd)
```
+ lsof 看不 TIME-WAIT, SYN-RECV 状态下的 socket
