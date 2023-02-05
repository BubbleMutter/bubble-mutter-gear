#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace cv;
using namespace std;

Mat g_srcImage, g_dstImage, g_grayImage, g_maskImage;
int g_nFillMode = 1; // floodFill mode
int g_nLowDifference = 20, // maximum of low-diff
    g_nUpDifference = 20; // maximum of up-diff
int g_nConnectivity = 4; // low-8-bit of @flag for floodFill()
int g_bIsColor = true;
int g_bUseMask = false;
int g_nNewMaskVal = 255;

static void onMouse(int event,
        int x, int y, int flags,
        void *userdata) {
    if(event != EVENT_LBUTTONDOWN)
        return;

    /**** prepare for floodFill() parameters ****/
    Point seed = Point(x, y);
    /* blank-area floodFill, either 0 or g_nLowDifference */
    int LowDifference = ((g_nFillMode == 0) ? 0 : g_nLowDifference);
    int UpDifference = ((g_nFillMode == 0) ? 0 : g_nUpDifference);
    /* @flags
     * 0~7 bit, g_nConnectivity,
     * 8~15 bit, g_nNewMaskVal,
     * 16~23 bit, FLOODFILL_FIXED_RANGE or 0
     */
    int floodFill_flags = g_nConnectivity + (g_nNewMaskVal << 8)
        + ((g_nFillMode == 1) ? FLOODFILL_FIXED_RANGE : 0);

    /* random r, g, b value */
    int b = (unsigned)theRNG() & 255;
    int g = (unsigned)theRNG() & 255;
    int r = (unsigned)theRNG() & 255;

    Rect ccomp; // define mimimum-edges rectange-area in redraw-area

    Scalar newVal = g_bIsColor ?
        Scalar(b, g, r)     /* color mode */
        :Scalar(r*0.299 + g*0.587 + b*0.114); /* gray mode */

    Mat dst = g_bIsColor ? g_dstImage : g_grayImage;
    int area;
    /* ======================================== */

    /**** perform real floodFill ******/
    if(g_bUseMask) {
        threshold(g_maskImage, g_maskImage, 1, 128, THRESH_BINARY);

        area = floodFill(dst, g_maskImage, seed, newVal,
                &ccomp, Scalar(LowDifference, LowDifference, LowDifference));
        imshow("mask", g_maskImage);
    } else {
        area = floodFill(dst, seed, newVal, &ccomp,
                Scalar(LowDifference, LowDifference, LowDifference),
                Scalar(UpDifference, UpDifference, UpDifference));
    }
    imshow("dst", dst);
    cout << area << "single pixel has been redrawed" << endl;
}

int main(int argc, char **argv) {
    g_srcImage = imread("riki_bh.jpg", 1);
    if(!g_srcImage.data)
        return -1;

    g_dstImage = g_srcImage.clone();
    cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
    g_maskImage.create(g_srcImage.rows + 2, g_srcImage.cols + 2, CV_8UC1);

    namedWindow("dst", WINDOW_AUTOSIZE);
    return 0;
}
