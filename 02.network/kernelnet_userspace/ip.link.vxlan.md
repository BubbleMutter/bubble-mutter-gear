[TOC]
# vxlan
1. 使得跨三层的 vxlan节点 下的主机如同在同个大二层中交互
2. 主机本身对 vxlan 协议是无感知的
3. vxlan节点 间通过 udp 包裹传输主机的原始报文
4. 最终实现 主机 可以维持 mac 和 ip 在交换机间迁移
5. 这样子的 主机 可以是虚拟机 或 虚拟机镜像

# constructor
## vxlan package
```txt
| vxlan-mac | vxlan-ip | vxlan-udp | origin-frame |
```

## VTEP ( vxlan Tunnel Endpoint )
1. 维护 nexthop 表
    + `key: { vxlan-id, dest主机ip }`
    + `value: next网关ip`
2. 后端 daemon
    + 监听 vxlan 协议端口
    + 处理 vxlan 报文
3. 接入层物理网口, 解析入方向报文
   根据 dest主机ip 以及端口配置的 vxlan-id
   查询 nexthop 表, 然后把报文转发 or 本机无 vxlan 转发
4. 上游端口, 监听 vxlan 报文

## VNI ( vxlan identify )
1. 形同 vlanid
2. 有 16M

# linux ip link vxlan
+ https://vincent.bernat.ch/en/blog/2017-vxlan-linux
+ VMs and VTEPs learning mode
    1. VTEPs multicast
    2. VTEPs unicast with static flooding
    3. unicast with static L2 entries
    4. unicast with static L3 entries

## VTEPs multicast (before linux-3.8.z)
```sh
ip -6 link add vxlan$id type vxlan id $id dstport 4789 \
  local 2001:db8:1::1 group ff05::100 \
  dev eth0 ttl 5
brctl addbr vxbr$id
brctl addif vxbr$id vxlan$id
brctl addif vxbr$id veth$vm0
brctl addif vxbr$id veth$vm1
brctl stp br100 off
```
1. broadcast/unknown-unicast/multicast flood within multicast group
2. new VTEPs are discovered via igmp (a specific multicast group)
3. new VMs are discovered via broadcast/unknown-unicast/multicast

## VTEPs unicast with static flooding
```sh
ip -6 link add vxlan$id type vxlan id $id dstport 4789 \
  local 2001:db8:1::1
bridge fdb append 00:00:00:00:00:00 dev vxlan$id dst 2001:db8:2::1
bridge fdb append 00:00:00:00:00:00 dev vxlan$id dst 2001:db8:3::1
```
1. broadcast/unknown-unicast/multicast flood via unicast to each specified VTEP
2. all VTEPs are manual specified by ip, but their macs are learned via source-address
3. new VMs are discovered via broadcast/unknown-unicast/multicast

## unicast with static L2 entries
```sh
ip -6 link add vxlan$id type vxlan id $id dstport 4789 \
  local 2001:db8:1::1 \
  nolearning
bridge fdb append 00:00:00:00:00:00 dev vxlan$id dst 2001:db8:2::1
bridge fdb append 00:00:00:00:00:00 dev vxlan$id dst 2001:db8:3::1
bridge fdb append 78:56:43:00:00:01 dev vxlan$id dst 2001:db8:2::1
bridge fdb append 78:56:43:00:00:02 dev vxlan$id dst 2001:db8:2::1
bridge fdb append 78:56:43:00:00:03 dev vxlan$id dst 2001:db8:3::1
```
1. broadcast/unknown-unicast/multicast flood via unicast to each specified VTEP
2. all VTEPs are manual specified by ip, but their macs are learned via source-address
3. all VMs are manual specified by mac (in fdb), and no more learning

## unicast with static L3 entries
```sh
ip -6 link add vxlan$id type vxlan id $id dstport 4789 \
  local 2001:db8:1::1 \
  nolearning proxy

ip -6 neigh add 2001:db8:ff::11 lladdr 78:56:43:00:00:01 dev vxlan$id
ip -6 neigh add 2001:db8:ff::12 lladdr 78:56:43:00:00:02 dev vxlan$id
ip -6 neigh add 2001:db8:ff::13 lladdr 78:56:43:00:00:03 dev vxlan$id

bridge fdb append 78:56:43:00:00:01 dev vxlan$id dst 2001:db8:2::1
bridge fdb append 78:56:43:00:00:02 dev vxlan$id dst 2001:db8:2::1
bridge fdb append 78:56:43:00:00:03 dev vxlan$id dst 2001:db8:3::1
```
1. no broadcast/unknown-unicast/multicast flood
2. all VTEPs are manual specified by ip and mac
3. all VMs are manual specified by mac (in fdb), and no more learning
