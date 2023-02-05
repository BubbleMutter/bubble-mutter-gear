#include <Magick++.h>
#include <zbar.h>
#include<iostream>
#include"barCode.hpp"
using namespace std;
using namespace zbar;

barCode::barCode(const void *imgData,
        int width, int height) {
    __barCodeSymbol = "";
    Image image(width, height, "Y800", imgData, width * height);

    ImageScanner scanner;
    // configure the reader
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
    // scan the image for barcodes
    scanner.scan(image);

    Image::SymbolIterator symbol = image.symbol_begin();

    __barCodeSymbol = symbol->get_data();
    image.set_data(NULL, 0);
}

const string barCode::barCodeSymbol(void) {
    return __barCodeSymbol;
}

