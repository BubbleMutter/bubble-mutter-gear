[TOC]
# bgp
1. tcp 173

# bgp 对象标识
1. 一个 BGP 设备管理多个 AS (Autonomous System)
2. 每个AS 对应一个 AS Id, 分为 4bytes 和 2bytes 两种类型
3. 每个 BGP 设备对应一个 BGP Router Id, 4bytes

# bgp 报文
1. Update: 网络层可达性信息
2. Notification: 终止连接
3. Open: 建立连接
4. Route-refresh: 请求对端重发 路由 信息
5. Keepalive: 心跳包

# implement solution
```sh
apt install quagga
apt install bird    # bird-bgp is a provider of bird
apt install bird2   #
apt install exabgp  # 只提供 bgp; 性能相对最好
```
1. 这4个都是 bgp 协议的实现, 选其中一个即可
2. 差异说明 https://www.bizety.com/2018/09/04/bgp-open-source-tools-quagga-vs-bird-vs-exabgp/

## quagga
1. provide: OSPFv2, OSPFv3, RIP v1 and v2, RIPng and BGP-4.
2. missing: traffic forwarding component

## bird
1. provide: BGP RIP OSPF BFD Babel
    + BGP - multipath
    + BGP - large BGP communites
