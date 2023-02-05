#include<opencv2/opencv.hpp>
#include<vector>
#include<iostream>
#include<stdexcept>
#include<cstdio>

using namespace cv;
using namespace std;

#define colorReduceExpression(x, div) do { x = (x / div*div) + (div/2); } while(0)

/* three way to access pixel */
void colorReduce(Mat & srcImage,
        Mat & dstImage,
        int div) {
    dstImage = srcImage.clone();

#if 1 /* [1] pointer access pixel method */
    /* Mat.ptr(column), uchar * pixel */
    int rowNumber = dstImage.rows;
    int colNumber = dstImage.cols * dstImage.channels();
    for(int i = 0; i < rowNumber; i++) {
        /* Mat member ->ptr(i), head-address of No.i row */
        uchar * data = dstImage.ptr<uchar>(i);
        // uchar * data = dstImage.data;
        for(int j = 0; j < colNumber; j++) {
            colorReduceExpression(data[j], div);
        } /* end of a row */
    }
    return;
#endif

#if 0 /* [2] iterator access pixel */
    Mat_<Vec3b>::iterator itrb = dstImage.begin<Vec3b>(); // begin
    Mat_<Vec3b>::iterator itre = dstImage.end<Vec3b>(); // end
    for(; itrb != itre; itrb++) {
        colorReduceExpression( (* itrb)[0], div ); /* blue */
        colorReduceExpression( (* itrb)[1], div ); /* green */
        colorReduceExpression( (* itrb)[2], div ); /* red */
    }
#endif

#if 0 /* [3] dynamic address calculate access pixel */
    /* Mat.at<Vec3b>(row, col)[0], means blue
     * Mat.at<Vec3b>(row, col)[1], means green
     * Mat.at<Vec3b>(row, col)[2], means red
     */
    int rowNumber = dstImage.rows;
    int colNumber = dstImage.cols;

    for(int i = 0; i < rowNumber; i++) {
        for(int j = 0; j < colNumber; j++) {
            colorReduceExpression( dstImage.at<Vec3b>(i, j)[0], div );
            colorReduceExpression( dstImage.at<Vec3b>(i, j)[1], div );
            colorReduceExpression( dstImage.at<Vec3b>(i, j)[2], div );
        }
    }
#endif
}

/* copyTo() with mask */
void ROI_AddImage(Mat srcImage, const char * logo_file) {
    Mat logo = imread(logo_file);

    Mat ROI = srcImage(Rect(200, 250, logo.cols, logo.rows));

    // [1] load mask, must be gray image
    Mat mask = imread(logo_file, 0);
    // [2] mask the ROI, copy the logo to ROI
    logo.copyTo(ROI, mask);
}

/* addWeighted() */
void LinearBlending(Mat srcImage1,
        Mat srcImage2,
        Mat dstImage) {
    /* srcImage1 and srcImage2 should have the same
     * size and channels */
    /* dst = src1[I]*alpha + src2[I]*beta + gamma; */
    double alphaValue;
    double betaValue;
    double gamma;

    alphaValue = 0.5;
    betaValue = 1 - alphaValue;
    gamma = 0.0;

    addWeighted(srcImage1, alphaValue, srcImage2, betaValue,
            gamma, dstImage);
    return;
}

int main(int argc, char **argv) {
    Mat src;
    Mat dst;
    colorReduce(src, dst, 32);
    return 0;
}
