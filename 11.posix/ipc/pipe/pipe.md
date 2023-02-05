[TOC]
# pipe
# 接口
```c++
int pipe(int pipefd[2]);
```
1. `pipefd[0]` readable, input
2. `pipefd[1]` writable, output

# 内核实现 create_pipe_files
```c++
int create_pipe_files(struct file **res, int flags) {
    struct inode *inode = get_pipe_inode();
	struct file *f;
    // ...
    f = alloc_file(&path, FMODE_WRITE, &pipefifo_fops);
    // ...
	f->private_data = inode->i_pipe;
}
```
1. 内核实现主要入口函数是 create_pipe_files
    1. `get_pipe_inode()` 实现 pipe 的私有数据结构
    2. `pipefifo_fops` 实现 pipe 的回调方法

## 内核实现 get_pipe_inode
```c++
static struct inode * get_pipe_inode(void) {
	struct pipe_inode_info *pipe;
    // ...
	pipe = alloc_pipe_info();
    // ...
	inode->i_pipe = pipe;
}
/**
 *	struct pipe_inode_info - a linux kernel pipe
 *	@mutex: mutex protecting the whole thing
 *	@wait: reader/writer wait point in case of empty/full pipe
 *	@nrbufs: the number of non-empty pipe buffers in this pipe
 *	@buffers: total number of buffers (should be a power of 2)
 *	@curbuf: the current pipe buffer entry
 *	@bufs: the circular array of pipe buffers
 **/
struct pipe_inode_info {
	struct mutex mutex;
	wait_queue_head_t wait;
	unsigned int nrbufs, curbuf, buffers;
    // ...
	struct pipe_buffer *bufs;
};

/**
 *	struct pipe_buffer - a linux kernel pipe buffer
 *	@page: the page containing the data for the pipe buffer
 *	@offset: offset of data inside the @page
 *	@len: length of data inside the @page
 **/
struct pipe_buffer {
	struct page *page;
	unsigned int offset, len;
    // ...
};
```
1. `get_pipe_inode()` 返回 `struct pipe_inode_info`
    1. 成员变量 `struct pipe_buffer *bufs` 作为环形数组存储
2. `struct pipe_buffer` 的实际存储成员是 `struct page`

## 内核实现 pipefifo_fops
```c++
const struct file_operations pipefifo_fops = {
	.open		= fifo_open,
	.llseek		= no_llseek,
	.read		= new_sync_read,
	.read_iter	= pipe_read,
	.write		= new_sync_write,
	.write_iter	= pipe_write,
	.poll		= pipe_poll,
	.unlocked_ioctl	= pipe_ioctl,
	.release	= pipe_release,
	.fasync		= pipe_fasync,
};
```

# 示例1 (单写多读) (apue v3 15.5 example)
1. 实现如下通讯模型
```txt
                         /----> program3
infile --> program1 --> {
                         \----> program2
```

2. 测试命令
```sh
# 先创建 fifo
mkfifo fifo1
# fifo 的输出作为 program3 的输入
program3 < fifo1
# infile 作为 program1 的输入
# program1 的输出通过 tee 双向分流到 fifo1 和 program2
program1 < infile | tee fifo1 | program2
```

# 示例2 (多写单读) (apue v3 15.5 example)
1. 实现如下通讯模型, 多生产者单消费者
```txt
productor1 --\
productor2 ---} --> fifo --> consumer
productor3 --/
```
1. 这个示例依赖 fifo 的写操作是原子性, 否则多写并发需要额外加锁
2. 实际上 pipefifo_fops 的 pipe_write 实现中加锁了
    1. 写操作前后, 加锁 `pipe_inode_info->mutex`
3. 因此用户态多写并发无需额外加锁处理

# 示例3 shell 管道
1. shell 进程先创建管道
2. shell 进程fork() 两个进程
3. 前置进程关闭"读描述符", 重定向 stdout 到 "写描述符"
    `close(pipefd[0]); dup2(pipefd[1], STDOUT_FILENO);`
4. 后置进程关闭"写描述符", 重定向 stdin  到 "读描述符"
    `close(pipefd[1]); dup2(pipefd[0], STDIN_FILENO);`
