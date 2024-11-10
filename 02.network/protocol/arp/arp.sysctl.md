# arp sysctl

# sysctl neigh gc
net.ipv4.neigh.default.gc_interval   = timer gc internal
net.ipv4.neigh.default.gc_stale_time = 定时器: 查一次相邻层记录的有效性
net.ipv4.neigh.default.gc_thresh1    = only `count > gc_thresh1`, gc is runnable
net.ipv4.neigh.default.gc_thresh2    = soft-maximum of neighbors, after `> gc_thresh2`, gc must run
net.ipv4.neigh.default.gc_thresh3    = hard-maximum of neighbors, no more new neighbor when `> gc_thresh3`

gc_thresh3 一般来说设置为 subnet 机器数的2倍
比如说 掩码是 255.255.255.0, 即 24bit
机器数就是 `2**(32 - 24)*2` 即 512
```c++
gc_thresh3 = num_subnet*2;
gc_thresh2 = gc_thresh3/2;
gc_thresh1 = gc_thresh2/2;
```