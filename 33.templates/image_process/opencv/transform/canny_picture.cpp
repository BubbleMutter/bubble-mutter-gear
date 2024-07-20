#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

#include<unistd.h>
using namespace cv;

#define IMG_PATH ("./picture.png")
#define SRC_TITLE ("[src] Canny Edge Detection")
#define DST_TITLE ("[dst] Canny Edge Detection")

int main(int argc, char **argv) {
    // read in a picture
    Mat src_img,
        dst_img,
        gray_img; /* gray image */

    for(int i = 0; i < 30; i++) {
        VideoCapture cap;
        cap.open(0);
        cap >> src_img;
        namedWindow(SRC_TITLE);
        imshow(SRC_TITLE, src_img);
        waitKey(1);

        /* convert the src_img to gray_img */
        cvtColor(src_img, gray_img, COLOR_BGR2GRAY);

        /* denoise with 3x3 core */
        blur(gray_img, dst_img, Size(5, 5));

        /*  run canny functor */
        Canny(dst_img, dst_img,
                3,  /* first threshold */
                9,  /* second threshold, 3 times of first one */
                3); /* apetureSize, Sobel-Functor-Size, default 3 */

        /* show the result */
        namedWindow(DST_TITLE);
        imshow(DST_TITLE, dst_img);
        waitKey(1);
        usleep(100 * 1000);
    }

    return 0;
}
