#include<opencv2/opencv.hpp>
#include<cstdlib>
using namespace cv;
using namespace std;

#define WINDOW_NAME ("window")
/* global data */
Rect g_rectangle;
bool g_bDrawingBox = false;
RNG g_rng(12345);

/* global function declare */
void DrawRectangle(cv::Mat & img, cv::Rect box) {
    rectangle(img, box.tl(), box.br(), Scalar(g_rng.uniform(0, 255),
                g_rng.uniform(0, 255), g_rng.uniform(0,255))); /* random color */
}

void onMouseHandle(int event,
        int x, int y,
        int flags, void *param) {
    Mat & image = *(Mat *)param;
    Mat tempImage = image.clone();

    switch (event) {
        case EVENT_MOUSEMOVE:
            if(g_bDrawingBox) {
                g_rectangle.width = x - g_rectangle.x;
                g_rectangle.height = y - g_rectangle.y;
#if 0   /* width, height < 0 situation */
                if(g_rectangle.width < 0) {
                    g_rectangle.x += g_rectangle.width;
                    g_rectangle.width *= -1;
                }

                if(g_rectangle.height < 0) {
                    g_rectangle.y += g_rectangle.height;
                    g_rectangle.height *= -1;
                }
#endif
                DrawRectangle(tempImage, g_rectangle);
            }
            imshow(WINDOW_NAME, tempImage);
            break;

        case EVENT_LBUTTONDOWN:
            /* start to draw, record the start-point */
            g_bDrawingBox = true;
            g_rectangle = Rect(x, y, 0, 0);
            break;

        case EVENT_LBUTTONUP:
            /* stop to draw, record the end-point */
            g_bDrawingBox = false;
#if 0   /* width, height < 0 situation */
            if(g_rectangle.width < 0) {
                g_rectangle.x += g_rectangle.width;
                g_rectangle.width *= -1;
            }

            if(g_rectangle.height < 0) {
                g_rectangle.y += g_rectangle.height;
                g_rectangle.height *= -1;
            }
#endif
            /* draw */
            DrawRectangle(image, g_rectangle);
            break;
    }
}

int main(int argc, char **argv) {
    /* [1] prepare the parameters */
    g_rectangle = Rect(-1, -1, 0, 0);
    Mat srcImage(600, 800, CV_8UC3);

    /* [2] set the mouse_manipulate callback */
    namedWindow(WINDOW_NAME);
    setMouseCallback(WINDOW_NAME, onMouseHandle, (void *)&srcImage);

    imshow(WINDOW_NAME, srcImage);
    while(waitKey(10) != 27)
        ; /* ESC */

    return 0;
}
