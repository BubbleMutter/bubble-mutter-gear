#include<opencv2/opencv.hpp>
#include<iostream>
#include<cstdio>
using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    Mat r = Mat(10, 3, CV_8UC3);
    randu(r, Scalar::all(0), Scalar::all(255));

#if 1 /* not really friendly output style */
    cout << "r (OpenCV default Style) = " << r << endl;
    getchar();

    cout << "r (Comma-Divide Style) = " << format(r, Formatter::FMT_CSV) << endl;
    getchar();

    cout << "r (Numpy Style) = " << format(r, Formatter::FMT_NUMPY) << endl;
    getchar();

    cout << "r (C lang Style) = " << format(r, Formatter::FMT_C) << endl;
    getchar();
#endif

    cout << "r (Python Style) = " << format(r, Formatter::FMT_PYTHON) << endl;
    getchar();

    Point2f point(6, 2);
    cout << "a 2D point = " << point << endl;
    getchar();

    Point3f p3f(8, 2, 0);
    cout << "a 3D point = " << p3f << endl;
    getchar();

    vector<float> vt;
    vt.push_back(3);
    vt.push_back(5);
    vt.push_back(7);
    cout << "[Mat-Basic Vector] shortvec = " << Mat(vt) << endl;

    vector<Point2f> points(20);
    for(size_t i = 0; i < points.size(); i++)
        points[i] = Point2f((float)(i * 5), (float)(i % 7));
    cout << " 2D points vector = " << points << endl;

    return 0;
}
