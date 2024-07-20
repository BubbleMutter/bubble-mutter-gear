#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
using namespace cv;

#define IMG_PATH ("./picture.png")
#define SRC_TITLE ("[src] Mean Filtering")
#define DST_TITLE ("[dst] Mean Filtering")

int main(int argc, char **argv) {
    Mat src_img,
        dst_img;

    src_img = imread(IMG_PATH);
    imshow(SRC_TITLE, src_img);

    /* perform blur */
    blur(src_img, dst_img, Size(7, 7));

    imshow(DST_TITLE, dst_img);
    waitKey();

    return 0;
}
