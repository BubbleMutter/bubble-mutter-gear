#include<opencv2/opencv.hpp>
#include<ctime>
#include<cstdlib>
#include<vector>
using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    FileStorage fs2("test.yaml", FileStorage::READ);
    int frameCount = (int)fs2["frameCount"];
    string date;
    fs2["calibrationDate"] >> date;

    Mat cameraMatrix2, distCoeffs2;
    fs2["cameraMatrix"] >> cameraMatrix2;
    fs2["distCoeffs"] >> distCoeffs2;

    cout << "frameCount: " << frameCount << endl
        << "calibrationDate: " << date << endl
        << "camera matrix: " << cameraMatrix2 << endl
        << "distortion coeffs: " << distCoeffs2 << endl;
    FileNode features = fs2["features"];
    FileNodeIterator itrb = features.begin(),
                     itre = features.end();
    int idx = 0;
    vector<uchar> lbpval;

    /* traverse the list */
    for(; itrb != itre; itrb++, idx++) {
        cout << "feature #" << idx << ": "
            << "x = " << (int)(* itrb)["x"] << ", y =" << (int)(* itrb)["y"] << ", lbp: (";
        (* itrb)["lbp"] >> lbpval;

        for(int i = 0; i < (int)lbpval.size(); i++)
            cout << " " << (int)lbpval[i];
        cout << ")" << endl;
    }
    return 0;
}
