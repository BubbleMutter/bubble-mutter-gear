/**
 * @file 把 log 打印到 /proc
 *       在初始化时指定 "总行数" 和 "行大小"
 *       需要较大的内存 2*总行数*行大小
 *       日志有两个buff, 一个满了就会使用另一个
 * @note 不支持重置 "总行数" 和 "行大小"
 *       日志函数没有加锁, 多cpu场景下会出问题
 */
#include <stdarg.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/seq_file.h>

typedef struct xyz_plog_t {
    const uint32_t line; // 行长度
    const uint32_t size; // 总行数
    char *const buff[2]; // 两个 buff
    uint32_t curr; // 当前行数
    uint32_t bufp; // 当前 buff 索引
    uint32_t swap; // 标记是否 swap
} xyz_plog_t;

#define XYZ_PLOG_DEFAULT_LINE 128
#define XYZ_PLOG_DEFAULT_SIZE 1024

#define xyz_plog_declare(name, line, size, bufu, bufv) \
    static unsigned char _name##bufu[_line*_size]; \
    static unsigned char _name##bufv[_line*_size]; \
    static xyz_plog_t name = { \
        .line = line, \
        .size = size,\
        .buff = { bufu, bufv },\
    }

#define xyz_plog_next(plog) ((!plog->bufp) & 0x1)

static void xyz_plog_swap(xyz_plog_t *plog) {
    if (plog->curr < plog->size)
        return;
    plog->swap = 1;
    plog->bufp = xyz_plog_next(plog);
    memset(plog->buff[plog->bufp], 0, plog->line*plog->size);
    plog->curr = 0;
}

__attribute__((format(printf, 2, 3))) int xyz_plog_printf(xyz_plog_t *plog, const char *format, ...) {
    va_list ap;
    xyz_plog_swap(plog);

    va_start(ap, format);
    vsnprintf(plog->buff[plog->bufp] + plog->line * plog->curr, 
              plog->line, format, ap); 
	va_end(ap);

    ++plog->curr;
    return 0;
}

int xyz_plog_seqshow(xyz_plog_t *plog, struct seq_file *m, void *v) {
    int i;
    if (plog->swap) {
        int next = xyz_plog_next(plog);
        for (i = 0; i < plog->size; ++i)
            seq_printf(m, "%s\n", plog->buff[next] + plog->line*i);
    }
    for (i = 0; i < plog->curr; ++i)
        seq_printf(m, "%s\n", plog->buff[plog->bufp] + plog->line*i);
    return 0;
}


// usage demo
xyz_plog_declare(mdrv_plog, XYZ_PLOG_DEFAULT_LINE, XYZ_PLOG_DEFAULT_SIZE);

static int xyz_proc_plog_show(struct seq_file *m, void *v) {
    // other message print
    xyz_plog_seqshow(mdrv_plog, m, v);
    return 0;
}

static int mdrv_proc_open(struct inode * inode, struct file * file) {
    return single_open(file, xyz_proc_plog_show, NULL);
}

static struct file_operations mdrv_proc_file_ops = {
    owner:      THIS_MODULE,
    open:       mdrv_proc_open,
    read:       seq_read,
    llseek:     seq_lseek,
    release:    single_release,
};

 __attribute__((format(printf, 1, 2))) int xyz_plog_printf(const char *format, ...) {
    char *line;
    va_list ap;

    if (!plog.init) return -1;

    xyz_plog_swap();
    line = plog.buff[plog.bufp] + plog.line * plog.curr;

    va_start(ap, format);
    vsnprintf(line, plog.line, format, ap); 
	va_end(ap);

    ++plog.curr;
    return 0;
}

int xyz_plog_remove() {
    remove_proc_entry("plog", xyz_proc_root);
    plog.init = 0;
    return 0;
}