#include<stdio.h>
#include<stdlib.h>
#include"jpeg_simp.h"

#define inMemJpeg "inMemJpeg.jpg"
// const char *srcJpegFile[2] = { "src_mono.jpg", "src_rgb.jpg" };

#define dstJpegFile "new.jpg"
int main(int argc, char **argv) {
    /* first read in buffer */
    unsigned char * g_ImageBuffer;
    long unsigned int width;
    long unsigned int height;
    long unsigned int channels;

    unsigned char * g_ImageBufferCprs; /* compress */
    unsigned char * g_ImageBufferNew;
    long unsigned int g_nImageSizeCprs;
    int ret;

    // use exec-param to specify input srcJpegFile
    if(argc != 2) {
        printf("usage: %s <jpeg_filename>\n", argv[0]);
        return -1;
    }

    /* [1] read jpg file, decompress to mem-g_ImageBuffer */
    ret = read_JPEG_file2mem(&g_ImageBuffer, argv[1],
            &height, &width, &channels);
    if(ret < 0)
        exit(EXIT_FAILURE); // exit immediately
    printf("raw size is %ld * %ld => %ld.\n",
            height * width, channels, height * width * channels);

    /* [2] encompress jpg from mem-g_ImageBuffer to mem-g_ImageBufferCprs */
    g_nImageSizeCprs = 0;
    ret = encompress_JPEG_m2m(&g_ImageBufferCprs, g_ImageBuffer,
            height, width, channels, 70, &g_nImageSizeCprs);
    if(ret < 0)
        exit(EXIT_FAILURE); // exit immediately
    printf("compressed size is %ld.\n", g_nImageSizeCprs);

    // direclty write compress mem-buffer-data to file
    FILE * of = fopen(inMemJpeg, "w");
    if(of != NULL) {
        fwrite((const char *)g_ImageBufferCprs, g_nImageSizeCprs, 1, of);
        fclose(of);
    } else
        exit(EXIT_FAILURE); // exit immediately

    /* [3] decompress jpg from mem-g_ImageBufferCprs to mem-g_ImageBufferNew */
    ret = decompress_JPEG_m2m(&g_ImageBufferNew, g_ImageBufferCprs,
            height * width * channels,
            &height, &width, &channels);
    if(ret < 0)
        exit(EXIT_FAILURE); // exit immediately

    /* [4] write jpg file, encompress mem-g_ImageBufferNew to file */
    ret = write_JPEG_mem2file(dstJpegFile, g_ImageBufferNew,
            height, width, channels, 70);
    if(ret < 0)
        exit(EXIT_FAILURE); // exit immediately

    free(g_ImageBuffer);
    free(g_ImageBufferCprs);
    free(g_ImageBufferNew);
    return 0;
}
