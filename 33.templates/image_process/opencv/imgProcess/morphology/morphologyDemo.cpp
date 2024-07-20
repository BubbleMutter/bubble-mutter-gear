#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace cv;
using namespace std;

Mat g_srcImage, g_dstImage;

const char *win_name[] = {
    "open/close operation",
    "erode/dilate",
    "top/black hat",
};

int g_nElementShape = MORPH_RECT; //

/* global vars for TrackBar output-params */
int g_nMaxIterationNum = 10;
int g_nOpenCloseNum = 0;    /* open/close operation */
int g_nErodeDilateNum = 0;  /* erode/dilate */
int g_nTopBlackHatNum = 0;  /* top/black-hat */

static void on_OpenClose(int slider, void *userdata);
static void on_ErodeDilate(int slider, void *userdata);
static void on_TopBlackHat(int slider, void *userdata);

int main(int argc, char **argv) {
    g_srcImage = imread("riki_bh.jpg");
    if(!g_srcImage.data)
        return -1;
    resize(g_srcImage, g_srcImage, Size(600, 400));
    namedWindow("srcImage");
    imshow("srcImage", g_srcImage);

    // 3 windows
    namedWindow(win_name[0], 1);
    namedWindow(win_name[1], 1);
    namedWindow(win_name[2], 1);

    // obtain parameter value
    g_nOpenCloseNum = 9;
    g_nErodeDilateNum = 9;
    g_nTopBlackHatNum = 2;

    // create trackbar for each window
    createTrackbar("iterator value : ", win_name[0],
            &g_nOpenCloseNum, g_nMaxIterationNum*2+1, on_OpenClose);
    createTrackbar("iterator value : ", win_name[1],
            &g_nErodeDilateNum, g_nMaxIterationNum*2+1, on_ErodeDilate);
    createTrackbar("iterator value : ", win_name[2],
            &g_nTopBlackHatNum, g_nMaxIterationNum*2+1, on_TopBlackHat);

    while(1) {
        int c;
#if 1
        on_OpenClose(g_nOpenCloseNum, 0);
        on_ErodeDilate(g_nErodeDilateNum, 0);
        on_TopBlackHat(g_nTopBlackHatNum, 0);
#endif

        c = waitKey();
        if(c == 'q' || c == 27)
            break;

        if(c == '1')
            g_nElementShape = MORPH_ELLIPSE;
        else if(c == '2')
            g_nElementShape = MORPH_RECT;
        else if(c == '3')
            g_nElementShape = MORPH_CROSS;
        else if(c == ' ') /* loop in the 3 shapes */
            g_nElementShape = (g_nElementShape + 1) % 3;
    }
    return 0;
}

static void on_OpenClose(int slider, void *userdata) {
    int offset = g_nOpenCloseNum - g_nMaxIterationNum;

    // absolute value of offset
    int abs_offset = offset > 0 ? offset : -offset;
    Mat element = getStructuringElement(g_nElementShape,
            Size(2*abs_offset+1, 2*abs_offset+1),
            Point(abs_offset, abs_offset));
    if(offset < 0) /* open operation */
        morphologyEx(g_srcImage, g_dstImage,
                MORPH_OPEN, element);
    else /* close operation */
        morphologyEx(g_srcImage, g_dstImage,
                MORPH_CLOSE, element);
    imshow(win_name[0], g_dstImage);
}

static void on_ErodeDilate(int slider, void *userdata) {
    int offset = g_nErodeDilateNum - g_nMaxIterationNum;

    // absolute value of offset
    int abs_offset = offset > 0 ? offset : -offset;
    Mat element = getStructuringElement(g_nElementShape,
            Size(2*abs_offset+1, 2*abs_offset+1),
            Point(abs_offset, abs_offset));
    if(offset < 0) /* erode */
        morphologyEx(g_srcImage, g_dstImage,
                MORPH_ERODE, element);
    else /* dilate */
        morphologyEx(g_srcImage, g_dstImage,
                MORPH_DILATE, element);
    imshow(win_name[1], g_dstImage);
}

static void on_TopBlackHat(int slider, void *userdata) {
    int offset = g_nTopBlackHatNum - g_nMaxIterationNum;

    // absolute value of offset
    int abs_offset = offset > 0 ? offset : -offset;
    Mat element = getStructuringElement(g_nElementShape,
            Size(2*abs_offset+1, 2*abs_offset+1),
            Point(abs_offset, abs_offset));
    if(offset < 0) /* top hat */
        morphologyEx(g_srcImage, g_dstImage,
                MORPH_TOPHAT, element);
    else /* black hat */
        morphologyEx(g_srcImage, g_dstImage,
                MORPH_BLACKHAT, element);
    imshow(win_name[2], g_dstImage);
}
