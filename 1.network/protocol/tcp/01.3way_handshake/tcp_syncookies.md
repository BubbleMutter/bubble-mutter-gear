[TOC]
https://segmentfault.com/a/1190000019292140 原理
https://blog.csdn.net/sinat_20184565/article/details/104828782 实现
https://lwn.net/Articles/277146/ 设计

# 1. syncookie 的原理
1. 因为, 客户端发出三次握手包中
    1. ack 必然是服务端二次握手生成的 seq+1
    2. seq 必然是一次握手的 seq+1
2. 如果, 服务端发出二次握手生成 **特殊seq**
3. 使得, 服务端收到三次握手反推 二次握手的 **特殊seq**
    1. 校验连接正确性 (包括1次握手的 seq, 4元祖)
4. 所以, 内核分为两个部分实现
    1. 二次握手服务端 seq 的生成
    2. 三次握手服务端 校验客户端发过来的 ack-1

# 2. syncookie 需要解决的本质问题 (2个)
1. 验证第三次握手 ack  报文的准确性 (没有半连接队列的情况下)
2. 保留 syn 中的TCP选项信息 (如: timestamp, sack, wscale, mss)

# 3. syncookie 目前实现的缺陷
1. 算法上, msstab 索引只能用 3bit (最多8种MSS值)
    + 如果 msstab 索引超过 3bit, 比如 8, 则无法校验
2. 服务端不识别一次握手报文中带有的其他选项信息
    + timestamp, sack, wscale

# 4. syncookie 功能开关 /proc/sys/net/ipv4/tcp_syncookies
+ 0 表示关闭
+ 1 表示超过半连接队列长度才使用
+ 2 表示强制开启

# 5. syncookie 功能完善 (用 timestamp 保留 sack wscale ecn )
1. timestamp 这一 option 有64bit, 取其中的 6 bit (降低 timestamps 精度)
2. 如果客户端 / 服务端都开启了 /proc/sys/net/ipv4/tcp_timestamps
3. 那么服务端将把 一次握手的 sack wscale ecn 信息保存到这 6bit
   从而使得一次握手报文中选项信息得以保存
   Note: 如果客户端没开启 那么三次握手报文将不会把 timestamps 回给服务端
         服务端则没法回溯 一次握手的 sack wscale ecn 信息了
4. 具体保存位置
    + `| TSval: 26bit | ecn: 1bit | sack: 1bit | wscale: 4bit | TSecr: 32bt |`
    + 其中 TSval 和 TSecr 是 timestamp 的原始字段
    + 如果没开 syncookie, TSval 就占 32bit
    + 如果开启 syncookie, TSval 把低  6bit 留出来, 给 sack wscale ecn

# 6. secure_tcp_syn_cookie 生成 (收到第一次握手 syn) (linux-3.16.78)
1.  tcp_v4_rcv             (inet 层收包回调主入口)
    tcp_v4_do_rcv
    tcp_rcv_state_process
    icsk->icsk_af_ops->conn_request
    tcp_v4_conn_request
    cookie_v4_init_sequence (syncookie 生成主入口)
    __cookie_v4_init_sequence
    secure_tcp_syn_cookie
2. 核心代码 secure_tcp_syn_cookie 走读
```c++
#define COOKIEBITS 24	/* Upper bits store count */
#define COOKIEMASK (((__u32)1 << COOKIEBITS) - 1)

static __u16 const msstab[] = {
    536,
    1300,
    1440,    /* 1440, 1452: PPPoE */
    1460,
};

static __u32 secure_tcp_syn_cookie(__be32 saddr, __be32 daddr, __be16 sport,
				   __be16 dport, __u32 sseq, __u32 data  /* msstab 的 索引 */ ) {
    // 其中 data 是 msstab 的索引, 只需要2bit
    // 根据客户端 syn 中的 mss 取相近值; 用来保留一次握手指示的 mss
	u32 count = tcp_cookie_time(); /* 每分钟 */
	return (  cookie_hash(saddr, daddr, sport, dport, 0, 0) /* 4元组 hash */ +
		      sseq /* 客户端第一次握手的 seq */ +
              (count << COOKIEBITS) /* 时间系数 左移 24bit */ +
		      ( (cookie_hash(saddr, daddr, sport, dport, count, 1) /* 4元组hash */ + data)
                & COOKIEMASK /* 保留低 24bit */)
            );
}
```

3. syn 是以下参数的累加
    1. (4元组时间无关的hash)
    2. (4元组时间有关的hash + msstab索引) 取低 24bit
    3. 客户端第一次握手的 seq
    4. 时间系数左移 24bit


# 7. check_tcp_syn_cookie 校验 (收到第三次握手 ack) (linux-3.16.78)
1.  tcp_v4_rcv       (inet 层收包回调主入口)
    tcp_v4_do_rcv    
    tcp_v4_hnd_req
    cookie_v4_check  (syncookie 检验主入口)
    __cookie_v4_check
    check_tcp_syn_cookie

2. 核心代码 check_tcp_syn_cookie 走读
```c++
int tcp_v4_do_rcv(struct sock *sk, struct sk_buff *skb) {
    // ...
	if (sk->sk_state == TCP_LISTEN) {
        /* 因为开启 syncookie 没有 SYN_RECV 状态;
         * 所以 ack 跑过来还是 LISTEN 状态;
         * 然而 syn 跑过来也是 LISTEN 状态;
         * 所以 必须在这里处理 */
		struct sock *nsk = tcp_v4_hnd_req(sk, skb);
        // ...
    }
    // ...
}
static struct sock *tcp_v4_hnd_req(struct sock *sk, struct sk_buff *skb) {
    // ...
#ifdef CONFIG_SYN_COOKIES
    /* 必须是内核编译时开启了 syncookies 选项 */
	if (!th->syn)
		sk = cookie_v4_check(sk, skb, &(IPCB(skb)->opt));
#endif
	return sk;
}
struct sock *cookie_v4_check(struct sock *sk, struct sk_buff *skb) {
    // ...    
    __u32 cookie = ntohl(th->ack_seq) - 1; /* 取得 二次握手服务端发出的 seq */
    // ...
	if (tcp_synq_no_recent_overflow(sk) ||
	    (mss = __cookie_v4_check(ip_hdr(skb), th, cookie)) == 0) {
		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_SYNCOOKIESFAILED);
		goto out;
	}
}

// 返回 mss
int __cookie_v4_check(const struct iphdr *iph, const struct tcphdr *th, u32 cookie) {
	__u32 seq = ntohl(th->seq) - 1; /* 取得 一次握手时客户端发出的 seq */
	__u32 mssind = check_tcp_syn_cookie(cookie, iph->saddr, iph->daddr,
					    th->source, th->dest, seq); /* 返回 一次握手的 msstab 索引 */
	return mssind < ARRAY_SIZE(msstab) ? msstab[mssind] : 0; /* 返回 0 指示校验失败 */
}

#define MAX_SYNCOOKIE_AGE 2

// 返回 一次握手的 msstab 索引
static __u32 check_tcp_syn_cookie(__u32 cookie, __be32 saddr, __be32 daddr,
				  __be16 sport, __be16 dport, __u32 sseq)
{
	u32 diff, count = tcp_cookie_time();
	cookie -= cookie_hash(saddr, daddr, sport, dport, 0, 0) + sseq;
              /* 减去 4元组时间无关hash
               * 减去 一次握手时客户端发出的 seq */
    /* 跑到这里, cookie 由以下组件构成
     * 高 12 bit: 时间系数
     * 低 24 bit: 4元组时间有关hash + msstab 索引 */
    /* 先取得时间系数, 判断是否在允许范围内 */
	diff = (count - (cookie >> COOKIEBITS)) & ((__u32) -1 >> COOKIEBITS);
    /* 时间系数实际上是分钟数, 因此小于 2分钟是允许的 */
	if (diff >= MAX_SYNCOOKIE_AGE)
		return (__u32)-1;

    /* a) count 是当前时间系数
     * b) count - diff 求得服务端二次握手生成的seq 时的 "时间系数"
     * c) COOKIEMASK 是低24bit
     * d) 低24bit 减去 "4元组时间有关hash" 则剩下 msstab 索引 */
	return (cookie -
		cookie_hash(saddr, daddr, sport, dport, count - diff, 1))
		& COOKIEMASK;	/* Leaving the data behind */
}
```

# 8. cookie_init_timestamp (ecn sack wscale 填充 timestamps) (linux-3.16.78)
1.  tcp_v4_rcv             (inet 层收包回调主入口)
    tcp_v4_do_rcv
    tcp_rcv_state_process
    icsk->icsk_af_ops->conn_request
    tcp_v4_conn_request
    tcp_v4_send_synack
    tcp_make_synack
    cookie_init_timestamp
2. 核心代码走读
```c++
#define TSBITS 6
#define TSMASK (((__u32)1 << TSBITS) - 1)
__u32 cookie_init_timestamp(struct request_sock *req)
{
	struct inet_request_sock *ireq;
	u32 ts, ts_now = tcp_time_stamp;
	u32 options = 0;

	ireq = inet_rsk(req);

	options = ireq->wscale_ok ? ireq->snd_wscale : 0xf;
	options |= ireq->sack_ok << 4;
	options |= ireq->ecn_ok << 5;

	ts = ts_now & ~TSMASK; // 把低 6bit 干掉
	ts |= options;         // 把 sack ecn wscale 的 6bit 填充

	if (ts > ts_now) {
        /* 处理由于 |= options 导致的 上溢 */
		ts >>= TSBITS;
		ts--;
		ts <<= TSBITS;
		ts |= options;
	}
	return ts;
}

struct sk_buff *tcp_make_synack(struct sock *sk, struct dst_entry *dst,
				struct request_sock *req,
				struct tcp_fastopen_cookie *foc)
{
    /* 内核编译选项; 开启了 syncookie;
     * 如果这个报文之前已经 构造了 cookie;
     * 那么把 ecn sack wscale 填充到 timestamp 中 */
#ifdef CONFIG_SYN_COOKIES
	if (unlikely(req->cookie_ts))
		TCP_SKB_CB(skb)->when = cookie_init_timestamp(req);
	else
#endif
	TCP_SKB_CB(skb)->when = tcp_time_stamp;
}
```

# 9. cookie_check_timestamp (timestamps 提取 ecn sack wscale) (linux-3.16.78)
1.  tcp_v4_rcv       (inet 层收包回调主入口)
    tcp_v4_do_rcv    
    tcp_v4_hnd_req
    cookie_v4_check  (syncookie 检验主入口)
    cookie_check_timestamp
2. 核心代码走读
```c++
bool cookie_check_timestamp(struct tcp_options_received *tcp_opt, struct net *net, bool *ecn_ok) {
	u32 options = tcp_opt->rcv_tsecr & TSMASK; // 低 6 bit

	if (!tcp_opt->saw_tstamp)  { // 报文没有 timestamp option
		tcp_clear_options(tcp_opt);
		return true;             // 直接返回有效
	}
	if (!sysctl_tcp_timestamps)  // 系统没开 timestamp; 但是报文有, timestamp 失效; 有效
		return false;

	tcp_opt->sack_ok = (options & (1 << 4)) ? TCP_SACK_SEEN : 0; /* 右往左第5bit, 表示 sack */
	*ecn_ok = (options >> 5) & 1; /* 右往左第6bit, 表示 ecn */

	if (*ecn_ok && !net->ipv4.sysctl_tcp_ecn) //  ecn 不符合当前系统状态; 非法
		return false;

	if (tcp_opt->sack_ok && !sysctl_tcp_sack) // sack 不符合当前系统状态; 非法
		return false;

	if ((options & 0xf) == 0xf)               // 报文没有 wscale 缩放因子; 有效
		return true; /* no window scaling */

	tcp_opt->wscale_ok = 1;
	tcp_opt->snd_wscale = options & 0xf;
	return sysctl_tcp_window_scaling != 0;    // 报文有 wscale 缩放因子, 但 系统没开; 非法
}
```