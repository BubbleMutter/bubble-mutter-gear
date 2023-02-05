[TOC]
# 1. 对象定义
```c++
struct skb_shared_info {
	/* skb->data 数据的引用; 区别于 skb->users 引用计数
	 * __alloc_skb() 中清零; __skb_clone() 中加加一 */
	atomic_t	dataref;         // 用于 skb_cloned(skb) 判断
	/* Intermediate layers must ensure that destructor_arg
	 * remains valid until skb destructor */
	void *		destructor_arg;
	/* must be last field, see pskb_expand_head() */
	skb_frag_t	frags[MAX_SKB_FRAGS];
};

struct sk_buff {
    unsigned int len, data_len;
    __u16 mac_len, hdr_len;
	/* These elements must be at the end, see alloc_skb() for details.  */
	sk_buff_data_t		tail;
	sk_buff_data_t		end;   // 实际指向 struct skb_shared_info
	unsigned char		*head,
                        *data;
	/* skb 本身的引用数; 区别于 dataref
	 * skb_shared(skb) 判断
	 * skb_get(skb)    持有, ++
	 * skb_unref(skb)  释放, --, 需要手动 free */
	refcount_t		users;

	/* 销毁回调 */
	void			(*destructor)(struct sk_buff *skb);

	__u8	    cloned:1,              // 用于 skb_cloned(skb) 判断
				nohdr:1,
				fclone:2,
				peeked:1,
				head_frag:1,
				xmit_more:1,
				pfmemalloc:1;

	__be16			protocol;         // 二层以太网协议; 由网卡驱动调用 eth_type_trans 赋值
	__u16			transport_header; // inet 层 skb_reset_transport_header
	__u16			network_header;   // ip   层 skb_reset_network_header
	__u16			mac_header;       // mac  层 skb_reset_mac_header
};

skb_headroom(skb)             // head 与 data 之间的空间
skb_tailroom(skb)             //  end 与 tail 之间的空间
skb_availroom(skb)            //  end 与 tail 之间, 去掉 reserved_tailroom 剩余的空间

skb_end_pointer(skb)
skb_end_offset(skb)

skb_tail_pointer(skb)
skb_tail_offset(skb)
skb_set_tail_pointer(skb)
skb_reset_tail_pointer(skb)
```
1. head 完整buff头指针
2. data 该层报文头指针
3. tail 该层报文的结尾
4. end  完整buff末指针
	1. 并非简单的 head + len
	2. 而是一块 `struct skb_shared_info` 内存
	3. 由 `__alloc_skb()` 申请时, 额外分配出来
	4. 由 `skb_shinfo(skb)` 返回

# 2. 销毁方法
```c++
void dev_kfree_skb(struct sk_buff *skb);     // 非中断上下文中
void dev_kfree_skb_irq(struct sk_buff *skb); // 中断上下文中
void dev_kfree_skb_any(struct sk_buff *skb); // 内嵌判断逻辑 自适应free; 耗性能
void kfree_skb(struct sk_buff *skb);         // 中断/非中断 上下文
```

# 3. 创建方法
```c++
struct sk_buff *alloc_skb(unsigned int len, int priority); // 申请
struct sk_buff *dev_alloc_skb(unsigned int length);        // 申请, GFP_ATOMIC

/**
 * @brief 深复制, 引用计数 users 为1
 */
struct sk_buff *skb_copy(const struct sk_buff *skb, gfp_t priority);


/**
 * @brief 浅复制, skb->data 指向相同的一块
 * @note  其他各种标记, 偏移都作为成员拷贝
 *        新skb克隆标记置一 new->cloned = 1;
 *        新skb使用标记置一 new->users  = 1;
 *        旧skb数据引用加一 atomic_inc(skb_shinfo(old)->dataref);
 *                          实际上浅复制 skb_shinfo(new) 同于旧的
 *        新skb销毁回调置空 new->destructor = NULL;
 */
struct sk_buff *skb_clone(struct sk_buff *skb, gfp_t priority);

/**
 * @brief 拷贝并扩展指定空间,
 *        如果 newheadroom = newtailroom = 0, 效果同于 skb_copy
 *        扩展后, offset 的数值会改变, 需要重新计算 (skb_reset_xxx_header)
 *        扩展后, 引用计数是1
 */
struct sk_buff *skb_copy_expand(const struct sk_buff *skb, int newheadroom, int newtailroom, int gfp_mask);

/**
 * @brief 按需扩展空间, 如果空间够, 那么不需要重新申请内存
 *        扩展后, offset 的数值会改变, 需要重新计算 (skb_reset_xxx_header)
 * @warning 该函数不能用于 shared 的 skb (引用数大于1的skb)
 *          可以用 skb = skb_share_check(skb); 新创建
 *          或者用 skb_shared 检查
 */
int pskb_expand_head(struct sk_buff *skb, int nhead, int ntail, gfp_t gfp_mask);

/**
 * @brief 深复制 skb 包括 sk_buff 和 data 但共享 skb_shared_info
 * @note  即通过 skb_shared_info 维护的 nonlinear skb 结构将会维持
 */
struct sk_buff *pskb_copy(struct sk_buff *skb, gfp_t gfp_mask);

/**
 * @brief 深复制 skb 包括 sk_buff 和 data 以及 skb_shared_info
 * @note  把 nonlinear skb 合并成为一个
 */
struct sk_buff *skb_copy(const struct sk_buff *skb, gfp_t gfp_mask);
```

# 4. 引用方法 skb_get skb_unref skb_shared skb_shared_check
```c++
/* Makes another reference to a socket buffer and returns a pointer to the buffer. */
static inline struct sk_buff *skb_get(struct sk_buff *skb) { // 引用数加一
	refcount_inc(&skb->users);
	return skb;
}

/* decrement the reference count and return true if we can free the skb */
static inline bool skb_unref(struct sk_buff *skb) { // 释放引用后; 计数为0时需要手动释放
	if (unlikely(!skb))
		return false;
	if (likely(refcount_read(&skb->users) == 1))
		smp_rmb();
	else if (likely(!refcount_dec_and_test(&skb->users)))
		return false;

	return true;
}

/* Drop a ref to the buffer and free it if the usage count has hit zero
 * Functions identically to kfree_skb */
void consume_skb(struct sk_buff *skb) { // 释放引用后; 计数为0时自动释放
	if (!skb_unref(skb))
		return;
	trace_consume_skb(skb);
	__kfree_skb(skb);
}

/* Returns true if more than one person has a reference to this buffer. */
static inline int skb_shared(const struct sk_buff *skb) {
	return refcount_read(&skb->users) != 1;
}

/*  If the buffer is shared, then the buffer is cloned and the old copy
 *	drops a reference. A new clone with a single reference is returned.
 *	If the buffer is not shared the original buffer is returned. */
static inline struct sk_buff *skb_share_check(struct sk_buff *skb, gfp_t pri) {
	might_sleep_if(gfpflags_allow_blocking(pri));
	/* "浅复制" 之前已经 "被引用" 的 skb;
	 * 并且消耗一次 之前的 skb */
	if (skb_shared(skb)) {
		struct sk_buff *nskb = skb_clone(skb, pri); // 浅复制

		if (likely(nskb))
			consume_skb(skb); // 原始 skb, 减少一次引用
		else
			kfree_skb(skb);   // 没内存了, 申请失败, 销毁原始 skb
		skb = nskb;
	}
	return skb;
}

static inline struct sk_buff *skb_unshare(struct sk_buff *skb, gfp_t pri) {
	might_sleep_if(gfpflags_allow_blocking(pri));
	/* "深复制" 之前已经 "浅复制" 的 skb;
	 * 并且消耗一次 之前的 skb */
	if (skb_cloned(skb)) {
		struct sk_buff *nskb = skb_copy(skb, pri);

		/* Free our shared copy */
		if (likely(nskb))
			consume_skb(skb);
		else
			kfree_skb(skb);
		skb = nskb;
	}
	return skb;
}

int usage(struct sk_buff *skb) {
	sk_buff *mskb;
	mskb = skb_get(skb); /* 1. 只要跑进来, 避免被中断/调度走, 引用一次 */
	if (task_need_skb_only(mskb)) {
		/* 2. 经过一定业务的处理, 发现需要修改 skb 的非 data 部分,
		 *    但 data 不需要改动, 所以保留一下
		 *    skb_share_check 后面的代码不应该依赖 skb, 因为是不可靠的 */
		mskb = skb_share_check(mskb);
		// ... task
		if (task_need_skb_data(mskb)) {
			/* 2. 经过一定业务的处理, 发现需要用到 skb 的 data 部分 */
			mskb = skb_unshare(mskb, GFP_ATOMIC);
			// ... task
		} else {
			/* 其他不需要 data 部分的业务 */
		}
		consume_skb(skb); // 对于上层 skb 已经不可靠了; 必须这里消耗一次
		return ACCEPT; /* 指示上层, 该函数已经处理了 skb, skb 指向不再可靠 */
	}

	skb_unref(skb);
	return UNUSED; /* 指示上层, skb 还是可靠的 */
}
```

# 5. 偏移方法 (实现 剥掉/封装 协议层)
```c++
// This function extends the used data area of the buffer.
skb_put(sbk, len)  { skb->tail += len; skb->len  += len; }

// This function extends the used data area of the buffer at the buffer start.
skb_push(skb, len) { skb->data -= len; skb->len  += len; }  /* 封装协议层 */
// This function removes data from the start of a buffer.
skb_pull(skb, len) { skb->len -= len; skb->data += len; }   /* 剥掉协议层 */

// Increase the headroom of an empty &sk_buff by reducing the tail room.
skb_reserve(skb, len) { skb->data += len; skb->tail += len; }
skb_tailroom_reserve(skb, len) { }
```

# 6. skb 读写
```c++
// skb 中拿数据
skb_copy_from_linear_data(const struct sk_buff *skb, void *to, const unsigned int len) {
	memcpy(to, skb->data, len);
}

// 拷数据到 skb
skb_copy_to_linear_data(struct sk_buff *skb, const void *from, const unsigned int len) {
	memcpy(skb->data, from, len);
}
```

# 8. skb_orphan
```c++
/**
 *	skb_orphan - orphan a buffer
 *	@skb: buffer to orphan
 *
 *	If a buffer currently has an owner then we call the owner's
 *	destructor function and make the @skb unowned. The buffer continues
 *	to exist but is no longer charged to its former owner.
 */
static inline void skb_orphan(struct sk_buff *skb)
{
	if (skb->destructor) {
		skb->destructor(skb);
		skb->destructor = NULL;
		skb->sk		= NULL;
	} else {
		BUG_ON(skb->sk);
	}
}
```