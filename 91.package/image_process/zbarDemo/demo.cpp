 /* this header is just for me to obtain image data */
#include <Magick++.h>

#include<iostream>
using namespace std;
#include"barCode.hpp"

int main(int argc, char **argv) {
    /* [1] <<== check input ==>> */
    string imgFileName;
    if(argc == 2)
        imgFileName = argv[1];
    else
        imgFileName = "./barcode.png";

    /* [2] <<== obtain image data ==>> */
    const void *imgData;
    Magick::Image magick(imgFileName);  // read an image file
    int width = magick.columns();   // extract dimensions
    int height = magick.rows();
    Magick::Blob blob;              // extract the raw data
    magick.modifyImage();
    magick.write(&blob, "GRAY", 8);
    imgData = blob.data();

    /* [3] class-init, and parse */
    barCode oneDimCode(imgData, width, height);

    /* [4] output */
    cout << oneDimCode.barCodeSymbol() << endl;

    return 0;
}
