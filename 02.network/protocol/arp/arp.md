# arp
+ [arp cache desgin](http://www.cnitblog.com/flutist1225/articles/19996.html)

# struct arphdr
```c++
#include <linux/if_arp.h>
struct arphdr {
	__be16		    ar_hrd;	/* format of hardware address	*/
	__be16		    ar_pro;	/* format of protocol address	*/
	unsigned char	ar_hln; /* length of hardware address	*/
	unsigned char	ar_pln; /* length of protocol address	*/
	__be16		    ar_op;	/* ARP opcode (command)		*/
#if 0 // Ethernet looks like this : This bit is variable sized however...
	unsigned char	ar_sha[ETH_ALEN]; /* sender hardware address	*/
	unsigned char	ar_sip[4];		  /* sender IP address		*/
	unsigned char	ar_tha[ETH_ALEN]; /* target hardware address	*/
	unsigned char	ar_tip[4];		  /* target IP address		*/
#endif
};
```
+ an ethernet ipv4 example
```c++
struct arphdr arphdr = {
    .ar_hrd = ETH_P_802_3, // 1
    .ar_pro = ETH_P_IP,    // 0x0800
    .ar_hln = ETH_ALEN,    // 6
    .ar_pln = sizeof(struct in_addr), // 4
}
```

# struct neighbour
```c++
struct neighbour {
	struct neighbour __rcu	*next;
	struct neigh_table	*tbl;
	struct neigh_parms	*parms;
	unsigned long		confirmed;
	unsigned long		updated;
	rwlock_t		lock;
	refcount_t		refcnt;
	unsigned int		arp_queue_len_bytes;
	struct sk_buff_head	arp_queue;
	struct timer_list	timer;
	unsigned long		used;
	atomic_t		probes;
	u8			nud_state;
	u8			type;
	u8			dead;
	u8			protocol;
	u32			flags;
	seqlock_t		ha_lock;
	unsigned char		ha[ALIGN(MAX_ADDR_LEN, sizeof(unsigned long))] __aligned(8);
	struct hh_cache		hh;
	int			(*output)(struct neighbour *, struct sk_buff *);
	const struct neigh_ops	*ops;
	struct list_head	gc_list;
	struct list_head	managed_list;
	struct rcu_head		rcu;
	struct net_device	*dev;
	netdevice_tracker	dev_tracker;
	u8			primary_key[];
} __randomize_layout;
```

# kernel learn gratuitous arp
```c++
arp_process() {
    if (inet_confirm_dst_addr(in_dev, sip)) {
        n = __neigh_lookup(&arp_tbl, &sip, dev, 1);
    }
}
```
1. patch in `arp_process()`