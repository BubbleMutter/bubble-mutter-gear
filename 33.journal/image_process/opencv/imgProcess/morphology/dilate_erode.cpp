#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace cv;
using namespace std;

Mat g_srcImage,
    g_dstImage;
int g_nTrackbarNumer = 0,   // 0 means erode, 1 means dilate
    g_nStructElementSize = 3; // core size

void Process();
void on_TrackbarNumChange(int slider, void *userdata);
void on_ElementSizeChange(int slider, void *userdata);

int main(int argc, char **argv) {
    g_srcImage = imread("./juggernaut.jpg");
    if(!g_srcImage.data)
        return -1;
    //resize(g_srcImage, g_srcImage, Size(480, 320));
    imshow("src image", g_srcImage);

    namedWindow("dst image");
    Mat element = getStructuringElement(MORPH_RECT,
            Size(2*g_nStructElementSize+1, 2*g_nStructElementSize+1),
            Point(g_nStructElementSize, g_nStructElementSize));
    erode(g_srcImage, g_dstImage, element);
    imshow("dst image", g_dstImage);

    createTrackbar("erode/dilate", "dst image",
            &g_nTrackbarNumer, 1, on_TrackbarNumChange);
    createTrackbar("core size : ", "dst image",
            &g_nStructElementSize, 21, on_ElementSizeChange);
    while(waitKey() != 'q')
        ;
    return 0;
}

void Process() {
    // obtain user-defined core
    Mat element = getStructuringElement(MORPH_RECT,
            Size(2*g_nStructElementSize+1, 2*g_nStructElementSize+1),
            Point(g_nStructElementSize, g_nStructElementSize));
    // perform erode or dilate
    if(g_nTrackbarNumer == 0)
        erode(g_srcImage, g_dstImage, element);
    else
        dilate(g_srcImage, g_dstImage, element);
    imshow("dst image", g_dstImage);
}

void on_TrackbarNumChange(int slider, void *userdata) {
    Process();
}
void on_ElementSizeChange(int slider, void *userdata) {
    Process();
}
