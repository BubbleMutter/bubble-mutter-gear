/* merge() and split core usage,
 *
 * vector<Mat> channels;
 * Mat  imageBlueChannels,
 *      imageGreenChannels,
 *      imageRedChannels
 *      srcImage,
 *      dstImage;
 * srcImage = imread("picture.jpg");
 * split(srcImage, channels);
 *
 * imageBlueChannels = channels.at(0);
 * imageGreenChannels = channels.at(1);
 * imageRedChannels = channels.at(2);
 *
 * ... manipulate each channels
 *
 * merge(channels, dstImage);
 */
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<vector>
#include<cstdio>

using namespace std;
using namespace cv;

bool MultiChannelBlending();
void ShowHelpText();

int main(int argc, char **argv) {
    MultiChannelBlending();
    waitKey();
    return 0;
}

bool MultiChannelBlending(void) {
    /* multi-channel-blending, blue-channel part */
    Mat srcImage,
        logoImage,
        ROI;
    vector<Mat> channels;

#if 1 /* merge blue channel */
    Mat imageBlueChannel;

    /* [1] read */
    logoImage = imread("dota_logo.jpg", 0); /* must be grey image */
    srcImage = imread("dota_jugg.jpg");

    if(!logoImage.data) {
        fprintf(stderr, "Can not read logoImage.\n");
        return -1;
    }
    if(!srcImage.data) {
        fprintf(stderr, "Can not read srcImage.\n");
        return -1;
    }
    /* [2] split a 3-channels image into 3 single-channel images */
    /* channels.at(0) blue
     * channels.at(1) green
     * channels.at(2) red */
    split(srcImage, channels); /* split the channels */
    /* [3] refers srcImage to the blue-channel */
    imageBlueChannel = channels.at(0); /* shared memory space */
    /* [4] addWeight */
    ROI = imageBlueChannel(Rect(500, 250, logoImage.cols, logoImage.rows));
    addWeighted(ROI, 1.0, logoImage, 0.5, 0.0, ROI);

    /* merge the 3 single-channel images into a 3-channels image */
    merge(channels, srcImage);

    namedWindow("[1] original image + blue channel");
    imshow("[1] original image + blue channel", srcImage);
#endif /* merge blue channel */

#if 1 /* merge green channel */
    Mat imageGreenChannel;

    /* [1] read */
    logoImage = imread("dota_logo.jpg", 0); /* must be grey image */
    srcImage = imread("dota_jugg.jpg");

    if(!logoImage.data) {
        fprintf(stderr, "Can not read logoImage.\n");
        return -1;
    }
    if(!srcImage.data) {
        fprintf(stderr, "Can not read srcImage.\n");
        return -1;
    }
    /* [2] split a 3-channels image into 3 single-channel images */
    /* channels.at(0) blue
     * channels.at(1) green
     * channels.at(2) red */
    split(srcImage, channels); /* split the channels */
    /* [3] refers srcImage to the green-channel */
    imageGreenChannel = channels.at(1); /* shared memory space */
    /* [4] addWeight */
    ROI = imageGreenChannel(Rect(500, 250, logoImage.cols, logoImage.rows));
    addWeighted(ROI, 1.0, logoImage, 0.5, 0.0, ROI);

    /* merge the 3 single-channel images into a 3-channels image */
    merge(channels, srcImage);

    namedWindow("[1] original image + green channel");
    imshow("[1] original image + green channel", srcImage);
#endif /* merge green channel */

#if 1 /* merge red channel */
    Mat imageRedChannel;

    /* [1] read */
    logoImage = imread("dota_logo.jpg", 0); /* must be grey image */
    srcImage = imread("dota_jugg.jpg");

    if(!logoImage.data) {
        fprintf(stderr, "Can not read logoImage.\n");
        return -1;
    }
    if(!srcImage.data) {
        fprintf(stderr, "Can not read srcImage.\n");
        return -1;
    }
    /* [2] split a 3-channels image into 3 single-channel images */
    /* channels.at(0) blue
     * channels.at(1) green
     * channels.at(2) red */
    split(srcImage, channels); /* split the channels */
    /* [3] refers srcImage to the red-channel */
    imageRedChannel = channels.at(2); /* shared memory space */
    /* [4] addWeight */
    ROI = imageRedChannel(Rect(500, 250, logoImage.cols, logoImage.rows));
    addWeighted(ROI, 1.0, logoImage, 0.5, 0.0, ROI);

    /* merge the 3 single-channel images into a 3-channels image */
    merge(channels, srcImage);

    namedWindow("[1] original image + red channel");
    imshow("[1] original image + red channel", srcImage);
#endif /* merge red channel */
}
