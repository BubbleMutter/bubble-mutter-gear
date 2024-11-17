# msgctl (command line tool)

# msg (xsi)
## 接口
```c++
#include <sys/msg.h>
int msgget(key_t key, int msgflg);
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```

## 实现
```c++
SYSCALL_DEFINE2(msgget,
SYSCALL_DEFINE4(msgsnd,
SYSCALL_DEFINE5(msgrcv,
SYSCALL_DEFINE3(msgctl,
```

### msgget
```c++
SYSCALL_DEFINE2(msgget, key_t, key, int, msgflg) {
    struct ipc_namespace *ns;
    static const struct ipc_ops msg_ops = {
        .getnew = newque,          // 创建新队列的方法; 由 ipcget_* 方法调用
        .associate = msg_security,
    };
    ns = current->nsproxy->ipc_ns; // 进程故有的 namespace
    // ...
    return ipcget(ns, &msg_ids(ns), &msg_ops, &msg_params);
}

static int newque(struct ipc_namespace *ns, struct ipc_params *params) {
    struct msg_queue *msq;
    // ...
    msq = ipc_rcu_alloc(sizeof(*msq)); // 申请内存
    // ...
    msq->q_perm.key = key;             // 传递 uuid
    INIT_LIST_HEAD(&msq->q_messages);  // 消息体队列; 双向链表
    INIT_LIST_HEAD(&msq->q_receivers); // 订阅者队列; 双向链表
    INIT_LIST_HEAD(&msq->q_senders);   // 发送者队列; 双向链表
    // ...
    // 把 uuid 加入到 ids 的表中
    id = ipc_addid(&msg_ids(ns), &msq->q_perm /* 内有 uuid */, ns->msg_ctlmni);
    // ...
}
```
### msgsnd
```c++
SYSCALL_DEFINE4(msgsnd, int, msqid, struct msgbuf __user *, msgp, size_t, msgsz, int, msgflg) {
    long mtype;
    if (get_user(mtype, &msgp->mtype)) // 先获取 msgbuf->mtype
        return -EFAULT;
    return do_msgsnd(msqid, mtype, msgp->mtext, msgsz, msgflg);
}

long do_msgsnd(int msqid, long mtype, void __user *mtext, size_t msgsz, int msgflg) {
    struct msg_queue *msq;
    struct msg_msg *msg;
    struct ipc_namespace *ns;
    ns = current->nsproxy->ipc_ns;
    // ...
    msg = load_msg(mtext, msgsz); // 申请 msg_msg 内存然后 copy_from_user 拿到 mtext
    msg->m_type = mtype;
    msg->m_ts = msgsz;
    // ...
    /* 根据 msqid 取得 msg_queue 对象
     * 对应 newque() 中 ipc_addid 传入的对象 */
    msq = msq_obtain_object_check(ns, msqid);
    for (;;) {
        /* 检查 msq 的可访问性; 某些场景下需要 阻塞等待(schedule())
         * a) msq->q_perm 权限问题
         * b) ipc_valid_object(&msq->q_perm); 判断是否存在
         *    可能在该进程 调度走后; msq 被其他进程销毁了
         * c) 判断 msq->q_qbytes 总容量
         *        msq->q_cbytes 当前容量
         *        msq->q_qnum 当面消息数  是否允许发送
         * d) 拉起了 IPC_NOWAIT 标记; 将马上 return EAGAIN
         * e) 发送者 current 加入 msq->q_senders 等待队列
         *    TASK_INTERRUPTIBLE
         * f) schedule() 调度走
         * g) 发送者 current 离开 msq->q_senders 等待队列
         *    表示重新回到该进程; 重新走 for 循环
         */
    }
    /* 当各种条件满足; 即允许发送 */
    ipc_update_pid(&msq->q_lspid /* last-set-pid */, task_tgid(current));
    msq->q_stime = get_seconds(); /* last-set-time */
    if (!pipelined_send(msq, msg)) {
        /* no one is waiting for this message, enqueue it */
        list_add_tail(&msg->m_list, &msq->q_messages); // 入列 msq->q_messages
        msq->q_cbytes += msgsz; // 计数 消息体长度
        msq->q_qnum++;          // 计数 消息体个数
        atomic_add(msgsz, &ns->msg_bytes); // namespace 计数 历史总和
        atomic_inc(&ns->msg_hdrs);         // namespace 计数 历史总和
    }
    // ... 错误处理; 解锁等
}
```
