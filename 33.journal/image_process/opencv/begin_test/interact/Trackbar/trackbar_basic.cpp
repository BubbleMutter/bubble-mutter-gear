#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cstdio>
using namespace cv;
using namespace std;

#define WINDOW_NAME ("linear combination")
#define SRC_IMG_1 ("./dr_lina.jpg")
#define SRC_IMG_2 ("./lina.jpg")
/* global data */
const int g_nMaxAlphaValue = 100; // alpha max
int g_nAlphaValueSlider;
double g_dAlphaValue;
double g_dBetaValue;

Mat g_srcImage1;
Mat g_srcImage2;
Mat g_dstImage;

bool cflags;
/* on_Trackbar, Trackbar-callback function */
void on_Trackbar(int ,
        void *) {
#if 1
    /* alpha scale of the max-alpha */
    g_dAlphaValue = (double)g_nAlphaValueSlider / g_nMaxAlphaValue;
    printf("%d, %d, %f\n", g_nAlphaValueSlider,
            g_nMaxAlphaValue, g_dAlphaValue);

    g_dBetaValue = (1.0 - g_dAlphaValue);

    Mat ROI = g_srcImage2(Rect(550, 300, g_srcImage1.cols, g_srcImage1.rows));
    /* linear combination with alpha and beta */
    addWeighted(ROI, 0.5, g_srcImage1, g_dAlphaValue, 0.0, ROI);

    imshow(WINDOW_NAME, g_srcImage2);
    waitKey();
#endif
}

int main(int argc, char **argv) {
    g_srcImage1 = imread(SRC_IMG_1);
    g_srcImage2 = imread(SRC_IMG_2);

    if(!g_srcImage1.data) {
        cout << "Can't find the image: " << SRC_IMG_1 << endl;
        return -1;
    }

    if(!g_srcImage2.data) {
        cout << "Can't find the image: " << SRC_IMG_2 << endl;
        return -1;
    }

    g_nAlphaValueSlider = 70; /* default value */
    namedWindow(WINDOW_NAME, 1);

    string TrackbarName = ""; TrackbarName = "alpha value 100";
    string win_name = WINDOW_NAME;
    createTrackbar(TrackbarName, win_name,
            &g_nAlphaValueSlider, g_nMaxAlphaValue, on_Trackbar);

    on_Trackbar(g_nAlphaValueSlider, NULL);
    while(1)
        waitKey();


    return 0;
}
