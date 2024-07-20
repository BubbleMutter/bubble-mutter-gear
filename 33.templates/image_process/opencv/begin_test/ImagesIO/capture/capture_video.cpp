#include<opencv2/opencv.hpp>
#include<cstring>
using namespace std;
using namespace cv;

#define VIDEO_PATH ("./video.mp4")
#define DST_TITLE ("[src] Capture Video")
#define SRC_TITLE ("[dst] Canny Edge Detection")

int main(int argc, char **argv) {
    Mat frame,
        edges;
    VideoCapture capture;

    if(argc != 2)
        return -1;
    else if(!strncmp(argv[1], "1", 1))
        goto video_from_file;
    else if(!strncmp(argv[1], "2", 1))
        goto video_from_camera;
    else if(!strncmp(argv[1], "3", 1))
        goto video_from_camera_to_gray;
    else
        return -1;

video_from_file:
    capture.open(VIDEO_PATH);
    while(1) {
        capture >> frame;
        if(frame.empty())
            return 0;

        imshow(SRC_TITLE, frame);
        if(waitKey(30) >= 0) /* 30 ms */
            return 0;
    }

video_from_camera:
    capture.open(0);
    while(1) {
        capture >> frame;

        imshow(SRC_TITLE, frame);
        if(waitKey(30) >= 0)
            return 0; /* 30 ms */
    }

video_from_camera_to_gray:
    capture.open(0);
    while(1) {
        capture >> frame;
        // convert to gray image
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        /* denoise with 3x3 core,
         * 2 x 3 + 1 = 7 */
        blur(edges, edges, Size(7, 7)); // blur

        // perform canny edge detection
        Canny(edges, edges, 0, 30, 3);
        imshow(DST_TITLE, edges);
        if(waitKey(30) >= 0)
            return 0;
    }
    return 0;
}
