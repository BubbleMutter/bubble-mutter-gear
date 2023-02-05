[TOC]

# tcp_keepalive_timer
```c++
static void tcp_keepalive_timer (unsigned long data) {
	if (elapsed >= keepalive_time_when(tp)) {
		/* If the TCP_USER_TIMEOUT option is enabled, use that
		 * to determine when to timeout instead.
		 */
		if ((icsk->icsk_user_timeout != 0 &&
		    elapsed >= icsk->icsk_user_timeout &&
		    icsk->icsk_probes_out > 0) ||
		    (icsk->icsk_user_timeout == 0 &&
		    icsk->icsk_probes_out >= keepalive_probes(tp))) {
			tcp_send_active_reset(sk, GFP_ATOMIC); // 发送 rst 报文
			tcp_write_err(sk);                     // 拉起 socket 错误标记
			goto out;
		}
        /* ... */
    }
}
static void tcp_write_err(struct sock *sk)
{
	sk->sk_err = sk->sk_err_soft ? : ETIMEDOUT;
	sk->sk_error_report(sk);

	tcp_write_queue_purge(sk);
	tcp_done(sk);
	__NET_INC_STATS(sock_net(sk), LINUX_MIB_TCPABORTONTIMEOUT);
}
```
1. 内核函数 tcp_keepalive_timer 处理 keepalive 超时
2. 只有 ESTABLISHED 状态才需要真实意义上需要超时处理
3. 主要业务包括
    1. 发送 rst
    2. 拉起 socket 错误标记  ETIMEDOUT; 使得用户态系统调用报错