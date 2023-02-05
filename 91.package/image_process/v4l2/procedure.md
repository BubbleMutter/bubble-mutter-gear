// [1] open

// [2] query capability
struct v4l2_capability cap;
ioctl() VIDIOC_QUERYCAP

// [3] set format
struct v4l2_format fmt;
ioctl() VIDIOC_S_FMT

// [4] request the mem-buffer-space from the driver
struct  v4l2_requestbuffers req;
ioctl() VIDIOC_REQBUFS


// [5] allocate buffer-space and map to userspace, DMA
struct  v4l2_buffer buf;
ioctl() VIDIOC_QUERYBUF
mmap()

// [6] push the buffer-space to the stream-input-queue
struct  v4l2_buffer buf;
ioctl() VIDIOC_QBUF

// [7] open the stream and start capture
ioctl() VIDIOC_STREAMON

// [8] loop to capture
struct v4l2_buffer buf;
ioctl() VIDIOC_DQBUF
// get the buffer from the [5]-userspace-mem
ioctl() VIDIOC_QBUF

// [9] stop capture
ioctl() VIDIOC_STREAMOFF

// [10] close
