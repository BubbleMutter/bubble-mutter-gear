[TOC]
```c++
// 2MSL 的默认值是 60秒
#define TCP_TIMEWAIT_LEN (60*HZ) /* how long to wait to destroy TIME-WAIT state, about 60 seconds	*/
```

```c++
// 1. tcp 4元组的保存方式是 hash 表
// 2. 拆分为 establish 状态表 和 listening 状态表
struct inet_hashinfo {
	struct inet_ehash_bucket	*ehash;
	spinlock_t			*ehash_locks;
	unsigned int			ehash_mask;
	unsigned int			ehash_locks_mask;
    struct inet_listen_hashbucket	listening_hash[INET_LHTABLE_SIZE] ____cacheline_aligned_in_smp;
};
struct inet_hashinfo tcp_hashinfo;
EXPORT_SYMBOL(tcp_hashinfo);

// 4元组找 established 状态的 socket
struct sock *__inet_lookup_established(struct net *net,
				  struct inet_hashinfo *hashinfo,
				  const __be32 saddr, const __be16 sport,
				  const __be32 daddr, const u16 hnum,
				  const int dif, const int sdif);

// 4元组找 listening 状态的 socket
struct sock *__inet_lookup_listener(struct net *net,
				    struct inet_hashinfo *hashinfo,
				    struct sk_buff *skb, int doff,
				    const __be32 saddr, __be16 sport,
				    const __be32 daddr, const unsigned short hnum,
				    const int dif, const int sdif);

// ehash 的大小
void __init tcp_init(void) {
	tcp_hashinfo.ehash =
		alloc_large_system_hash("TCP established",
					sizeof(struct inet_ehash_bucket),
					thash_entries,
					17, /* one slot per 128 KB of memory */
					0,
					NULL,
					&tcp_hashinfo.ehash_mask,
					0,
					thash_entries ? 0 : 512 * 1024);
}
```