# qdisc (queueing discipline)
```sh
# show device qdisc
tc qdisc show dev $dev

tc qdisc replace dev $dev root $qdisc [$qdisc_param]

tc qdisc replace dev $dev root pfifo_fast
tc qdisc replace dev $dev root pfifo
tc qdisc replace dev $dev root fq_codel

tc qdisc replace $qdisc help # show help info
```

物理网卡默认类型 pfifo_fast (老版本是 fq_codel)
虚拟网卡默认类型 noqueue

classless qdisc: 只在 device 的 (egress) root 点生效
classful  qdisc: 包含 class 组件, 同 class 的 qdisc 之间流通, 最终呈现树状结构的流动