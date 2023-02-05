/* Basic Figure Draw Functions
 * fork from  OpenCV3ProgramingIntroduction
 */
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;
#define WINDOW_WIDTH (600)

void DrawEllipse(Mat img, double angle) {
    int thickness = 2,
        lineType = 8;

    ellipse(img, Point(WINDOW_WIDTH/2, WINDOW_WIDTH/2),
            Size(WINDOW_WIDTH/4, WINDOW_WIDTH/16), angle,
            0, 360, Scalar(255, 129, 0), thickness, lineType);
}

void DrawFilledCircle(Mat img, Point center) {
    int thickness = 2, lineType = 8;

    circle(img,
            center,
            WINDOW_WIDTH/32,
            Scalar(0, 0, 255),
            thickness,
            lineType);
}

void DrawPolygon(Mat img) {
    int lineType = 8;
    // create some points
    Point rookPoints[20];
    rookPoints[0] = Point(WINDOW_WIDTH / 4, 7 * WINDOW_WIDTH / 8);
    rookPoints[1] = Point(3 * WINDOW_WIDTH / 4, 7 * WINDOW_WIDTH / 8);
    rookPoints[2] = Point(3 * WINDOW_WIDTH / 4, 13 * WINDOW_WIDTH / 16);
    rookPoints[3] = Point(11 * WINDOW_WIDTH / 16, 13 * WINDOW_WIDTH / 16);
    rookPoints[4] = Point(19 * WINDOW_WIDTH / 32, 3 * WINDOW_WIDTH / 8);
    rookPoints[5] = Point(3 * WINDOW_WIDTH / 4, 3 * WINDOW_WIDTH / 8);
    rookPoints[6] = Point(3 * WINDOW_WIDTH / 4, WINDOW_WIDTH / 8);
    rookPoints[7] = Point(26 * WINDOW_WIDTH / 40, WINDOW_WIDTH / 8);
    rookPoints[8] = Point(26 * WINDOW_WIDTH / 40, WINDOW_WIDTH / 4);
    rookPoints[9] = Point(22 * WINDOW_WIDTH / 40, WINDOW_WIDTH / 4);

    rookPoints[10] = Point(22 * WINDOW_WIDTH / 40, WINDOW_WIDTH / 8);
    rookPoints[11] = Point(18 * WINDOW_WIDTH / 40, WINDOW_WIDTH / 8);
    rookPoints[12] = Point(18 * WINDOW_WIDTH / 40, WINDOW_WIDTH / 4);
    rookPoints[13] = Point(14 * WINDOW_WIDTH / 40, WINDOW_WIDTH / 4);
    rookPoints[14] = Point(14 * WINDOW_WIDTH / 40, WINDOW_WIDTH / 8);
    rookPoints[15] = Point(WINDOW_WIDTH / 4, WINDOW_WIDTH / 8);
    rookPoints[16] = Point(WINDOW_WIDTH / 4, WINDOW_WIDTH / 8);
    rookPoints[17] = Point(13 * WINDOW_WIDTH / 32, 3 * WINDOW_WIDTH / 8);
    rookPoints[18] = Point(5 * WINDOW_WIDTH / 16, 13 * WINDOW_WIDTH / 16);
    rookPoints[19] = Point(WINDOW_WIDTH / 4, 13 * WINDOW_WIDTH / 16);

    const Point *ppt = rookPoints;
    int npt = 20;
    fillPoly(img, &ppt, &npt, 1, Scalar(255, 255, 255), lineType);
}

void DrawLine(Mat img, Point start, Point end) {
    int thickness = 2;
    int lineType = 8;
    line(img, start, end, Scalar(0, 0, 0), thickness, lineType);
}

int main(int argc, char **argv) {
    Mat img(600, 800, CV_8UC3, Scalar(0, 0, 0));
    Mat imgTemp;

    imgTemp = img.clone();
    Point circle_center(600/2, 800/2);
    DrawFilledCircle(imgTemp, circle_center);
    imshow("title", imgTemp);
    waitKey();

    imgTemp = img.clone();
    DrawPolygon(imgTemp);
    imshow("title", imgTemp);
    waitKey();

    Mat imgrect(2, 2, CV_8UC3);
    Mat imgRow = imgrect.row(1).clone();
    cout << imgRow;
    return 0;
}
