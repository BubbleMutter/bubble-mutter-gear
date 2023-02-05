#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/videodev2.h>
#include<strings.h>
#include<sys/mman.h>
#include<unistd.h>

#include<cstdio>
#include<cstdlib>
#include<cerrno>

#include<opencv2/core.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
using namespace cv;
using namespace std;

static void yuyv2bgr(unsigned char **bgr,
        const unsigned char *src,
        const int height,
        const int width) {

    /* yuyv, 16, yuv = 4:2:2,
     * src :        Y0 U0, Y1 V1;       Y2 U2,  Y3 V3;
     * dst : [Y0 U0 V1], [Y1 U0 V1]; [Y2 U2 V3], [Y3 U2 V3]; */
    int nPixels = width * height;
    int i, /* row counter */
        j; /* column counter */
    unsigned char *dst = (unsigned char *)malloc(3 * nPixels);
    bzero(dst, 3 * nPixels);
    for(i = 0; i < height; i++) {
        int lineStart = i * width * 2;
        int uStart = lineStart + 1;
        int vStart = lineStart + 3;

        for(j = 0; j < width; j++) {
            int posPixel = i * width + j;
            char y = src[posPixel * 2];
            char u = src[uStart + (j/2)*4];
            char v = src[vStart + (j/2)*4];
#if 0
            /* R */ dst[posPixel*3 + 2] = y + 1.140*v;
            /* G */ dst[posPixel*3 + 1] = y - 0.395*u - 0.581*v;
            /* B */ dst[posPixel*3 + 0] = y + 2.032*u;
#endif
#if 1
            char val;
            val = 1.164*(y-16) + 1.159*(v-128);
            /* R */ dst[posPixel*3 + 2] = (val < 0) ? 0 : (val > 255 ? 255 : val);
            val = 1.164*(y-16) - 0.394 * (u - 128) - 0.813 * (v -128);
            /* G */ dst[posPixel*3 + 1] = (val < 0) ? 0 : (val > 255 ? 255 : val);
            val = 1.164*(y-16) + 2.018*(u-128);
            /* B */ dst[posPixel*3 + 0] = (val < 0) ? 0 : (val > 255 ? 255 : val);
#endif

        }
    }
    *bgr = dst;
}

/* wrapper of ioctl() handle the EINTR situation */
static int Ioctl(int fh, int request, void *arg) {
    int r;
    do {
        r = ioctl(fh, request, arg);
    } while (-1 == r && EINTR == errno);
    return r;
}

static void v4l2_fmt_parser(const int format) {
    char fmt_str[5];
    int i;
    for(i = 0; i < 4; i++) {
        //fmt_str[i] = ((0xff << i*8) & format) >> i*8;
        fmt_str[i] = (format >> i*8) & 0xff;
    }
    fmt_str[i] = '\0';
    printf("v4l2 capture format is set to %s.\n", fmt_str);
}

#define dev_name ("/dev/video0")
#define file_name ("./img.raw")

struct buffer {
    void  *start;
    size_t length;
} g_buffer;

int main(int argc, char **argv) {
    int ret;
    int type;
    // [1] open device node, file
    int fdVideo = open(dev_name, O_RDWR /* | O_NONBLOCK*/ , 0);
    int fdOutFile = open(file_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    // [2] query the capability
    struct v4l2_capability cap;
    if(Ioctl(fdVideo, VIDIOC_QUERYCAP, &cap) == -1)
        perror("ioctl() VIDIOC_QUERYCAP");

    if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
        printf("It's a capture device\n");

    // [3] set the video format
    struct v4l2_format fmt;
    bzero(&fmt, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.height = 480;
    fmt.fmt.pix.width = 640;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; //V4L2_PIX_FMT_BGR24, V4L2_PIX_FMT_RGB24
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED; // buffer-mechanism ??

    if(Ioctl(fdVideo, VIDIOC_S_FMT, &fmt) == -1)
        perror("ioctl() VIDIOC_S_FMT");
    /* check the setting */
    printf("width:%d, height:%d.\n",
            fmt.fmt.pix.width, fmt.fmt.pix.height);
    v4l2_fmt_parser(fmt.fmt.pix.pixelformat);

    // [4] request the mem-buffer-space from the driver
    struct  v4l2_requestbuffers req;
    req.count = 1; // buffer count, can be set to `1'?
    req.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory= V4L2_MEMORY_MMAP;
    if(-1 == Ioctl(fdVideo, VIDIOC_REQBUFS, &req))
        perror("ioctl VIDIOC_REQBUFS");
    printf("%d memory space has been allocated.\n", req.count);

    // [5] allocate buffer-space and map to userspace, DMA
    struct  v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if (Ioctl(fdVideo, VIDIOC_QUERYBUF, &buf) == -1)
        perror("ioctl() VIDIOC_QUERYBUF");
    g_buffer.length = buf.length;
    g_buffer.start = mmap(NULL, buf.length,
            PROT_READ | PROT_WRITE /* required */,
            MAP_SHARED /* recommended */,
            fdVideo, buf.m.offset);
    if (MAP_FAILED == g_buffer.start)
        perror("mmap() failed");
    printf("each buffer-space is %d bytes.\n", (int)g_buffer.length);

    // [6] push the buffer-space to the stream-input-queue
    // open the stream and start capture
    int i;
    bzero(&buf, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if (Ioctl(fdVideo, VIDIOC_QBUF, &buf) == -1)
        perror("ioctl() VIDIOC_QBUF");
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(Ioctl(fdVideo, VIDIOC_STREAMON, &type) == -1)
        perror("ioctl() VIDIOC_STREAMON");

    // [7]
    for(i = 0; i < 3; i++) {
        struct v4l2_buffer buf;
        bzero(&buf, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if(Ioctl(fdVideo, VIDIOC_DQBUF, &buf) == -1)
            perror("ioctl() VIDIOC_DQBUF");

        Mat srcYUYV(480, 640, CV_8UC2);
        memcpy(srcYUYV.data, (unsigned char *)g_buffer.start, srcYUYV.rows * srcYUYV.cols * srcYUYV.channels());
        Mat img(480, 640, CV_8UC3);
        cvtColor(srcYUYV, img, COLOR_YUV2BGR_YUYV);
        imshow("img, decode from opencv", img);
        waitKey();

#if 1 /* my decode yuyv algorithm, with bugs */
        unsigned char *bgr;
        yuyv2bgr(&bgr, (unsigned char *)g_buffer.start, 480, 640);
        memcpy(img.data, bgr, img.rows * img.cols * img.channels());
        imshow("img, decode from myself", img);
        waitKey();
#endif
        write(fdOutFile, g_buffer.start, g_buffer.length);

        if(Ioctl(fdVideo, VIDIOC_QBUF, &buf) == -1)
            perror("ioctl() VIDIOC_QBUF");
    }

    // [9] stop capture
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    Ioctl(fdVideo, VIDIOC_STREAMOFF, & type);

    // [10] close device
    close(fdVideo);
    return 0;
}
#if 0
void() {
    unsigned char* y0 = yuv + 0;
    unsigned char* u0 = yuv + 1;
    unsigned char* y1 = yuv + 2;
    unsigned char* v0 = yuv + 3;

    unsigned  char* r0 = rgb + 0;
    unsigned  char* g0 = rgb + 1;
    unsigned  char* b0 = rgb + 2;
    unsigned  char* r1 = rgb + 3;
    unsigned  char* g1 = rgb + 4;
    unsigned  char* b1 = rgb + 5;

    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;

    for(i = 0; i <= (WIDTH * HEIGHT) / 2 ;i++)

        bt0 = 1.164 * (*y0 - 16) + 2.018 * (*u0 - 128);
    gt0 = 1.164 * (*y0 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);
    rt0 = 1.164 * (*y0 - 16) + 1.596 * (*v0 - 128);

    bt1 = 1.164 * (*y1 - 16) + 2.018 * (*u0 - 128);
    gt1 = 1.164 * (*y1 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);
    rt1 = 1.164 * (*y1 - 16) + 1.596 * (*v0 - 128);


    if(rt0 > 250)    rt0 = 255;
    if(rt0< 0)       rt0 = 0;

    if(gt0 > 250)    gt0 = 255;
    if(gt0 < 0)  gt0 = 0;

    if(bt0 > 250)    bt0 = 255;
    if(bt0 < 0)  bt0 = 0;

    if(rt1 > 250)    rt1 = 255;
    if(rt1 < 0)  rt1 = 0;

    if(gt1 > 250)    gt1 = 255;
    if(gt1 < 0)  gt1 = 0;

    if(bt1 > 250)    bt1 = 255;
    if(bt1 < 0)  bt1 = 0;

    *r0 = (unsigned char)rt0;
    *g0 = (unsigned char)gt0;
    *b0 = (unsigned char)bt0;

    *r1 = (unsigned char)rt1;
    *g1 = (unsigned char)gt1;
    *b1 = (unsigned char)bt1;

    yuv = yuv + 4;
    rgb = rgb + 6;
    if(yuv == NULL)
        break;

    y0 = yuv;
    u0 = yuv + 1;
    y1 = yuv + 2;
    v0 = yuv + 3;

    r0 = rgb + 0;
    g0 = rgb + 1;
    b0 = rgb + 2;
    r1 = rgb + 3;
    g1 = rgb + 4;
    b1 = rgb + 5;
}
#endif
