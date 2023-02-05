[TOC]
# 内核选项开启
```s
* General setup  --->
    + [*] Profiling support
    + Kernel Performance Events And Counters  --->
        -  [*] Kernel performance events and counters
```

# perf + 生成火焰图
```sh
# 在运行机子中 #######################################
perf record -F 1000 -a -g -o /var/perf.data # 采集数据
# Ctrl-C 结束采集
perf script -i /var/perf.data > perf.unfold # 解析数据
# 或者配合 timeout 采集给定时间
timeout perf record -F 1000 -a -g -o /var/perf.data
#####################################################

# 在编译服务器上 #####################################
git clone https://github.com/brendangregg/FlameGraph
# 安装 ....
stackcollapse-perf.pl perf.unfold  > perf.folded # 展开数据
flamegraph.pl perf.folded > perf.svg             # 生成火焰图
#####################################################
```