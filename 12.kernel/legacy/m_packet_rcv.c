// 之前设计了一个 packet_rcv 有问题
// 1. 入口处没有检查 skb_share_check
// 2. 出错时; 没有 kfree(skb)
// 3. dev_queue_xmit(skb) 出错时; kfree(skb)
// 这里更正一下
int m_packet_rcv(struct sk_buff *skb, struct net_device *dev,
	    struct packet_type *pt, struct net_device *orig_dev) {

    if (unlikely(!(skb = skb_share_check(skb, GFP_ATOMIC))))
		goto drop;

    // 业务1: 魔数校验
    if (!match_magic(skb))
        goto drop_free;
    // 业务2: 允许操作
    if (!allow_operation(skb))
        goto drop_free;

    if (dev_queue_xmit(skb) < 0) {
        mprint_error("dev_queue_xmit error\n");
        goto drop;
    }

    return NET_RX_SUCCESS;
drop_free:
    kfree(skb);
drop:
    return NET_RX_DROP; // 上层 __netif_receive_skb_core 不会根据返回值 kfree
}

// 实际上反映了一个本质问题: 
// 协议栈处理 skb 的函数; 开发者必须关注此函数是否释放 skb
// 更加简单的原则:
// 每层函数 (inet层, packet层, ip层, driver层 等) 保证:
// 1. 入口处检查 share
// 2. skb 要么被释放, 要么 进入下一层函数

//参考 arp_rcv, mpls_forward，phonet_rcv，phonet_rcv