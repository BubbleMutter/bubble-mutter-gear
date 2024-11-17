void setsockopt_example() {
    // 发送 / 接收超时等待
    // Note: 如果 tv 都是零, 即不超时, 并不是 non-blocking
    //       而且, 非阻塞是fd层的玩意, 所以要用 fnctl 设置
    struct timeval tv = { .tv_sec = 1, .tv_usec = 0 };
    setsockopt(sockfd, SOL_SOCET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(sockfd, SOL_SOCET, SO_SNDTIMEO, &tv, sizeof(tv));

    

}

#if 0 // vlan 优先级
SO_PRIORITY，设置在套接字发送的所有包的协议定义优先权。Linux通过这一值来排列网络队列。
这个值在0到6之间（包括0和6），由option_value指定。赋给sock->sk->sk_priority
#endif