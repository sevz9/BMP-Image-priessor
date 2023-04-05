#pragma once

#include <stdio.h>
#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} bmp_fileheader_t;

typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} bmp_infoheader_t;

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} pixel_t;

typedef struct {
    bmp_fileheader_t fileheader;
    bmp_infoheader_t infoheader;
    pixel_t *pixels;
} bmp_t;

#pragma pack(pop)

int32_t load_bmp(char *filename, bmp_t *bmpFile);

int32_t crop(bmp_t *bmpFile, int32_t x, int32_t y, int32_t w, int32_t h, bmp_t *bmpFile_crop);

int32_t rotate(bmp_t *bmpFile, bmp_t *bmpFile_rotate);

int32_t save_bmp(bmp_t *bmpFile, char * outFile);

