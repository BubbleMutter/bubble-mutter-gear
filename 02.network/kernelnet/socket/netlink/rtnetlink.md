# 1. 注册 protocol
1. rtnl_register > `__rtnl_register(int protocol, int msgtype)`
2. `rtnl_msg_handlers[protocol]`
3. protocol 对应 `ifinfomsg->ifi_family`
4. msgtype  对应 `nlmsghdr->nlmsg_type`

# 2. 内核收包
1. rtnetlink_net_init 中创建 rtnetlink
2. `netlink_kernel_create(net, NETLINK_ROUTE, &cfg);`
3. 对应用户态 `socket(AF_NETLINK, socket_type, NETLINK_ROUTE);`
1. 其中收包函数 rtnetlink_rcv > rtnetlink_rcv_msg

# 3. 用户态 sendmsg 到 内核收包调用链
(struct proto_ops)->sendmsg
netlink_sendmsg
netlink_unicast
   (rtnetlink 调用 netlink_kernel_create 创建时; 赋上了 NETLINK_F_KERNEL_SOCKET)
   (所以 netlink_is_kernel 这里会返回 true)
netlink_unicast_kernel, 这个函数只会返回 skb->len, 无论 netlink_rcv 回调正确与否
nlk->netlink_rcv(skb)
   (rtnetlink 调用 netlink_kernel_create 创建时; 附上了 netlink_rcv = cfg->input )
   (即 rtnetlink_rcv)

# 99. experiment
+ 构造 rta 报文踩了太多坑了, 一些经验记录一下
## 99.1 brport_disable_flood
1. 干掉桥的子接口的 3个广播属性
```c++
int brport_disable_flood(const char *ifname, const int ifindex) {
    // 1. 构造 nlmsg 的头
    struct {
        struct nlmsghdr nlh;
        char buf[LEN_128];
    } req = {
        .nlh = {.nlmsg_type = RTM_SETLINK /* 设置 */, .nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
                .nlmsg_pid = 0/* 表示发向内核 */, .nlmsg_flags   = NLM_F_REQUEST  /* netlink_rcv_skb */ },
        .buf = { 0 }
    };

    // 2. 构造 ifinfomsg (man 3 rtnetlink) (rtnl_bridge_setlink)
    struct ifinfomsg *ifm = (struct ifinfomsg *)NLMSG_DATA(&req.nlh);
    ifm->ifi_family = PF_BRIDGE; /* 桥属性 */
    ifm->ifi_index = ifindex; /* 指定端口 */
    ifm->ifi_change = 0xffffffff;

    // 3. 构造 首个 rtattr 指示设置端口作为 桥子接口的属性 (br_setlink)
    struct rtattr *head = IFLA_RTA(NLMSG_DATA(&req.nlh));
    head->rta_type = IFLA_PROTINFO | NLA_F_NESTED;
    head->rta_len  = NLA_ALIGN(RTA_LENGTH(0));

    // 4. 构造 n个 rtattr 指示待设置的属性 flags (br_setport, br_set_port_flag)
    int type[] = { IFLA_BRPORT_UNICAST_FLOOD, IFLA_BRPORT_MCAST_FLOOD, IFLA_BRPORT_BCAST_FLOOD };
    struct rtattr *body = RTA_DATA(head);
    for (int i = 0, attrlen = 0; i < sizeof(type)/sizeof(*type); ++i) {
        body->rta_type = type[i];
        body->rta_len = NLA_ALIGN(RTA_LENGTH(sizeof(uint8_t))); // br_set_port_flag
        *(uint8_t *)RTA_DATA(body) = 0;
        head->rta_len += body->rta_len;
        body = RTA_NEXT(body, attrlen);
    }
    req.nlh.nlmsg_len += head->rta_len;

    int fd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    // 这里不能手贱 bind, 不然一定有问题
    ret = send(fd, &req, req.nlh.nlmsg_len, 0);
    if (ret < 0)
        fprintf(stderr, "port %s failed to brport_disable_flood\n", ifname);
    return -1;
}
```
