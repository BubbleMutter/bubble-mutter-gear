[TOC]
# tuntap
+ https://docs.kernel.org/networking/tuntap.html
    + transmit packets crossing kernelspace and userspace
    + if_tun.h
    + if_tap.h
+ tap provides l2 packet transmit only
    `ip tuntap add mode tun multi_queue name tap0`
+ tun provides l3 packet transmit
    `ip tuntap add mode tun multi_queue name tun0`
+ [userspace example](https://docs.kernel.org/networking/tuntap.html#multiqueue-tuntap-interface)
+ ker
+ via tcpdump to pretty print packets:
    + process-x: userspace send to kernel
    + process-y: tcpdump -lnevvi tun0

# driver/net/tun.c
```c++
// userspace operations
static const struct file_operations tun_fops = {
	.open	= tun_chr_open,
};
static int tun_chr_open(struct inode *inode, struct file * file) {
	tfile->socket.ops = &tun_socket_ops;
}
static const struct proto_ops tun_socket_ops = {
	.sendmsg = tun_sendmsg,
	.recvmsg = tun_recvmsg,
};
static int tun_sendmsg(struct socket *sock, struct msghdr *m, size_t total_len) {
	tun_get_user(...) {
        copy_from_user(...)
        // ...
        netif_rx(...)
    }
}
static int tun_recvmsg(struct socket *sock, struct msghdr *m, size_t total_len, int flags) {
    tun_do_read(...) {
        tun_put_user(...) {
            copy_to_user(...)
        }
    }
}
```