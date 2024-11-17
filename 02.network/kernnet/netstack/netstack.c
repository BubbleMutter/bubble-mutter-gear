参考 https://blog.csdn.net/zxorange321/article/details/75676063
参考 https://www.cs.unh.edu/cnrg/people/gherrin/linux-net.html#tth_sEc3.2.2
参考 https://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/

遗留问题:
pskb_may_pull()            该逻辑有些复杂, 看不懂
SDR messages? route cache? route cache entries? route cache reject entries?
ip_route_input_slow()      中 loopback 地址处理

instructions
1. 初始化流程
2. 关键结构体
3. socket 创建
4. 发包过程 write > ... > 网卡驱动发包
5. 收包过程 硬件中断 > ip_rcv > tcp_rcv/udp_rcv > read

net_proto_family // 协议簇回调, 只有create方法  /* This is the 1st argument to socket(2) */
proto_ops        // socket操作接口, socket -> inet_socket, /* This is the 2nd argument to socket(2) */
proto            // sock操作接口,   sock -> socket,
net_protocol     // ip  层回调, 操作对象:sk_buff, 回调协议tcp/udp/icmp/igmp/sctp
packet_type      // link层回调, 操作对象:sk_buff, 回调协议ip/ip6/arp/rarp/bootp/pppoes
napi_struct      // 收包控制
sk_buff          // 描述 一个报文的所有信息
sk_buff_head     // 内核缓存 sk_buff 的队列
net_device       // 描述 一个网络设备
softnet_data     // 描述一个CPU的收包队列

msghdr { iovec }
socket {
  file { file_operations { read write } }
  proto_ops { ioctl bind accept }
  sock {
    sock_common { proto { ioctl bind accept } }
    sk_buff_head::sk_write_queue
    sk_buff_head::sk_receive_queue
  }
}

tcp_sock { inet_connection_sock { inet_sock { sock { sock_common } } } }
udp_sock { inet_sock { sock { sock_common } }  }

inet_protosw { proto_ops proto }

packet_type {
   net_device
   func
}

softnet_data {
  sk_buff_head::input_pkt_queue
  napi_struct {
    poll
  }
}

/**
 * sk_buff 关键的成员
 * @vlan_tci: vlan tag control information
 *
 * @len: Length of actual data                 一层协议的数据总长度 包括协议的 header+buffer
 * @data_len: Data length
 * @mac_len: Length of link layer header
 * @hdr_len: writable header length of cloned skb
 *
 * @sk: Socket we are owned by
 * @_skb_refdst: destination entry (with norefcount bit)  TODO: 什么意思?
 *
 *
 * @transport_header: Transport layer header   传输层相对head的偏移  skb->head + skb->transport_header
 * @network_header: Network layer header       网络层相对head的偏移  skb->head + skb->network_header
 * @mac_header: Link layer header              链路层相对head的偏移  skb->head + skb->mac_header
 *
 * @tail: Tail pointer                         数据帧的尾部指针, 即payload后一bytes
 * @end: End pointer                           数据帧的终止指针
 * @head: Head of buffer                       数据帧的头指针, 每一层中都一样, 几乎是readonly
 * @data: Data head pointer                    一层协议的数据指针
 *
 * head | data | tail | end
 */
sk_buff {
    sock
    net_device
    sk_buff_data_t::transport_header
    sk_buff_data_t::network_header
    sk_buff_data_t::mac_header
}

// "剥掉" 一层协议, len expected 协议头的长度
unsigned char *__skb_pull(struct sk_buff *skb, unsigned int len)
{
	skb->len -= len;                  // 下一层协议的数据长度 减少
	BUG_ON(skb->len < skb->data_len);
	return skb->data += len;          // 下一层协议的数据指针 后移
}