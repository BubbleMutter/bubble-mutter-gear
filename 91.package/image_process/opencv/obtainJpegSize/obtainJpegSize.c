#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#define MOTOSHORT(pt) (((*pt) << 8) + *(pt + 1))

int iHeight, iWidth;

int obainJPEGsize(const char *filename,
        int *width,
        int *height) {
    unsigned char buf[16];
    FILE *fpJpeg;
    int Marker,
        FileSize,
        nbytes,
        i;
    fpJpeg = fopen(filename, "r");
    Marker = 0;
    i = 2; /* file position indicator */

    /* obtain file size */
    fseek(fpJpeg, 0L, SEEK_END);
    FileSize = ftell(fpJpeg);
    fseek(fpJpeg, 0L, SEEK_SET);

    do {
        fseek(fpJpeg, i, SEEK_SET);
        nbytes = fread(buf, 16, 1, fpJpeg);
        /* nbytes handler */
        Marker = MOTOSHORT(&buf[0]) & 0xfffc; // must Bit-AND 0xfffc
        if(Marker < 0xff00) {
            /* invalid marker, "Arles Image WebPage Creator" or Accusoft */
            i += 2;
            continue;
        }
        if(Marker == 0xffc0)
            break;
        /* normal Marker */
        i += 2 + MOTOSHORT(&buf[2]);
    } while (i < FileSize);

    if(Marker != 0xffc0)
        return -1;
    *height = MOTOSHORT(&buf[5]);
    *width = MOTOSHORT(&buf[7]);
    return 0;
}

int main(int argc, char **argv) {
    obainJPEGsize("dota2_jugg.jpg", &iHeight, &iWidth);
    printf("Height:%d, Width:%d.\n", iHeight, iWidth);
    return 0;
}
