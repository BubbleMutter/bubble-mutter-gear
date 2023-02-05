#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

#include<unistd.h>
#include<cstdio>
using namespace cv;
#define dev_name ("/dev/video0")

/* ===== global variables ===== */
Mat g_srcImage, g_srcGrayImage, g_dstImage;
// Canny related vars
Mat g_cannyDetectEdges;
int g_cannyLowThreshold = 1; // TrackBar position param

// Sobel related vars
Mat g_sobelGradient_X, g_sobelGradient_Y;
Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;
int g_sobelKernelSize = 1; // TrackBar position param

// Scharr filtor relate
Mat g_scharrGradient_X, g_scharrGradient_Y;
Mat g_scharrAbsGradient_X, g_scharrAbsGradient_Y;

/* ==== global function declare ==== */
static void on_Canny(int, void *); // Canny Window Callback func
static void on_Sobel(int, void *); // Sobel window Callback func
static void Scharr(); // a wrapper func of Scharr-Edges-Detect related interface

int main(int argc, char **argv) {
    VideoCapture cap(0);
    for(int i = 0; i < 2 * 60 * 1000; i++) {
        cap >> g_srcImage;
        namedWindow("src");
        imshow("src", g_srcImage);

        // create the dst-rectangle with the same size of src
        g_dstImage.create(g_srcImage.size(), g_srcImage.type());

        // convert the src to gray image
        cvtColor(g_srcImage, g_srcGrayImage, COLOR_BGR2GRAY);

        // create window
        namedWindow("dst Canny", WINDOW_AUTOSIZE);
        namedWindow("dst Sobel", WINDOW_AUTOSIZE);
        // create trackbar
        createTrackbar("value: ", "dst Canny", &g_cannyLowThreshold, 120, on_Canny);
        createTrackbar("value: ", "dst Sobel", &g_sobelKernelSize, 3, on_Sobel);

        // start callback function
        on_Canny(0, 0);
        on_Sobel(0, 0);

        // Scharr
        Scharr();
        if(waitKey(1) == 'q')
            break;
    }
    return 0;
}

static void on_Canny(int, void *) {
    // denoise
    blur(g_srcGrayImage, g_cannyDetectEdges, Size(3,3));
    Canny(g_cannyDetectEdges, g_cannyDetectEdges,
            g_cannyLowThreshold, g_cannyLowThreshold * 3);
    g_dstImage = Scalar::all(0);
    g_srcImage.copyTo(g_dstImage, g_cannyDetectEdges);
    imshow("dst Canny", g_dstImage);
}

static void on_Sobel(int, void *) {
    // find the gradient on X-direction
    Sobel(g_srcImage, g_sobelGradient_X, CV_16S, 1, 0,
            (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
    // canculate the absolute value
    convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);

    // find the gradient on Y-direction
    Sobel(g_srcImage, g_sobelGradient_Y, CV_16S, 1, 0,
            (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
    // canculate the absolute value
    convertScaleAbs(g_sobelGradient_Y, g_sobelAbsGradient_Y);

    // combine gradient
    addWeighted(g_sobelAbsGradient_X, 0.5,
            g_sobelAbsGradient_Y, 0.5,
            0, g_dstImage);
    imshow("dst Sobel", g_dstImage);
}

static void Scharr(void) {
    // find the gradient on X-direction
    Sobel(g_srcImage, g_scharrGradient_X, CV_16S, 1, 0,
            1, 0, BORDER_DEFAULT);
    // calculate the absolute value
    convertScaleAbs(g_scharrGradient_X, g_scharrAbsGradient_X);

    // find the gradient on Y-direction
    Scharr(g_srcImage, g_scharrGradient_Y, CV_16S, 1, 0,
            1, 0, BORDER_DEFAULT);
    // calculate the absolute value
    convertScaleAbs(g_scharrGradient_Y, g_scharrAbsGradient_Y);

    addWeighted(g_scharrAbsGradient_X, 0.5,
            g_scharrAbsGradient_Y, 0.5,
            0, g_dstImage);
    imshow("dst Scharr", g_dstImage);
}
