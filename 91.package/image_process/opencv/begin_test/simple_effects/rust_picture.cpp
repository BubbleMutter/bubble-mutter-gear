#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
using namespace cv;

#define IMG_PATH ("./picture.png")
#define SRC_TITLE ("[src] image rust")
#define DST_TITLE ("[dst] image rust")
int main(int argc, char **argv) {
    // read in a picture
    Mat src_img,
        dst_img;

    src_img = imread(IMG_PATH);
    imshow(SRC_TITLE, src_img);

    /* perform rust */
    Mat element = /* obtain core matrix */
        getStructuringElement(MORPH_RECT, Size(10, 10));
    erode(src_img, dst_img, element); /* do rust */

    /* show the result */
    imshow(DST_TITLE, dst_img);
    waitKey();

    return 0;
}
