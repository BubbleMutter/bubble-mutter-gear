
1. `br_handle_frame()` 桥收包入口
    1. 组播报文泛洪 br_flood_forward
2. `br_flood_forward()` > `__br_forward()`; 
    1. NF_HOOK(NFPROTO_BRIDGE, NF_BR_FORWARD, skb, indev, skb->dev, br_forward_finish);
    2. 入接口是桥接口; 出接口是 sbk->dev;
3. `br_nf_forward_ip()` // 这个钩子是挂在 NFPROTO_BRIDGE::NF_BR_FORWARD 上的
    1. NF_HOOK(pf, NF_INET_FORWARD, skb, brnf_get_logical_dev(skb, in), parent, br_nf_forward_finish);
    2. 入接口是原始物理口; 出接口是 parent = bridge_parent(out); 即桥接口
4. `br_nf_forward_finish()`
    1. NF_HOOK_THRESH(NFPROTO_BRIDGE, NF_BR_FORWARD, skb, in, skb->dev, br_forward_finish, 1);
    2. 入接口是原始物理口; 出接口是 应该出去的物理口
5. `br_forward_finish()`
    1. NF_HOOK(NFPROTO_BRIDGE, NF_BR_POST_ROUTING, skb, NULL, skb->dev, br_dev_queue_push_xmit);

-------------
也正是因为 3. 所以: 
`nft add rule inet filter forward meta oifname "br_ext" pkttype multicast drop` 才能阻止br_ext的泛洪