#include<opencv2/opencv.hpp>
#include<vector>
#include<iostream>
#include<stdexcept>
#include<cstdio>

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    Mat img,
        dota_image,
        dota_logo;

    img = imread("lina.jpg");
    imshow("[1] lina", img);
    waitKey();

    dota_image = imread("dr_lina.jpg", 2 | 4);
    imshow("[2] orignal painting", dota_image);
    waitKey();

    Mat imageROI;
    imageROI = img(Rect(30, 200, dota_image.cols, dota_image.rows));
    addWeighted(imageROI, -0.3, dota_image, -1.0, 0, imageROI);
    imshow("[3] lina with sister", img);
    waitKey();
    imwrite("generate_image_lina.png", img);

    return 0;
}
