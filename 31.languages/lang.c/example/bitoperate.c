#define param1_mask 0x1
#define param2_mask 0x2
#define param3_mask 0x3
#define param4_mask 0x4
void set_bit_with_flag_mask(int *bit, int value, int mask) {
    *bit = (*bit & ~mask) | (value & mask);
    /* a) 保留其他的 bit; 且把感兴趣的 bit 置空
     * b) 取得 value 中感兴趣的 bit
     * c) 取 或操作
     */
}

void usage() {
    void *ctx;
    // 设置为 true
    set_bit_with_flag_mask(ctx->flag, param1_mask, param1_mask);
    // 设置为 false
    set_bit_with_flag_mask(ctx->flag, 0, param1_mask);
}