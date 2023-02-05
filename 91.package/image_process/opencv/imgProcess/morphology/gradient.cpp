#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace cv;
using namespace std;

Mat g_srcImage, g_dstImage;

const char *win_name = "gradient";

int g_nElementShape = MORPH_RECT; //

/* global vars for TrackBar output-params */
int g_nMaxIterationNum = 10;
int g_nGradientNum = 0;

static void on_Gradient(int slider, void *userdata);

int main(int argc, char **argv) {
    g_srcImage = imread("riki_bh.jpg", 0);
    if(!g_srcImage.data)
        return -1;
    resize(g_srcImage, g_srcImage, Size(600, 400));
    namedWindow("srcImage");
    imshow("srcImage", g_srcImage);

    // 3 windows
    namedWindow(win_name, 1);

    // create trackbar for each window
    createTrackbar("iterator value : ", win_name,
            &g_nGradientNum, g_nMaxIterationNum, on_Gradient);

    while(1) {
        int c;
        on_Gradient(g_nGradientNum, 0);

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

static void on_Gradient(int slider, void *userdata) {
    // absolute value of offset
    Mat element = getStructuringElement(g_nElementShape,
            Size(2*g_nGradientNum+1, 2*g_nGradientNum+1),
            Point(g_nGradientNum, g_nGradientNum));
        morphologyEx(g_srcImage, g_dstImage,
                MORPH_GRADIENT, element);
    imshow(win_name, g_dstImage);
}
