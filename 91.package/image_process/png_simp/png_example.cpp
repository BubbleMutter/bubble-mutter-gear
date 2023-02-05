#include<png.h>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

#include<cstring>
#include<cstdlib>
#include<iostream>
#include<vector>
using std::vector;
using std::cout;
using std::endl;

using cv::Mat;
using cv::imshow;
using cv::imread;
using cv::waitKey;
#define file_name "ArmFamily.png"
#define newpng "new.png"

int main( int argc, char **argv ) {
    Mat srcMat;
    srcMat = imread(file_name, CV_LOAD_IMAGE_COLOR);

    png_image image; /* The control structure used by libpng */
    memset(&image, 0, (sizeof image));

   cout << "--------" << endl;
   cout << "PNG_FORMAT_GRAY " << PNG_FORMAT_GRAY << endl;
   cout << "PNG_FORMAT_GA " << PNG_FORMAT_GA << endl;
   cout << "PNG_FORMAT_AG  " << PNG_FORMAT_AG << endl;
   cout << "PNG_FORMAT_RGB " << PNG_FORMAT_RGB << endl;
   cout << "PNG_FORMAT_BGR " << PNG_FORMAT_BGR << endl;
   cout << "PNG_FORMAT_RGBA " << PNG_FORMAT_RGBA << endl;
   cout << "PNG_FORMAT_ARGB " << PNG_FORMAT_ARGB << endl;
   cout << "PNG_FORMAT_BGRA " << PNG_FORMAT_BGRA << endl;
   cout << "PNG_FORMAT_ABGR " << PNG_FORMAT_ABGR << endl;
   cout << "--------" << endl;

   Mat pngMat(srcMat.cols, srcMat.rows, CV_8UC3);
   cv::cvtColor(srcMat, pngMat, CV_BGR2RGB, pngMat.channels());

   image.version = PNG_IMAGE_VERSION;
   image.width = pngMat.cols;
   image.height = pngMat.rows;
   image.format = PNG_FORMAT_RGB;
   image.flags = 0;
   image.colormap_entries = 256;
   image.warning_or_error = 0;

    png_bytep newPngBuf = (png_bytep)malloc(pngMat.rows * pngMat.step);
    png_alloc_size_t newPngBufSize; /* an inout-param */
    newPngBufSize = pngMat.rows * pngMat.step;
    int ret;
    ret = png_image_write_to_memory(&image,
            newPngBuf, /* an inoutparam param */&newPngBufSize,
            /* convert_to_8bit */0, pngMat.data,
            /* row_stride */pngMat.step, NULL);
    cout << "ret is " << ret << endl;
    newPngBuf = (png_bytep)realloc(newPngBuf, newPngBufSize);
#if 1 /* write */
    ret = png_image_begin_read_from_memory(&image, newPngBuf, newPngBufSize);
    png_bytep buffer = (png_bytep)malloc(PNG_IMAGE_SIZE(image));
    png_image_finish_read(&image, NULL, buffer, 0, NULL);

    //image.format = PNG_FORMAT_RGBA;
    ret = png_image_write_to_file(&image, "darkness.png",
            0/*convert_to_8bit*/,
            buffer,
            pngMat.step/*row_stride*/,
            NULL/*colormap*/);
    if(ret != 0)
        fprintf(stderr, "write success\n");
    else
        fprintf(stderr, "write failure\n");
#endif

    //imshow("img", pngMat);
    //waitKey(0);
    return 0;
}
