二层网卡驱动层, 同步三层表的方案
# 已知场景
1. interface down 了
2. arp RECHABLE 到 STALE 时; 销毁
3. arp RECHABLE 更新时; 插入
    + 对应 arp_process,
    + 最底下 neigh_update(n, sha, state, override ? NEIGH_UPDATE_F_OVERRIDE : 0, 0);
4. 手动删掉
5. 手动增加

## 场景堆栈
neigh_update_notify
    < neigh_update
        < neigh_delete 手动删除
            < rtnl_register(PF_UNSPEC, RTM_DELNEIGH, neigh_delete, NULL, 0);
        < neigh_add    手动增加
            < rtnl_register(PF_UNSPEC, RTM_NEWNEIGH, neigh_add, NULL, 0);
        < neigh_event_ns
            < arp_process
        < arp_process  学习为 rechable 状态
        < arp_req_set    手动增加 < arp_ioctl
        < arp_invalidate 手动删除 < arp_ioctl
    < neigh_timer_handler 超时

neigh_update_notify
    > __neigh_notify 通知到用户态netlink

# 方案1 扩展 neighbor 对象; 注册 netnotifier 消息回调
1. neighbor 对象加成员
    1. `bkn_fdb_skb { int magic_head; int flags; uint32_t interface; int magic_tail; }`
2. 注册 neighbor 更新消息回调
1. skb.cb, 在 driver 层赋值上
    1. `bkn_fdb_skb { int magic_head; int flags; uint32_t interface; int magic_tail; }`
2. arp_process 学习为 rechable 状态时
    1. 校验 skb.cb 的 magic
    2. 然后 初始化 neighbor 的 bkn_fdb_skb
    3. 从而使得 neighbor 更新消息回调中, 可以拿到 interface (这个是ge口)
5. 消息回调 (使用 netevent_notifier)
    1. 判断 neigh 是 NUD_REACHABLE 状态; 检查 magic; 校验 interface; 学习到 bkn_tbl 中
    2. 判断 neigh 是 NUD_FAILED  状态时; 检查 magic; 校验 interface; 从 bkn_tbl 中删除
## 遗留问题:
1. bkn 处理 手动增加, 手动删除的场景
    + 原因1: 这些场景的 neighbor->dev 可能是3层口的 ge口; 冗余


# ng version
## 表同步
## 返回用户层

```c++
// neigh_update 关键逻辑走读
int neigh_update(struct neighbour *neigh, const u8 *lladdr, u8 new, u32 flags, u32 nlmsg_pid) {
    old    = neigh->nud_state;
    /* 非 NEIGH_UPDATE_F_ADMIN (表示用户态 设置)
     * 且 之前状态是静态 arp
     * 则 不更新
     * Note: a) 系统调用设置arp表项时 neigh_add/neigh_delete/arp_req_delete/arp_req_set
     *          都会有 NEIGH_UPDATE_F_ADMIN
     *       b) 即 收到arp报文更新/iface-down/iface销毁 没有该 flags
     */
    if (!(flags & NEIGH_UPDATE_F_ADMIN) &&
        (old & (NUD_NOARP | NUD_PERMANENT)))
	    goto out;

    /* neigh 从有效 变 无效; 将会更新
     * #define NUD_VALID (NUD_PERMANENT|NUD_NOARP|NUD_REACHABLE|NUD_PROBE|NUD_STALE|NUD_DELAY)
     * #define NUD_CONNECTED (NUD_PERMANENT|NUD_NOARP|NUD_REACHABLE)
     */
	if (!(new & NUD_VALID)) {
		neigh_del_timer(neigh);
		if (old & NUD_CONNECTED)
			neigh_suspect(neigh);
		neigh->nud_state = new;
		err = 0;
		notify = old & NUD_VALID;
		if ((old & (NUD_INCOMPLETE | NUD_PROBE)) &&
		    (new & NUD_FAILED)) {
			neigh_invalidate(neigh);
			notify = 1;
		}
		goto out;
	}
}
```