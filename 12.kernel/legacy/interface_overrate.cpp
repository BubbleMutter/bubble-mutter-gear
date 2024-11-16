// 网卡驱动层 限流实现

// 令牌桶算法 -------------------------------------------------------------------
struct dummy_rx_rate_unit_t {
	unsigned int CIR; /* incre rate */
	unsigned int CBS; /* burst rate */
	unsigned int CBC;  /* current size */
	unsigned int last_time;
};
struct dummy_rx_rate_ctx_t {
	struct dummy_rx_rate_unit_t stream;
	struct dummy_rx_rate_unit_t packet;
};

static DEFINE_PER_CPU(struct dummy_rx_rate_ctx_t, dummy_rx_rate_ctx);

/**
 * @return true means overrate
 */
static int __dummy_rx_overrate(struct dummy_rx_rate_unit_t *ctx, int unit) {
	bool overrate = false;

	if (unit > ctx->CBC) {
		unsigned int curr_time = jiffies_to_msecs(jiffies);
		ctx->CBC += ctx->CIR * (curr_time - ctx->last_time);
		if(unlikely(ctx->CBC > ctx->CBS)) {
			ctx->CBC = ctx->CBS;
		}
		ctx->last_time = curr_time;
	}

    if(unit <= ctx->CBC) {
        ctx->CBC -= unit;
		overrate = false;
    } else {
		overrate = true;
    }
	return overrate;
}

/**
 * @return true means overrate
 * @param bytes size of a received single packet
 */
static int dummy_rx_overrate(int bytes) {
	int retval;
	struct dummy_rx_rate_ctx_t *ctx = &get_cpu_var(dummy_rx_rate_ctx);
	retval = __dummy_rx_overrate(&ctx->stream, bytes) || 
			 __dummy_rx_overrate(&ctx->packet, 1);
	put_cpu_var(dummy_rx_rate_ctx);
	return retval;
}

static void dummy_rx_rate_init(void) {
	int cpu;
	for_each_possible_cpu(cpu) {
		struct dummy_rx_rate_ctx_t *ctx = &per_cpu(dummy_rx_rate_ctx, cpu);
		ctx->stream.CIR = 10*1024; /* incre rate 20 KB/ms */
		ctx->stream.CBS = 20*1024; /* burst rate 40 KB/ms */
		ctx->stream.CBC = 0; /* current size */
		ctx->stream.last_time = 0;

		ctx->packet.CIR = 10; /* incre rate 10 p/ms */
		ctx->packet.CBS = 50; /* burst rate 50 p/ms */
		ctx->packet.CBC = 0; /* current size */
		ctx->packet.last_time = 0;
	}
}
//////////////////////////////////////////////////////////////////////////////////