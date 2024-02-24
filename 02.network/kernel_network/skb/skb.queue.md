[TOC]
# skb_queue
```c++
void skb_queue_head_init(struct sk_buff_head *list);             // 初始化

struct sk_buff *skb_peek(const struct sk_buff_head *list_);      // 队列头 读取 (无锁操作)
struct sk_buff *skb_peek_tail(const struct sk_buff_head *list_); // 队列尾 读取 (无锁操作)
void skb_unlink(struct sk_buff *skb, struct sk_buff_head *list); // 出列后手动释放

struct sk_buff *skb_dequeue(struct sk_buff_head *list);        // 队列头 出列 (带锁版本)
struct sk_buff * __skb_dequeue (struct sk_buff_head *list);    // 队列头 出列 (无锁版本)
struct sk_buff *skb_dequeue_tail(struct sk_buff_head *list);   // 队列尾 出列 (带锁版本)
struct sk_buff *__skb_dequeue_tail(struct sk_buff_head *list); // 队列尾 出列 (无锁版本)

void skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk);     // 队列头 入列 (加锁版本)
void __skb_queue_head (struct sk_buff_head *list, struct sk_buff * newsk); // 队列头 入列 (无锁版本)
void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk);     // 队列尾 入列 (加锁版本)
void __skb_queue_tail (struct sk_buff_head *list, struct sk_buff * newsk); // 队列尾 出列 (无锁版本)

void skb_queue_purge(struct sk_buff_head *list);    // 清空队列, 带锁版本
void __skb_queue_purge(struct sk_buff_head *list);  // 清空队列, 无锁版本

int skb_queue_empty(struct sk_buff_head *list);     // 检查队列是否为空
```