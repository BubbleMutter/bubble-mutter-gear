[TOC]
1. `skb_orphan()` 调用 skb->destructor 持有者销毁回调
2. 分析典型的持有者销毁回调

# netlink_skb_destructor (packet  socket)
# tpacket_destruct_skb   (netlink socket)
# unix_destruct_scm      (unix    socket)
# sock_rfree      原始 收包 socket 销毁
# sock_wfree      原始 发包 socket 销毁
# sock_efree      原始 错包 socket 销毁
# sock_ofree      ??
# sock_rmem_free  ??
# tcp_wfree    tcp 带数据  发包 socket 销毁
```c++
void tcp_wfree(struct sk_buff *skb) {
	struct sock *sk = skb->sk;
	struct tcp_sock *tp = tcp_sk(sk);
	struct tsq_tasklet *tsq; // 一个 tcp_socket 对应一个 tsq_tasklet
	// ...
	tsq = this_cpu_ptr(&tsq_tasklet);    // 每个 cpu 一个 tsq_tasklet 队列
	list_add(&tp->tsq_node, &tsq->head); // 当前 skb 的 tasket 入列当前cpu队列
	tasklet_schedule(&tsq->tasklet);     // 拉起 tasket, 其中实现 tcp skb 重传
	local_irq_restore(flags);
	// ...
}

/* 发包时赋值 tcp_wfree */
static int __tcp_transmit_skb(struct sock *sk, struct sk_buff *skb,
			      int clone_it, gfp_t gfp_mask, u32 rcv_nxt) {
	skb_orphan(skb);
	skb->sk = sk;
	skb->destructor = skb_is_tcp_pure_ack(skb) ? __sock_wfree : tcp_wfree;
}

static void tcp_tasklet_func(unsigned long data); // tsq_tasklet 回调
static void tcp_tsq_handler(struct sock *sk); // tsq_tasklet 发送方法
```
1. 为什么要在 发包 销毁中重新发包 ??

# __sock_wfree tcp 纯 ack  发包 socket 销毁
```c++
void __sock_wfree(struct sk_buff *skb) {
	struct sock *sk = skb->sk;
	/* 减少 socket 发包缓存区计数
	 * 如果 socket 发包缓存区为零, 释放 socket */
	if (refcount_sub_and_test(skb->truesize, &sk->sk_wmem_alloc))
		__sk_free(sk);
}

static int __tcp_transmit_skb(struct sock *sk, struct sk_buff *skb,
			      int clone_it, gfp_t gfp_mask, u32 rcv_nxt) {
	skb_orphan(skb);
	skb->sk = sk;
	skb->destructor = skb_is_tcp_pure_ack(skb) ? __sock_wfree : tcp_wfree;
}
```

# sock_edemux  tcp  SYN_RECV/TIME_WAIT 销毁
```c++
void sock_edemux(struct sk_buff *skb) {
	sock_gen_put(skb->sk);
}

void sock_gen_put(struct sock *sk) {
	if (!refcount_dec_and_test(&sk->sk_refcnt))
		return;

	if (sk->sk_state == TCP_TIME_WAIT)
		inet_twsk_free(inet_twsk(sk)); // 从 TIME_WAIT 队列销毁
	else if (sk->sk_state == TCP_NEW_SYN_RECV)
		reqsk_free(inet_reqsk(sk));    // 从 半连接    队列销毁
	else
		sk_free(sk);                   // 直接销毁
}

/* sock_edemux 赋值处 */
int tcp_v4_early_demux(struct sk_buff *skb) {
    // ...
    /* 4元组找 socket */ 
    sk = __inet_lookup_established(dev_net(skb->dev), &tcp_hashinfo,
				       iph->saddr, th->source,
				       iph->daddr, ntohs(th->dest),
				       skb->skb_iif, inet_sdif(skb));
	if (sk) {
		skb->sk = sk;
		skb->destructor = sock_edemux;
        // ...
    }
    // ...
}

/* sock_edemux 赋值处 */
void skb_set_owner_w(struct sk_buff *skb, struct sock *sk)
{
	skb_orphan(skb);
	skb->sk = sk;

    /* 非 time_wait 和 syn_recv 状态的 socket */
	if (unlikely(!sk_fullsock(sk))) {
        /* 跑进来, 说明是 time_wait 和 syn_recv 状态的 socket */
		skb->destructor = sock_edemux;
		sock_hold(sk);
		return;
	}

	skb->destructor = sock_wfree;
    // ...
}
static inline bool sk_fullsock(const struct sock *sk)
{
	return (1 << sk->sk_state) & ~(TCPF_TIME_WAIT | TCPF_NEW_SYN_RECV);
}
```
1. sock_edemux 调用 sock_gen_put 销毁 半连接队列, TIME_WAIT 队列
2. sock_edemux 在 tcp_v4_early_demux 赋值, 如果是数据收发报文, 其他函数将覆盖 skb->destructor
3. sock_edemux 在 skb_set_owner_w 中把 time_wait 和 syn_recv 状态的赋值为 sock_edemux