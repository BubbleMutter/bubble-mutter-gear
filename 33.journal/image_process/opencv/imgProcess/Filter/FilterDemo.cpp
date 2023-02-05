#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace cv;
using namespace std;
/* global variables */
const char *win_name[] = {
    "[1] box filter",
    "[2] mean blur",
    "[3] gaussian blur",
    "[4] median blur",
    "[5] bilateral filter",
};

Mat g_srcImage,
    g_dstImage_box,
    g_dstImage_mean,
    g_dstImage_gaussian,
    g_dstImage_median,
    g_dstImage_bilateral;

int g_nBoxFilterValue = 6, /* Box blur */
    g_nMeanBlurValue = 10, /* Mean Blur */
    g_nGaussianBlurValue = 6, /* Gaussian Blur */
    g_nMedianBlurValue = 10,
    g_nBilateralFilterValue = 10;

/* global function */
static void on_BoxFilter(int slider, void *userdata);
static void on_MeanBlur(int slider, void *userdata);
static void on_GaussianBlur(int slider, void *userdata);
static void on_MedianBlur(int slider, void *userdata);
static void on_BilateralFilter(int slider, void *userdata);

int main(int argc, char **argv) {
    g_srcImage = imread("juggernaut.jpg");

    if(!g_srcImage.data)
        return -1;
    g_dstImage_box = g_srcImage.clone();
    g_dstImage_mean = g_srcImage.clone();
    g_dstImage_gaussian = g_srcImage.clone();
    g_dstImage_median = g_srcImage.clone();
    g_dstImage_bilateral = g_srcImage.clone();

    namedWindow("src image", 1);
    imshow("src image", g_srcImage);

    /* box filter */
    namedWindow(win_name[0], 1);
    createTrackbar("core : ", win_name[0],
            &g_nBoxFilterValue,
            50, on_BoxFilter);
    on_BoxFilter(g_nBoxFilterValue, 0);
    /* ========== */

    /* mean blur */
    namedWindow(win_name[1], 1);
    createTrackbar("core : ", win_name[1],
            &g_nMeanBlurValue,
            50, on_MeanBlur);
    on_MeanBlur(g_nMeanBlurValue, 0);
    /* ========== */

    /* gaussian blur */
    namedWindow(win_name[2], 1);
    createTrackbar("core : ", win_name[2],
            &g_nGaussianBlurValue,
            50, on_GaussianBlur);
    /* ========== */

    /* median blur */
    namedWindow(win_name[3], 1);
    createTrackbar("core : ", win_name[3],
            &g_nMedianBlurValue,
            50, on_MedianBlur);
    /* ========== */
    namedWindow(win_name[4], 1);
    createTrackbar("core : ", win_name[4],
            &g_nBilateralFilterValue,
            50, on_BilateralFilter);

    while(waitKey() != 'q')
        ;
    return 0;
}

static void on_BoxFilter(int slider, void *userdata) {
    boxFilter(g_srcImage, g_dstImage_box, -1,
            Size(g_nBoxFilterValue + 1, g_nBoxFilterValue + 1));
    imshow(win_name[0], g_dstImage_box);
}

static void on_MeanBlur(int slider, void *userdata) {
    blur(g_srcImage, g_dstImage_mean,
            Size(g_nMeanBlurValue + 1, g_nMeanBlurValue + 1), Point(-1, -1));
    imshow(win_name[1], g_dstImage_mean);
}

static void on_GaussianBlur(int slider, void *userdata) {
    GaussianBlur(g_srcImage, g_dstImage_gaussian,
            Size(g_nGaussianBlurValue*2 + 1, g_nGaussianBlurValue*2 + 1), 0, 0);
    imshow(win_name[2], g_dstImage_gaussian);
}

static void on_MedianBlur(int slider, void *userdata) {
    medianBlur(g_srcImage, g_dstImage_median,
            g_nMedianBlurValue * 2 + 1);
    imshow(win_name[3], g_dstImage_median);
}

static void on_BilateralFilter(int slider, void *userdata) {
    bilateralFilter(g_srcImage, g_dstImage_bilateral,
            g_nBilateralFilterValue, g_nBilateralFilterValue * 2,
            g_nBilateralFilterValue / 2);
    imshow(win_name[4], g_dstImage_bilateral);
}
