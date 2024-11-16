[TOC]
# xdp

# 3 running mode
1. 原生模式: 驱动早期的路径挂钩子点. 10G 以上的网卡基本支持
2. 卸载模式: 直接把 eBPF 字节码卸载到网卡硬件上. 少有的 netronome 智能网卡支持
3. 通用模式: 驱动收包结束后挂钩子点. 由内核协议栈支持

# implement (in protocol-stack)
```txt
\-- __netif_receive_skb_core
 \-- do_xdp_generic
  \-- netif_receive_generic_xdp
   |---- XDP_TX -------- generic_xdp_tx
   |---- XDP_REDIRECT -- xdp_do_generic_redirect -> generic_xdp_tx
   |---- XDP_PASS
   |---- XDP_DROP
```
## __netif_receive_skb_core
```c++
int __netif_receive_skb_core(struct sk_buff **pskb, bool pfmemalloc, struct packet_type **ppt_prev) {
    // ...
    if (static_branch_unlikely(&generic_xdp_needed_key)) {
		int ret2;

		preempt_disable();
		ret2 = do_xdp_generic(rcu_dereference(skb->dev->xdp_prog), skb);
		preempt_enable();

		if (ret2 != XDP_PASS) {
			ret = NET_RX_DROP;
			goto out;
		}
		skb_reset_mac_len(skb);
	}
    // ...
}
```
1. 协议栈统一收包函数 `__netif_receive_skb_core()`
2. 全局 xdp 使能开关 `generic_xdp_needed_key`
3. 统一入口 `do_xdp_generic()`, 传入 skb->dev->xdp_prog
    + 因此, xdp 程序是挂载到 net_device, 一一对应
4. 如果返回 XDP_PASS 则继续走协议栈, 否则 drop

## do_xdp_generic
```c++
int do_xdp_generic(struct bpf_prog *xdp_prog, struct sk_buff *skb) {
	if (xdp_prog) {
		struct xdp_buff xdp;
		u32 act;
		int err;

		act = netif_receive_generic_xdp(skb, &xdp, xdp_prog);
		if (act != XDP_PASS) {
			switch (act) {
			case XDP_REDIRECT:
				err = xdp_do_generic_redirect(skb->dev, skb, &xdp, xdp_prog);
				if (err)
					goto out_redir;
				break;
			case XDP_TX:
				generic_xdp_tx(skb, xdp_prog);
				break;
			}
			return XDP_DROP;
		}
	}
	return XDP_PASS;
out_redir:
	kfree_skb(skb);
	return XDP_DROP;
}
EXPORT_SYMBOL_GPL(do_xdp_generic);
```
1. 当 xdp_prog 为空, 即 net_device 无 xdp_prog, 直接 XDP_PASS
2. `netif_receive_generic_xdp()` 中
    1. 调用 `bpf_prog_run_xdp(xdp_prog, xdp);` 执行 eBPF 字节码, 得到 `XDP_*` 返回码
3. 经过 `netif_receive_generic_xdp()` 后, 根据 `XDP_*` 返回码:
    1. XDP_PASS, 即继续走 内核协议栈
    2. XDP_TX, 走 XDP 的发包 (`generic_xdp_tx()`)
    3. XDP_REDIRECT, 走 XDP 重定向 (`xdp_do_generic_redirect()`)

## xdp_do_generic_redirect
```c++
int xdp_do_generic_redirect(struct net_device *dev, struct sk_buff *skb,
			    struct xdp_buff *xdp, struct bpf_prog *xdp_prog) {
	struct net_device *fwd;
    // ...
	fwd = dev_get_by_index_rcu(dev_net(dev), index);
    // ...
	skb->dev = fwd;
    // ...
	generic_xdp_tx(skb, xdp_prog);
	return 0;
err:
    // ...
}
```
1. 重定向的核心是逻辑是, 把 skb 通过 `generic_xdp_tx()` 发送到 `skb->dev`
2. 即, eBPF 程序返回 XDP_REDIRECT 时, 要求把 `skb->dev` 设置为目标 `net_device`

## generic_xdp_tx
```c++
void generic_xdp_tx(struct sk_buff *skb, struct bpf_prog *xdp_prog) {
	// ...
    netdev_start_xmit(skb, dev, txq, 0);
    // ...
}
```
1. 绕过 协议栈/tc, 直接走 device 发包方法

# implement (in kernel-driver)
driver list https://github.com/iovisor/bcc/blob/master/docs/kernel-versions.md#xdp
## drivers/net/ethernet/intel/i40e/i40e_txrx.c
1. 函数 `i40e_run_xdp`
2. 在收包上协议栈前, 实现形同 `do_xdp_generic()` 的逻辑
    1. 调用 `bpf_prog_run_xdp()`, 其中 eBFP 程序是驱动层的, 不是内核层
    2. 根据 `XDP_*` 返回码, 区别处理 XDP_PASS / XDP_TX / XDP_REDIRECT
    3. XDP_PASS 收包到协议栈
    4. XDP_TX 走驱动层发包
    5. XDP_REDIRECT 调用 `xdp_do_redirect()` 发包到目标 nic
