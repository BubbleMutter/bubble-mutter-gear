[TOC]
# sock mem related parameters (sysctl/cgroup/socket-option)
```sh
# 1. sysctl
# 2. cgroup kmem, tcp, all
# 3. userspace socket setting
sysctl -a 2>/dev/null | grep -E "udp.*mem|tcp.*mem|core.*mem"

# cgroup kmem
cat /sys/fs/cgroup/memory/$group/memory.kmem.usage_in_bytes     # currently usage
cat /sys/fs/cgroup/memory/$group/memory.kmem.limit_in_bytes     # limit
cat /sys/fs/cgroup/memory/$group/memory.kmem.max_usage_in_bytes # max usage in history

# cgroup tcp
cat /sys/fs/cgroup/memory/$group/memory.kmem.tcp.usage_in_bytes     # currently usage
cat /sys/fs/cgroup/memory/$group/memory.kmem.tcp.limit_in_bytes     # limit
cat /sys/fs/cgroup/memory/$group/memory.kmem.tcp.max_usage_in_bytes # max usage in history

# cgroup all
cat /sys/fs/cgroup/memory/$group/memory.usage_in_bytes     # currently usage
cat /sys/fs/cgroup/memory/$group/memory.limit_in_bytes     # limit
cat /sys/fs/cgroup/memory/$group/memory.max_usage_in_bytes # max usage in history
```
1. memory is not enough, when `usage_in_bytes` is near `limit_in_bytes`

# sock mem in sysctl
```ini
net.core.optmem_max = 20480
net.core.rmem_default = 1048576
net.core.rmem_max = 16777216
net.core.wmem_default = 1048576
net.core.wmem_max = 16777216
net.ipv4.tcp_mem = 2239245	2985660	4478490
net.ipv4.tcp_rmem = 4096	87380	33554432
net.ipv4.tcp_wmem = 4096	65536	33554432
net.ipv4.udp_mem = 4478490	5971321	8956980
net.ipv4.udp_rmem_min = 4096
net.ipv4.udp_wmem_min = 4096
```
1. dump via `sysctl -a 2>/dev/null | grep -E "udp.*mem|tcp.*mem|core.*mem"`

# tcp rmem machanism
1. on init, `socket()`,   socket-rmem is `net.ipv4.tcp_rmem[1]`
2. after set `SO_RCVBUF`,  socket-rmem will be twice
3. when communicating, rmem is dynamic
4. and will be restriced by
    1. `net.core.rmem_max`
    2. `net.ipv4.tcp_mem[2]`
    3. `net.ipv4.tcp_rmem[2]`

# udp rmem machanism
1. on init, `socket()`,   socket-rmem is `net.ipv4.udp_mem[1]`
2. after set `SO_RCVBUF`,  socket-rmem will be the same setting value
3. and will be restriced by
    1. `net.core.rmem_max`
    2. `net.ipv4.udp_mem[2]`