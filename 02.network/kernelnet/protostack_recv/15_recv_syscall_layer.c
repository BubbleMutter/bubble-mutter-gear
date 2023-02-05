SYSCALL_DEFINE3(recvmsg, int, fd, struct msghdr __user *, msg,
		unsigned int, flags)
{
	if (flags & MSG_CMSG_COMPAT)
		return -EINVAL;
	return __sys_recvmsg(fd, msg, flags);
}

long __sys_recvmsg(int fd, struct msghdr __user *msg, unsigned flags)
{
    // ...
    // 通过fd 获取 socket
    // 1. int fd 获取 struct fd
    // 2. fd->file->private_data, sock_map_fd()函数中设置private_data为socket
	sock = sockfd_lookup_light(fd, &err, &fput_needed);
	if (!sock)
		goto out;

	err = ___sys_recvmsg(sock, msg, &msg_sys, flags, 0);

    // if sockfd_lookup_light 找不到 socket
    // 则 fput_light 销毁 file
	fput_light(sock->file, fput_needed);
out:
	return err;
}

// 该函数 本质把 msghdr 从用户态, 切到内核态
// msghdr 中的成员有 sockaddr_storage 和 iovec[]
// 该函数 把msghdr的成员 iovec[] 和 sockaddr_storage 切到内核态
// iovec->iov_base才是物理内存意义上的 userspace,
// 后续收包逻辑, 才把sk_buff数据 复制到 iovec->iov_base中
static int ___sys_recvmsg(struct socket *sock, struct msghdr __user *msg,
			 struct msghdr *msg_sys, unsigned int flags, int nosec)
{
    // msg_sys kernelspace, 用于临时存储userspace的msg
	struct compat_msghdr __user *msg_compat =
	    (struct compat_msghdr __user *)msg;
	struct iovec iovstack[UIO_FASTIOV];
	struct iovec *iov = iovstack;
	unsigned long cmsg_ptr;
	int err, total_len, len;

	/* kernel mode address */
	struct sockaddr_storage addr;

	/* user mode address pointers */
	struct sockaddr __user *uaddr;
	int __user *uaddr_len;

    // userspace 的 msg 赋值到 msg_sys
	if (MSG_CMSG_COMPAT & flags)
		err = get_compat_msghdr(msg_sys, msg_compat);
	else
		err = copy_msghdr_from_user(msg_sys, msg);
	if (err)
		return err;

    // 分配足够多内核态 iovec
    // iovstack[UIO_FASTIOV] 默认放在栈上的 若干个
	if (msg_sys->msg_iovlen > UIO_FASTIOV) {
		err = -EMSGSIZE;
		if (msg_sys->msg_iovlen > UIO_MAXIOV)
			goto out;
		err = -ENOMEM;
		iov = kmalloc(msg_sys->msg_iovlen * sizeof(struct iovec),
			      GFP_KERNEL);
		if (!iov)
			goto out;
	}

	/* Save the user-mode address (verify_iovec will change the
	 * kernel msghdr to use the kernel address space)
	 */
	uaddr = (__force void __user *)msg_sys->msg_name;
	uaddr_len = COMPAT_NAMELEN(msg);
    // verify_iovec 本质; userspace 切换到 kernelspace
    // 1. msg_sys->msg_name = addr; 指向kernelspace       => move_addr_to_user
    // 2. msg_sys->msg_iov指向的数据 赋值到 iov指向的数据 => copy_from_user, msg_sys->msg_iov之前是指向userspace的
    // 3. msg_sys->msg_iov = iov
    // Note: msg_iov->iov_base 才是指向物理意义上的用户空间, 用于存储buffer
	if (MSG_CMSG_COMPAT & flags)
		err = verify_compat_iovec(msg_sys, iov, &addr, VERIFY_WRITE);
	else
		err = verify_iovec(msg_sys, iov, &addr, VERIFY_WRITE);
	if (err < 0)
		goto out_freeiov;

    // ...
    // 收包
	err = (nosec ? sock_recvmsg_nosec : sock_recvmsg)(sock, msg_sys,
							  total_len, flags);
    // ...
    // 切回用户态
	if (uaddr != NULL) {
		err = move_addr_to_user(&addr,
					msg_sys->msg_namelen, uaddr,
					uaddr_len);
		if (err < 0)
			goto out_freeiov;
	}
    // ...
out:
	return err;
}