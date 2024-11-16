[TOC]
# vxlan
+ features:
    + 在跨三层网络中, 组建形同 vlan 的内网, 俗称三层的VLAN, 又称 overlay 网络
    + vxlan节点 间通过 udp 包裹传输主机的原始报文
+ vxlan header =
    + VNI (vxlan identify) = 形同 vlanid, size = 16M
+ vxlan packet = `phy-mac | phy-ip | vxlan-udp | inner-mac | inner-ip | l4-payload`
    + `outer_dmac = phy-gateway`,  `outer_smac = local-phy`
    + `outer_dip = remote-vxlan`, `outer_sip = local-vxlan`
+ VTEP(Vxlan Tunnel Endpoint) = vtepip 指 vxlan 的 phy 网卡的 ip

# linux vxlan tx looking route mechanism
```sh
# 1. lookup inner_sip, inner_smac via inner_dip, and out interface is vxlan
ip route get $inner_di dev dev vxlan
# 2. lookup inner_dmac via inner_dip
ip neigh show to $inner_dip dev vxlan
# 3. lookup outer_dip via inner_dmac
bridge fdb get $inner_dmac self dev vxlan
# 4. lookup outer_sip, outer_smac via outer_dip, and out interface is phy of vxlan
ip route get $outer_dip dev $phy
# 5. lookup outer_dmac via outer_dip
ip neigh show to $outer_dip dev $phy
```
1. route to vxlan needs 5 step: inner_route / inner_neigh / fdb / outer_route / outer_neigh
2. an remote virtual machine = (inner_dip, inner_dmac)

# linux vxlan tx setting route example
```sh
fakert_ip=1.1.1.1
fakert_gw=1.1.1.0

ip link add vxlan type vxlan id $vni dev $phy local $inner_ip dstport 4789 nolearning
ip addr add $fakert_ip/31 dev vxlan
ip link set up vxlan

ip route add $inner_dip via $fakert_gw dev vxlan
ip neigh add $fakert_gw dev vxlan lladdr $inner_dmac nud permanent
bridge fdb add $inner_dmac dev vxlan dst $outer_dip
ip route add $outer_dip via $phy_gw dev $phy
ip neigh add $phy_gw dev $phy lladdr $outer_dmac nud permanent
```

# netlink filter vxlan bridge programming tips
```c++
socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
bind(sockfd, sockaddr_nl.nl_groups |= RTMGRP_NEIGH);
struct {
    struct nlmsghdr nlmsghdr;
    struct ndmsg ndmsg;
    char buf[8192];
} msg = {
    .nlmsghdr = {
        .nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)),
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP,
        .nlmsg_type = RTM_GETNEIGH,
    },
    .ndmsg = { /* apply filter */
        .ndm_family = AF_BRIDGE,
        .ndm_ifindex = vxlan_ifindex,
        .ndm_flags = NTF_SELF,
        .ndm_state = NUD_PERMANENT,
    }
};
struct ndmsg {
    unsigned char ndm_family;  /* filter be AF_BRIDGE */
    int           ndm_ifindex; /* filter vxlan index */
    __u8          ndm_flags;   /* filter NTF_SELF */
    __u16         ndm_state;   /* filter 如果是静态配置则 NUD_PERMANENT */
    __u8          ndm_type;
}; // in NLMSG_DATA(nlmsghdr)
```

# VTEPS and VMs learning mode
+ https://vincent.bernat.ch/en/blog/2017-vxlan-linux
+ VMs and VTEPs learning mode
    1. VTEPs multicast
    2. VTEPs unicast with static flooding
    3. unicast with static L2 entries
    4. unicast with static L3 entries

## multicast (before linux-3.8.z)
```sh
ip -6 link add vxlan$id type vxlan \
  id $id dstport 4789 \
  local 2001:db8:1::1 \
  group ff05::100 \
  dev $phy

bridge fdb show dev vxlan$id | grep -w dst
```
1. VTEPs are discovered via igmp (in the multicast group)
2. VMs are discovered via broadcast among VTEPs
    + append to neighbor table and fdb table
    + example: `outer[2001:db8:1::1 -> ff05::100] inner[who has vm? tell vm0]`

## unicast with static flooding
```sh
ip -6 link add vxlan$id type vxlan \
  id $id dstport 4789 \
  local 2001:db8:1::1
bridge fdb append 00:00:00:00:00:00 dev vxlan$id dst 2001:db8:2::1
bridge fdb append 00:00:00:00:00:00 dev vxlan$id dst 2001:db8:3::1
```
1. VTEPs are statically specified and no learning
2. VMs are discovered via broadcast among VTEPs using arp/ndisc
    + append to neighbor table and fdb table

## unicast with static L2 entries
```sh
ip -6 link add vxlan$id type vxlan \
  id $id dstport 4789 \
  local 2001:db8:1::1 \
  nolearning
bridge fdb append 00:00:00:00:00:00 dev vxlan$id dst 2001:db8:2::1 # used for broadcast
bridge fdb append 00:00:00:00:00:00 dev vxlan$id dst 2001:db8:3::1 # used for broadcast
bridge fdb append 78:56:43:00:00:01 dev vxlan$id dst 2001:db8:2::1 # VM's fdb
bridge fdb append 78:56:43:00:00:02 dev vxlan$id dst 2001:db8:2::1 # VM's fdb
bridge fdb append 78:56:43:00:00:03 dev vxlan$id dst 2001:db8:3::1 # VM's fdb
```
1. VTEPs are specified and no learning
    + `nolearning` disables vxlan learn bridge fdb, according `vxlan.c:vxlan_set_mac(),vxlan_rcv()`
    + but neigh can be learned
2. VMs are discovered via broadcast among VTEPs using arp/ndisc
    + append to neighbor table but no fdb table
3. when VMs' inner-dmac is unspecified in fdb, packets will be broadcast to all VTEPs

## unicast with static L3 entries
```sh
ip -6 link add vxlan$id type vxlan \
  id $id dstport 4789 \
  local 2001:db8:1::1 \
  nolearning \
  proxy

ip -6 neigh add 2001:db8:ff::11 lladdr 78:56:43:00:00:01 dev vxlan$id # VM's neigh
ip -6 neigh add 2001:db8:ff::12 lladdr 78:56:43:00:00:02 dev vxlan$id # VM's neigh
ip -6 neigh add 2001:db8:ff::13 lladdr 78:56:43:00:00:03 dev vxlan$id # VM's neigh

bridge fdb append 78:56:43:00:00:01 dev vxlan$id dst 2001:db8:2::1 # VM's fdb
bridge fdb append 78:56:43:00:00:02 dev vxlan$id dst 2001:db8:2::1 # VM's fdb
bridge fdb append 78:56:43:00:00:03 dev vxlan$id dst 2001:db8:3::1 # VM's fdb
```
1. VTEPs are specified and no learning
2. all VMs are specified and no learning
    + `proxy` disables vxlan send arp/ndisc_ns, according `vxlan.c:vxlan_xmit()`

## unicast with dynamic L3 entries
```sh
ip -6 link add vxlan$id type vxlan \
  id $id dstport 4789 \
  local 2001:db8:1::1 \
  nolearning proxy \
  l2miss l3miss

ip monitor neigh dev vxlan100
# miss 2001:db8:ff::12 STALE
# miss lladdr 50:54:33:00:00:0a STALE
```
1. VTEPs and VMs are not learning
2. recvmsg from `socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);` and get missing event
    1. dynamically set neigh and fdb with ctrl-plane

# vxlan on practice
```sh
ip route add default via $gw_on_phy dev vxlan$id
ip neigh add $gw_on_phy lladdr 00:01:02:03:04:05 dev vxlan$id nud permanent
bridge fdb add 00:01:02:03:04:05 dev vxlan$id dst $gw_on_phy
```
1. currently get gateway upgrade to vxlan, reconfigure host to adapt