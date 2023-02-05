[TOC]
# skb->pkt_type / skb->protocol
```c++
struct sk_buff {
    __u8			pkt_type:3;
	__be16			protocol;
};

/**
 * @brief set skb->pkt_type and return skb->protocol
 */
__be16 eth_type_trans(struct sk_buff *skb, struct net_device *dev);

// skb->pkt_type
#define PACKET_HOST		 // packet-mac-addr 为 接收网卡的 mac-addr
#define PACKET_BROADCAST // packet-mac-addr 为 ff:ff:ff:ff:ff:ff
#define PACKET_MULTICAST // packet-mac-addr 为 组播 mac
#define PACKET_OTHERHOST // packet-mac-addr 为 单播但不是接收网卡的 mac

// skb->protocol
#define ETH_P_IP	0x0800		/* Internet Protocol packet	*/
#define ETH_P_ARP	0x0806		/* Address Resolution packet */
#define ETH_P_IPV6	0x86DD		/* IPv6 over bluebook		*/
#define ETH_P_LLDP	0x88CC		/* Link Layer Discovery Protocol */

// usage
static int rtl8139_rx(struct net_device *dev, struct rtl8139_private *tp,
		      int budget) {
    // ...
    skb->protocol = eth_type_trans(skb, dev);
    netif_receive_skb(skb);
    // ...
}
```
1. `skb->pkt_type` `skb->protocol` 与记录链路层协议处理
2. `eth_type_trans()` 设置 `skb->pkt_type` 并返回 `skb->protocol`
    + 通常用于网卡收包回调, 上协议栈前

# struct packet_type / ptype_base / ptype_all
```c++
struct packet_type {
	__be16			type;	/* This is really htons(ether_type). */
	int			(*func) (struct sk_buff *, struct net_device *,
					     struct packet_type *, struct net_device *);
    // ...
	struct net		*af_packet_net;  /* net namespace */
	void			*af_packet_priv; /* 协议上下文 */
	struct list_head	list;        /* 链表 */
    // ...
};

struct list_head ptype_base[PTYPE_HASH_SIZE] __read_mostly;
struct list_head ptype_all __read_mostly;	/* Taps */

static inline struct list_head *ptype_head(const struct packet_type *pt) {
	if (pt->type == htons(ETH_P_ALL))
		return pt->dev ? &pt->dev->ptype_all : &ptype_all;
	else
		return pt->dev ? &pt->dev->ptype_specific :
				 &ptype_base[ntohs(pt->type) & PTYPE_HASH_MASK];
}
```
1. `struct packet_type`: 一个链路层协议的处理器
    + 其中 `type` 对应 `skb->protocol`
2. `ptype_base`: 链路层协议的全局索引表
3. `ptype_all`:  所有链路层协议都处理的特殊回调链表
4. `ptype_head()`: 根据 type 取得 hash-bucket

# dev_add_pack / dev_remove_pack
```c++
void dev_add_pack(struct packet_type *pt) {
    // ...
	struct list_head *head = ptype_head(pt);
	list_add_rcu(&pt->list, head);
    // ...
}
void dev_remove_pack(struct packet_type *pt) {
	struct list_head *head = ptype_head(pt);
	struct packet_type *pt1;
    // ...
	list_for_each_entry(pt1, head, list) {
		if (pt == pt1) {
			list_del_rcu(&pt->list);
            // ...
		}
	}
    // ...
}
```
1. `dev_add_pack()`:    注册一个链路层协议的处理器
2. `dev_remove_pack()`: 删除一个链路层协议的处理器