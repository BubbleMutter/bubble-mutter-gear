[TOC]
syn 选取问题
# 1. 原始问题
假如一个TCP连接的第一个报文序列号(ISN)是0，那么后续每个报文的序列号是固定的
但是为了防止黑客使用TCP Spoof方法攻击, 这个ISN要求是随机的, 避免被黑客猜到

# 2. RFC1948 建议算法
RFC1948里建议的随机算法是 ISN = M + F(localhost, localport, remotehost, remoteport)
其中M是一个计时器，每4毫秒加1, F是一个Hash算法，比如MD5或者SHA256

TCP协议要使用的序列号是后面报文实际携带的序列号和ISN的相对值。

# 3. 内核实现 ( 客户端发 syn; 服务端回 syn+ack ) ( linux-3.16.78 )
1. 内核公式 seq = 基准值 + md5sum( 4元组, 随机数 )
    + 基准值每 64 纳秒加一
	+ 随机数只有在内核首次调用时, 初始化一次, 之后只有重启才会变化
1. 客户端 入口调用链
    SYSCALL_DEFINE3(connect, int, fd, struct sockaddr __user *, uservaddr, int, addrlen);
    sock->ops->connect  // socket 系统调用时赋值
    tcp_prot->connect
    tcp_v4_connect
2.	tcp_v4_rcv
 	tcp_v4_do_rcv
	tcp_rcv_state_process
	icsk->icsk_af_ops->conn_request
	tcp_v4_conn_request
	tcp_v4_init_sequence
2. 关键函数
```c++
tcp_v4_connect() { /* 客户端 */
	if (tp->rx_opt.ts_recent_stamp && inet->inet_daddr != daddr) {
        // ...
		if (likely(!tp->repair))
			tp->write_seq	   = 0; 
	}
    if (!tp->write_seq && likely(!tp->repair))
        // 按照 rfc19894 的建议; seq 根据4元组变动
		tp->write_seq = secure_tcp_sequence_number(inet->inet_saddr,
							   inet->inet_daddr,
							   inet->inet_sport,
							   usin->sin_port);
}

tcp_v4_conn_request() { /* 服务端 */
	/* 半连接队列满了 */
	if ((sysctl_tcp_syncookies == 2 ||
	     inet_csk_reqsk_queue_is_full(sk)) && !isn) {
		want_cookie = tcp_syn_flood_action(sk, skb, "TCP");
		// ...
	}

	if (want_cookie) {
		/* 如果前面判断是 需要 cookie 的话, 那么走 syncookie 生成 syn 逻辑 */
		// ...
		isn = cookie_v4_init_sequence(sk, skb, &req->mss);
		// ...
	} else if (!isn) {
		/* 在不使用 cookie 的情况下走 默认随机生成的 逻辑 */
		// ...
		isn = tcp_v4_init_sequence(skb); // 直接 return secure_tcp_sequence_number
		// ...
	}
	tcp_rsk(req)->snt_isn = isn;
}

__u32 secure_tcp_sequence_number(__be32 saddr, __be32 daddr,
				 __be16 sport, __be16 dport)
{
	u32 hash[MD5_DIGEST_WORDS];

	net_secret_init(); // 获取一次随机数; 放到局部静态变量net_secret中
	hash[0] = (__force u32)saddr;
	hash[1] = (__force u32)daddr;
	hash[2] = ((__force u16)sport << 16) + (__force u16)dport;
	hash[3] = net_secret[15];

	md5_transform(hash, net_secret);

	return seq_scale(hash[0]);
}

// 获取一个随机数
static u32 net_secret[NET_SECRET_SIZE] ____cacheline_aligned;
static __always_inline void net_secret_init(void) {
	net_get_random_once(net_secret, sizeof(net_secret));
}

// 在 seq 加上基准值; 基准值每 64 纳秒加一
static u32 seq_scale(u32 seq) {
	/*
	 *	As close as possible to RFC 793, which
	 *	suggests using a 250 kHz clock.
	 *	Further reading shows this assumes 2 Mb/s networks.
	 *	For 10 Mb/s Ethernet, a 1 MHz clock is appropriate.
	 *	For 10 Gb/s Ethernet, a 1 GHz clock should be ok, but
	 *	we also need to limit the resolution so that the u32 seq
	 *	overlaps less than one time per MSL (2 minutes).
	 *	Choosing a clock of 64 ns period is OK. (period of 274 s)
	 */
    // ktime_to_ns 求得纳秒数;
    // >> 6, 即 / 64;
	return seq + (ktime_to_ns(ktime_get_real()) >> 6);
}
```
