#define TS_FMTMAC_X "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX"
#define TS_MAC_UINT64_T(mac)            (uint8_t)(mac&0xff),\
                                                                                (uint8_t)((mac>>8) & 0xff),\
                                                                                (uint8_t)((mac>>16) & 0xff),\
                                                                                (uint8_t)((mac>>24) & 0xff),\
                                                                                (uint8_t)((mac>>32) & 0xff),\
                                                                                (uint8_t)((mac>>40) & 0xff)

#define lprintf(fmt, ...) \
    do { \
            FILE *_fp = fopen("/dev/ttydbg", "w"); \
            if(_fp) { \
                fprintf(_fp, "[%s:%d]"fmt"\n", __func__, __LINE__, ##__VA_ARGS__); \
                fclose(_fp); \
            } \
    } while(0);

#define ARRAY_NELEM(a) (sizeof(a)/sizeof(*a))
