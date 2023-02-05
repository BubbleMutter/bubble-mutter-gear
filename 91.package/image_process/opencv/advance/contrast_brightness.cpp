/* perform contrast and brightness
 * dynamic adjust
 * dst(col, row)  = a * src(col, row) + b
 */

#include<opencv2/opencv.hpp>
#include<vector>
#include<iostream>
#include<stdexcept>
#include<cstdio>
const char *win_name = "result window";

using namespace cv;
using namespace std;

Mat g_srcImage,
    g_dstImage;
int g_nContrastValue,
    g_nBrightnessValue;

static void on_ContrastAndBrightness(int slider, void *userdata);

int main(int argc, char **argv) {
    /* [1] read in */
    g_srcImage = imread("../dota2/lina.jpg"); /* can't be grey image */
    g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());

    /* [2] initialize the contrast and brightness */
    g_nContrastValue = 80;
    g_nBrightnessValue = 80;

    /* [3] create windows */
    namedWindow(win_name, 1);
    createTrackbar("contrast : ", win_name, &g_nContrastValue,
            300, on_ContrastAndBrightness);
    createTrackbar("brightness :", win_name, &g_nBrightnessValue,
            200, on_ContrastAndBrightness);
    imshow("original image", g_srcImage);

    /* [4] initialize the callback function */
    on_ContrastAndBrightness(g_nContrastValue, NULL);
    on_ContrastAndBrightness(g_nBrightnessValue, NULL);
    while(waitKey(1) != 'q')
        ;
    return 0;
    return 0;
}

static void on_ContrastAndBrightness(int slider, void *userdata) {
    /* [1] create window */
#if 0 /* vector access method */
    for(int y = 0; y < g_srcImage.rows; y++)
        for(int x = 0; x < g_srcImage.cols; x++)
            for(int c = 0; c < 3; c++) { /* 3 channels */
                uchar *srcPixelc = &g_srcImage.at<Vec3b>(y, x)[c];
                uchar *dstPixelc = &g_dstImage.at<Vec3b>(y, x)[c];
                *dstPixelc = saturate_cast<uchar>(
                        (g_nContrastValue * 0.01) *
                        *srcPixelc + g_nBrightnessValue);
            }
#endif

#if 1 /* pointer access method */
    int chs = g_srcImage.channels();
    for(int y = 0; y < g_srcImage.rows; y++) {
        uchar *srcData = g_srcImage.ptr<uchar>(y);
        uchar *dstData = g_dstImage.ptr<uchar>(y);
        for(int x = 0; x < g_srcImage.cols; x++)
            for(int c = 0; c < chs; c++)
                dstData[x*chs + c] = saturate_cast<uchar>(
                        (g_nContrastValue * 0.01) *
                        srcData[x*chs + c] + g_nBrightnessValue);
    }
#endif

    imshow(win_name, g_dstImage);
}
