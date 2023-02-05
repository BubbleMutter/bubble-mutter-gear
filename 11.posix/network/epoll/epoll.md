[TOC]
# 1. 系统调用入口
`SYSCALL_DEFINE6(epoll_pwait)` > `SYSCALL_DEFINE4(epoll_wait)`
`SYSCALL_DEFINE4(epoll_ctl)`
`SYSCALL_DEFINE1(epoll_create)` > `SYSCALL_DEFINE1(epoll_create1)`

# 2. 对象定义
1. 用户态 1个 epoll-fd 对应 内核态 1个 `struct eventpoll;`
2. 用户态 1个 待监听fd 对应 内核态 1个 `struct epitem;`
3.  1个 `struct eventpoll;` 关联多个 `struct epitem;`
4. 多个 `struct epitem;`    用 红黑树 和 链表的方式储存
    1. 其中, rbtree 的 索引是 `epitem->ffd` (由 `struct file *` 指针 和 `int fd` 共同构成)
       question 2021/3/29: 为什么要用 ffd, 而不是只用 fd ??
       避免 重复调用 `fdget(fd)` 查照 `struct fd` 中的 `struct file *` 对象
       `fdget(fd)` 是耗时操作
```c++
struct eventpoll {
	spinlock_t lock; /* Protect the access to this structure */
	/*
	 * This mutex is used to ensure that files are not removed
	 * while epoll is using them. This is held during the event
	 * collection loop, the file cleanup path, the epoll file exit
	 * code and the ctl operations.
	 */
	struct mutex mtx;
	wait_queue_head_t wq; /* Wait queue used by sys_epoll_wait() */
	wait_queue_head_t poll_wait; /* Wait queue used by file->poll() */
	struct list_head rdllist;  /* 准备就绪, 即发生了事件但还没处理的 fd 列表 */
	struct rb_root_cached rbr; /* RB tree root used to store monitored fd structs */
                               /* 以 struct *file, int fd 合并为 `struct epoll_filefd` 作为 key
                                * 存 struct *epitem 对象
                                */

	struct epitem *ovflist;	  /* 所有 struct *epitem 对象 */
	struct wakeup_source *ws; /* wakeup_source used when ep_scan_ready_list is running */
	struct user_struct *user; /* The user that created the eventpoll descriptor */
                              /* 这指的是 user 描述; 即一个 登录用户的内核对象, 包含 uid */
	struct file *file;        /* the file object epoll-fd */

	/* used to optimize loop detection check */
	int visited;
	struct list_head visited_list_link;

#ifdef CONFIG_NET_RX_BUSY_POLL
	/* used to track busy poll napi_id */
	unsigned int napi_id;
#endif
};
```

# epoll_create > epoll_create1 主要逻辑
```c++
SYSCALL_DEFINE1(epoll_create1, int, flags) {
    int error, fd;
	struct eventpoll *ep = NULL;
	struct file *file;
    // ...
    /* 初始化 eventpoll 底层对象 */
    error = ep_alloc(&ep);
    // ...
	fd = get_unused_fd_flags(O_RDWR | (flags & O_CLOEXEC)); /* 从进程的 fd 表中申请 fd */
    // ...

    /* 创建匿名的 inode 节点, 即生成 file 对象,
     * file 对象的操作方法为 eventpoll_fops
     * ep 为对象的操作方法的 上下文对象; 即 file->private_data = ep;
     */
	file = anon_inode_getfile("[eventpoll]", &eventpoll_fops, ep,
				 O_RDWR | (flags & O_CLOEXEC));
    ep->file = file;
    // ...
	fd_install(fd, file); // 绑定 fd 和 file
	return fd;
}
```

# epoll_ctl 主要逻辑
```c++
SYSCALL_DEFINE4(epoll_ctl, int, epfd, int, op, int, fd, struct epoll_event __user *, event) {
	struct eventpoll *ep;    // epfd 关联的 eventpoll 底层对象
	struct epitem *epi;
	struct epoll_event epds; // 用户态参数, 指示一个 事件
    struct fd f, tf;

    // 获取 struct epoll_event 参数
    if (ep_op_has_event(op) &&
	    copy_from_user(&epds, event, sizeof(struct epoll_event)))
		goto error_return;
    // ...

    // 获取 epoll-fd 的 file-description 及其 file 对象
    f = fdget(epfd);
	if (!f.file)
		goto error_return;
    // ...

    // 获取 监听 fd 的 file-description 及其 file 对象
	tf = fdget(fd);
	if (!tf.file)
		goto error_fput;
    // ...

    // 监听 fd 的 file_operations 必须实现 ->poll 方法 (一般由 fs 的驱动实现)
    if (!tf.file->f_op->poll)
		goto error_tgt_fput;
    // ...

    ep = f.file->private_data; // 这个 private_data 在创建时, 调用 anon_inode_getfile 传过去的
    // ...

    /**
     * ep 表示 eventpoll 底层对象
     * tf.file 指示待操作 fd
     * fd      指向待操作 fd
     *
     * 在 ep->rbr 红黑树中, 通过 fd 找到 epi 对象
     */
	epi = ep_find(ep, tf.file, fd);

    switch (op) {
	case EPOLL_CTL_ADD:
		if (!epi) {
			epds.events |= POLLERR | POLLHUP;
            // 创建 epi 对象; 插入到 ep->rbr 和 ep->ovflist 中
			error = ep_insert(ep, &epds, tf.file, fd, full_check);
		}
        // ...
		break;
	case EPOLL_CTL_DEL:
		if (epi)
            // 把 epi 对象从 ep->rbr 和 ep->ovflist 去掉; 并销毁
			error = ep_remove(ep, epi);
        // ...
		break;
	case EPOLL_CTL_MOD:
		if (epi) {
			if (!(epi->event.events & EPOLLEXCLUSIVE)) {
				epds.events |= POLLERR | POLLHUP;
				error = ep_modify(ep, epi, &epds);
			}
		} // ...
		break;
	}
}
```

# epoll_pwait > epoll_wait 主要逻辑
```c++
SYSCALL_DEFINE4(epoll_wait, int, epfd, struct epoll_event __user *, events,
		int, maxevents, int, timeout)
{
	struct fd f;
	struct eventpoll *ep;
	f = fdget(epfd); // 根据传入 fd 取得内核文件对象 句柄
	/* ... */
	ep = f.file->private_data; // 取得 eventpoll 私有对象
	                           // 由 epoll_create 调用 anon_inode_getfile 赋值
	error = ep_poll(ep, events, maxevents, timeout);
	/* ... */
}
static inline int ep_events_available(struct eventpoll *ep)
{
	// 即当事件发生时, fd 将加入到 ep->rdllist, ep->ovflist 中
	return !list_empty(&ep->rdllist) || ep->ovflist != EP_UNACTIVE_PTR;
}
static int ep_send_events(struct eventpoll *ep,
			  struct epoll_event __user *events, int maxevents);
static int ep_scan_ready_list(struct eventpoll *ep,
			      int (*sproc)(struct eventpoll *,
					   struct list_head *, void *),
			      void *priv, int depth, bool ep_locked);
static int ep_send_events_proc(struct eventpoll *ep,
				   struct list_head *head, void *priv);
static inline unsigned int ep_item_poll(struct epitem *epi, poll_table *pt)
{
	pt->_key = epi->event.events;
	// 由此可以得知 文件访问 poll 返回的是事件 bitmaps
	// epi->event.events 充当 事件掩码, 检查事件类型是否满足条件
	// 事件bits 包括: POLLIN / POLLOUT / POLLERR 等类型
	return epi->ffd.file->f_op->poll(epi->ffd.file, pt) & epi->event.events;
}
```
1. 获取 epfd 中的 eventpoll 对象传入到 ep_poll
3. `ep_poll` 主要逻辑
	1. 使得进程进入 TASK_INTERRUPTIBLE 状态阻塞
	2. 调用 ep_events_available(ep); 等待直到有 fd 就绪
	3. 调用 ep_send_events > ep_scan_ready_list 扫描 fd 判定满足条件
4. `ep_send_events`
	1. 简单传递参数到 ep_scan_ready_list
	2. 其中 ep_send_events_proc 作为 参数 sproc
5. `ep_scan_ready_list`
	1. 取出 ep->rdllist 到临时链表 txlist
	2. 置空 ep->ovflist; 这使得在处理 txlist 过程中,
	   有新事件发生时, 将会重新加入到 ep->ovflist 中.
	3. 调用 sproc 处理 txlist, 并传递用户态传出参数 priv
	4. 重新把 ep->ovflist 节点插入 ep->rdllist 链表
	   因为 sproc 处理 过程中, 可能异步产生了事件,
	   异步产生的事件首先传入到 ep->ovflist 中
	5. 再把 txlist 重新拼接到 ep->rdllist 中
	   这是因为 txlist 在 sproc 中可能会被丢弃部分节点
6. `ep_send_events_proc`
	1. 遍历 txlist 链表
	2. 把节点 epi 从 txlist 链表去掉
	3. 调用 ep_item_poll 检查是否有事件发生
	4. 针对有事件发生的 epi, 返回用户态传出参数的数组中
	   如果是水平触发的 epi, 还需要重新入列 ep->rdllist
	5. 针对没时间发生的 epi, 从 txlist 去掉后就不见了

# ext4fs 的 f_op->poll 方法
```c++

```

# socket 的 f_op->poll 方法
```c++
static const struct file_operations socket_file_ops = {
	.poll =		sock_poll,
};
static unsigned int sock_poll(struct file *file, poll_table *wait)
{
	unsigned int busy_flag = 0;
	sock = file->private_data;

	if (sk_can_busy_loop(sock->sk)) {
		/* this socket can poll_ll so tell the system call */
		busy_flag = POLL_BUSY_LOOP;

		/* once, only if requested by syscall */
		if (wait && (wait->_key & POLL_BUSY_LOOP))
			sk_busy_loop(sock->sk, 1);
	}
	return busy_flag | sock->ops->poll(file, sock, wait);
}
// 具体由一下方法返回 事件 bitmaps
static const struct proto_ops packet_ops = {
	.poll =		packet_poll,
};
const struct proto_ops inet_stream_ops = {
	.poll		   = tcp_poll,
};
const struct proto_ops inet_dgram_ops = {
	.poll		   = udp_poll,
};
static const struct proto_ops inet_sockraw_ops = {
	.poll		   = datagram_poll,
};
static const struct proto_ops netlink_ops = {
	.poll =		datagram_poll,
};
```