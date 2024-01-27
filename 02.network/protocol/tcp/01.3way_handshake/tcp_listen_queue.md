# tcp 三次握手 半连接队列 和 全连接队列
参考 https://www.cnblogs.com/xiaolincoding/p/12995358.html
1. 半连接队列, listen 中内核已发送二次握手的队列, 也称 syn 队列 (`ss -tnp state syn-recv`)
2. 全连接队列, listen 中内核已完成三次握手但没有 accept 的队列,
   也称 accept 队列 ( `ss -tnpl` LISTEN 的 Recv-Q 列)
   因为 accept 之后, 将会创建新的 socket 返回 fd. 并从 listen 监听的 fd 中删掉
3. 实际上, 半连接队列, 对于服务端用户态程序而言, 是透明的

## 调试
1. 查看半连接队列
    1. `ss -tnp state syn-recv`
    2. `netstat -tnp | grep SYN_RECV`
2. 查看全连接队列 (listen 的 fd 接收缓存)
    1. ` ss -tnpl` 的 Recv-Q 列
    2. `netstat -tpnl` 的 Recv-Q 列
3. 检查半连接队列溢出 `ss -s | grep ?`
```txt
>_ netstat -s | grep -i listen
100 times the listen queue of a socket overflowed
100 SYNs to LISTEN sockets ignored
```
4. 检查全连接队列溢出 `ss -s | grep ?`
5. 调整半连接队列 `tcp_max_syn_backlog`
6. 调整全连接队列 `/proc/sys/net/core/somaxconn` 和 `listen` 的第二个参数
7. 模拟半连接溢出 `hping3 -S -p 8080 --flood 127.0.0.1` (也是模拟 http 的半连接攻击)
    + `-S` 使用 SYN 攻击
    + `-p 8080` http 服务器端口
8. 模拟全连接溢出 `wrk -t 4 -c 30000 -d 120s http://127.0.0.1:8080`
    + `-t 4` 使用的线程数
    + `-c 30000` 半连接个数
    + `-d 120s` 持续120秒

## 机制
1. 服务端收到客户端的 syn 请求后
    1. 内核创建 半连接对象; 加入 半连接队列
    2. 内核定时向客户端发送 syn + ack; 等待客户端 的 ack
2. 服务端收到客户端的 ack 响应后
    1. 内核把连接从 半连接对象队列移除
    2. 创建全连接对象
    3. 加入全连接队列
3. 服务端用户态进程调用 accept
    1. 内核把全连接队列从 全连接队列取出
    2. 创建 socket 返回新的 fd

## 实现
1. 全连接队列大小的确定
```c++
SYSCALL_DEFINE2(listen, int, fd, int, backlog) {
    int somaxconn;
    if (sock) {
        somaxconn = sock_net(sock->sk)->core.sysctl_somaxconn;
        // 取 somaxconn, backlog 的较小值
        if ((unsigned int)backlog > somaxconn) // somaxconn 即 /proc/sys/net/core/somaxconn
            backlog = somaxconn;               // backlog 即 listen 的第二个参数
        err = security_socket_listen(sock, backlog);
        if (!err)
            err = sock->ops->listen(sock, backlog); // inet_stream_ops { .listen = inet_listen, }
}
int inet_listen(struct socket *sock, int backlog) {
    // ...
    sk->sk_max_ack_backlog = backlog;
    // ...
}
bool sk_acceptq_is_full(const struct sock *sk) {
    return sk->sk_ack_backlog > sk->sk_max_ack_backlog;
}
```
2. 半连接队列大小的理论值
    1. 开启 tcp_syncookies; 等同全连接队列大小 `sk->sk_max_ack_backlog`
    2. 关掉 tcp_syncookies; 全连接队列的 0.75 `sk->sk_max_ack_backlog*0.75`
    3. 当全连接队列满了, 即使半连接队列没有, 也同样丢包 `tcp_v4_conn_request()`
```c++
int inet_csk_reqsk_queue_is_full(const struct request_sock_queue *queue) {
    return inet_csk_reqsk_queue_len(sk) >= sk->sk_max_ack_backlog;
}

int tcp_v4_conn_request(struct sock *sk, struct sk_buff *skb) {
    bool want_cookie = false;
    // inet_csk_reqsk_queue_is_full() 判断半连接队列是否 full
    // 该函数中, 半连接队列等同全连接队列大小
    if ((sysctl_tcp_syncookies == 2 ||
        inet_csk_reqsk_queue_is_full(sk)) && !isn) {
        want_cookie = tcp_syn_flood_action(sk, skb, "TCP");
        if (!want_cookie)
            goto drop;
    }
    // 全连接队列满了, 这里半连接队列不一定满, 直接丢包
    if (sk_acceptq_is_full(sk)) {
        NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENOVERFLOWS);
        goto drop;
    }
    // ...
	if (!want_cookie && !isn) {
		// 没有开启 sysctl_tcp_syncookies 时
        // sysctl_max_syn_backlog - 当前队列大小 < sysctl_max_syn_backlog*0.25
        // 即 sysctl_max_syn_backlog*0.75
        // 就会丢弃三次握手
		if (!net->ipv4.sysctl_tcp_syncookies &&
		    (net->ipv4.sysctl_max_syn_backlog - inet_csk_reqsk_queue_len(sk) <
		     (net->ipv4.sysctl_max_syn_backlog >> 2)) &&
		    !tcp_peer_is_proven(req, dst)) {
            // ...
			goto drop_and_release;
		}
    }
    // ...
}
```

## 三次握手 tcp_v4_syn_recv_sock
```c++
struct sock *tcp_v4_syn_recv_sock(struct sock *sk, struct sk_buff *skb,
				  struct request_sock *req,
				  struct dst_entry *dst){
    tcp_sync_mss(newsk, dst_mtu(dst)); // 初始化的 mss 就是目的 interface 的 mtu

}

struct sock *tcp_check_req(struct sock *sk, struct sk_buff *skb,
			   struct request_sock *req,
			   struct request_sock **prev,
			   bool fastopen) {
    // icsk_af_ops->syn_recv_sock 指向 tcp_v4_syn_recv_sock
	child = inet_csk(sk)->icsk_af_ops->syn_recv_sock(sk, skb, req, NULL);
	if (child == NULL)
		goto listen_overflow;

	inet_csk_reqsk_queue_unlink(sk, req, prev);
	inet_csk_reqsk_queue_removed(sk, req);

	inet_csk_reqsk_queue_add(sk, req, child);
}

void inet_csk_reqsk_queue_added(struct sock *sk, const unsigned long timeout) {
    // 入列 icsk_accept_queue
	if (reqsk_queue_added(&inet_csk(sk)->icsk_accept_queue) == 0)
		inet_csk_reset_keepalive_timer(sk, timeout);
}

int tcp_rcv_state_process(struct sock *sk, struct sk_buff *skb,
			  const struct tcphdr *th, unsigned int len) {
    if (tcp_check_req(sk, skb, req, NULL, true) == NULL)
        goto discard;
    switch (sk->sk_state) {
        case TCP_ESTABLISHED:
            tcp_data_queue(sk, skb);
            queued = 1;
            break;
        }
    }
}

static void tcp_data_queue(struct sock *sk, struct sk_buff *skb) {
    if (eaten <= 0) {
queue_and_out:
        if (eaten < 0 &&
            tcp_try_rmem_schedule(sk, skb, skb->truesize))
            goto drop;

        eaten = tcp_queue_rcv(sk, skb, 0, &fragstolen);
    }
}
```

## 一次握手 tcp_v4_conn_request
```c++
// 收到首次握手
tcp_v4_conn_request() {
    // 准许入列的校验
    // 包括队列是否满了, tcp 一些option的正确性 等

    // 开了 fastopen 且 cookie 有效; 走fastopen 逻辑
	fastopen = !want_cookie &&
		   tcp_try_fastopen(sk, skb, req, &foc, dst);
    // 发送 二次握手报文
	err = tcp_v4_send_synack(sk, dst, req,
				 skb_get_queue_mapping(skb), &foc);
    // 没开 fastopen; 则进入半连接队列
	if (!fastopen) {
        // ...
		inet_csk_reqsk_queue_hash_add(sk, req, TCP_TIMEOUT_INIT);
	}
}
// 进入半连接队列 步骤1
void inet_csk_reqsk_queue_hash_add(struct sock *sk, struct request_sock *req, unsigned long timeout) {
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct listen_sock *lopt = icsk->icsk_accept_queue.listen_opt;
    // 求得hash值
	const u32 h = inet_synq_hash(inet_rsk(req)->ir_rmt_addr,
				     inet_rsk(req)->ir_rmt_port,
				     lopt->hash_rnd, lopt->nr_table_entries);
    // 插入hash表
	reqsk_queue_hash_req(&icsk->icsk_accept_queue, h, req, timeout);
    // 进入半连接队列
	inet_csk_reqsk_queue_added(sk, timeout);
}
// 进入半连接队列 步骤2
void inet_csk_reqsk_queue_added(struct sock *sk, const unsigned long timeout) {
    // 入列 icsk_accept_queue
	if (reqsk_queue_added(&inet_csk(sk)->icsk_accept_queue) == 0)
		inet_csk_reset_keepalive_timer(sk, timeout);
}
```

## 半连接队列 全连接队列的数据定义
```c++
// 半连接队列只放在 inet_connection_sock 的 isck_accept_queue 中
// 实际上, 全连接socket, 半连接socket都会放在 icsk_accept_queue 中
struct inet_connection_sock {
	/* inet_sock has to be the first member! */
	struct inet_sock	  icsk_inet;
	struct request_sock_queue icsk_accept_queue;
}

// 而全连接队列多出一个副本放在 sock 的接收队列里面
struct sock {
	struct sock_common	__sk_common;
    struct sk_buff_head	sk_receive_queue;
}
```