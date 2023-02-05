[TOC]
主要实现逻辑
```c++
int __sock_create(struct net *net, int family, int type, int protocol, struct socket **res, int kern)
{
	pf = rcu_dereference(net_families[family]);  // 根据协议簇取得句柄
    /* ... */
    err = pf->create(net, sock, protocol, kern); // 调用协议簇的回调
}

// 注册协议簇, 协议簇方法是 packet_create; 即 socket 创建的回调
static const struct net_proto_family packet_family_ops = {
	.family =	PF_PACKET,
	.create =	packet_create,
	.owner	=	THIS_MODULE,
};

// af_packet 子系统初始化
static int __init packet_init(void)
{
    /* ... */
	sock_register(&packet_family_ops);        // 注册到全局协议簇列表 net_families[] 中
    /* ... */
}

// socket 创建回调
static int packet_create(struct net *net, struct socket *sock, int protocol, int kern)
{
    struct packet_sock *po;
	__be16 proto = (__force __be16)protocol; /* weird, but documented */
    /* ... */
	po->prot_hook.func = packet_rcv;          // 二层协议报文的回调函数
    if (sock->type == SOCK_PACKET)
		po->prot_hook.func = packet_rcv_spkt; // 二层协议报文的回调函数
    if (proto) {
		po->prot_hook.type = proto;           // 二层协议报文的协议号, 形同 ETH_P_IP ETH_P_ARP 等
		__register_prot_hook(sk);             // 注册二层协议的回调
	}
    /* ... */
}
static void __register_prot_hook(struct sock *sk)
{
    /* ... */
	dev_add_pack(&po->prot_hook);             // 注册二层协议的回调
    /* ... */
}
```