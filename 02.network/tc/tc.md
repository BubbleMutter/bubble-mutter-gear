# tc (traffic controll)
+ shaping (egress)
+ scheduling (egress)
+ policing (ingress)
+ dropping (egress + ingress)

# qdiscs
network-interface-transmission

# classes
classless qdisc
classful  qdisc
qdisc 本质是数据通讯算法, 最早可以追溯到电话时代

# filters
used by the classful qdisc determine packet-enqueue-class
+ bpf: using ebpf filter packet
+ basic: ematch-expression
+ cgroup:
+ flow,flower: flow-based classifiers
+ fw: directly maps fwmark value to traffic class
+ route: filter packet by routing table
+ rsvp: match RSVP packet. (Resource Reservation Protocol)
+ tcindex: filter by tc index
+ u32: filter on arbitray packet data
+ matchall: every packet

# classless qdisc
## choke
variation of RED qdisc.

## codel
adaptive "no-knobs" AQM (active queue management).
to address shortcomings of RED-like qdisc

## pfifo: packet fifo
fifo and limited in packets

## bfifo: byte fifo
fifo and limited in bytes

## fq: fair queue scheduler
实现 tcp-pacing
适配每 qdisc 百万级流并发

## fq_codel: fq + codel

## fq_pie: fq + pie

## gred: generalized RED
combined multiple RED queues.

## hhf: Heavy-Hitter-Filter.

## ingress:
applies on an interface. used for tc::filtering.

## mqprio: multiqueue priority qdisc

## multiq: multiqueue
qdisc optimized for devices with multiple tx queues.

## netem: network emulator
traffic control facilities.
+ features:
    + add delay
    + packet loss
    + duplication

## pfifo_fast:

## pie: Proprotional Integral controller-Enhanced
AQM scheme
aims to control delay

## red: Random Early Detection


## rr: Round-Robin (deprecated)

## sfb: Stochastic Fair Blue

## sfq: Stochastic Fairness Queueing

## tbf: Token Bucket Filter
slowing traffic down to precisely configured rate.
scales well to large bandwidths

# classful qdiscs
## ATM

## CBQ

## DRR: Deficit Round Robin

## DSMARK

## ETS

## HFSC: Hierarchy Fair Service Curve

## HTB: Hierarchy Token Bucket

## PRIO:

## QFQ: Quick Fair Queueing
