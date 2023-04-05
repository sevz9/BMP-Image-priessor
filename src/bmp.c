#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bmp.h"


int32_t load_bmp(char *filename, bmp_t *bmpFile){
    FILE *file = fopen(filename, "rb");

    if(file == NULL){
        return 1;
    }

    bmp_fileheader_t fileheader;
    bmp_infoheader_t infoheader;

    if(fread(&fileheader, sizeof(bmp_fileheader_t), 1, file) != 1){
        return 1;
    }

    if(fread(&infoheader, sizeof(bmp_infoheader_t), 1, file) != 1){
        return 1;
    }

    if(fileheader.bfType != 0x4D42){
        return 1;
    }

    if(infoheader.biBitCount != 24){
        return 1;
    }

    if(infoheader.biHeight <= 0 || infoheader.biWidth <= 0){
        return 1;
    }

    if(infoheader.biCompression != 0){
        return 1;
    }


    bmpFile->fileheader = fileheader;
    bmpFile->infoheader = infoheader;

    pixel_t *pixels = malloc(sizeof(pixel_t) * infoheader.biHeight * infoheader.biWidth);

    if(pixels == NULL){
        return 1;
    }

    int32_t pad = (4 - ( ( infoheader.biWidth * 3 ) % 4 ) ) % 4;

    for (int32_t i = 0; i < infoheader.biHeight; ++i) {
        for (int j = 0; j < infoheader.biWidth; ++j) {
            if(fread(&(pixels[i * infoheader.biWidth + j]), sizeof(pixel_t), 1, file) != 1){
                return 1;
            }
        }
        fseek(file, pad, SEEK_CUR);
    }

    bmpFile->pixels = pixels;

    return 0;
}



int32_t crop(bmp_t *bmpFile, int32_t x, int32_t y, int32_t w, int32_t h, bmp_t *bmpFile_crop){
    if(0 > x || x + w > bmpFile->infoheader.biWidth || w <= 0 || 0 > y || y + h > bmpFile->infoheader.biHeight || h <= 0){
        return 1;
    }

    bmpFile_crop->fileheader = bmpFile->fileheader;
    bmpFile_crop->infoheader = bmpFile->infoheader;

    bmpFile_crop->infoheader.biWidth = w;
    bmpFile_crop->infoheader.biHeight = h;

    int32_t pad = (4 - ( ( bmpFile_crop->infoheader.biWidth * 3 ) % 4) ) % 4;

    bmpFile_crop->infoheader.biSizeImage = h * (w * (bmpFile_crop->infoheader.biBitCount / 8) + pad);
    bmpFile_crop->fileheader.bfSize = bmpFile_crop->infoheader.biSizeImage + bmpFile_crop->fileheader.bfOffBits;




    pixel_t *pixels = malloc(sizeof(pixel_t) * bmpFile_crop->infoheader.biHeight * bmpFile_crop->infoheader.biWidth);
    if(pixels == NULL){
        return 1;
    }

    for (int i = 0; i < bmpFile_crop->infoheader.biHeight; ++i) {
        for (int j = 0; j < bmpFile_crop->infoheader.biWidth; ++j) {
            pixels[i * bmpFile_crop->infoheader.biWidth + j] =
                    bmpFile->pixels[(i + bmpFile->infoheader.biHeight - y - h) * bmpFile->infoheader.biWidth + (j + x)];

        }
    }

    bmpFile_crop->pixels = pixels;
    return 0;
}

int32_t rotate(bmp_t *bmpFile, bmp_t *bmpFile_rotate){
    bmpFile_rotate->fileheader = bmpFile->fileheader;
    bmpFile_rotate->infoheader = bmpFile->infoheader;

    bmpFile_rotate->infoheader.biWidth = bmpFile->infoheader.biHeight;
    bmpFile_rotate->infoheader.biHeight = bmpFile->infoheader.biWidth;

    int32_t pad = (4 - ( ( bmpFile_rotate->infoheader.biWidth * 3 ) % 4) ) % 4;

    bmpFile_rotate->infoheader.biSizeImage = bmpFile_rotate->infoheader.biHeight * (bmpFile_rotate->infoheader.biWidth * (bmpFile_rotate->infoheader.biBitCount / 8) + pad);
    bmpFile_rotate->fileheader.bfSize = bmpFile_rotate->infoheader.biSizeImage + bmpFile_rotate->fileheader.bfOffBits;

    pixel_t *pixels = malloc(sizeof(pixel_t) * bmpFile->infoheader.biHeight * bmpFile->infoheader.biWidth);
    if(pixels == NULL){
        return 1;
    }

    for (int i = 0; i < bmpFile_rotate->infoheader.biHeight; ++i) {
        for (int j = 0; j < bmpFile_rotate->infoheader.biWidth; ++j) {
            pixels[i * bmpFile_rotate->infoheader.biWidth + j] =
                    bmpFile->pixels[j * bmpFile->infoheader.biWidth + (bmpFile->infoheader.biWidth - 1 - i)];

        }
    }
    bmpFile_rotate->pixels = pixels;

    return 0;
}

int32_t save_bmp(bmp_t *bmpFile, char * outFile){
    FILE *file = fopen(outFile, "wb");
    if(file == NULL){
        return 1;
    }


    int32_t pad = (4 - ( ( bmpFile->infoheader.biWidth * 3 ) % 4) ) % 4;

    int8_t zero_byte = 0x00;

    if(fwrite(&(bmpFile->fileheader), sizeof(bmp_fileheader_t), 1, file) != 1){
        return 1;
    }

    if(fwrite(&(bmpFile->infoheader), sizeof(bmp_infoheader_t), 1, file) != 1){
        return 1;
    }

    for (int32_t i = 0; i < bmpFile->infoheader.biHeight; ++i) {
        for (int32_t j = 0; j < bmpFile->infoheader.biWidth; ++j) {
            if(fwrite(&(bmpFile->pixels[i * bmpFile->infoheader.biWidth + j]), sizeof(pixel_t), 1, file) != 1){
                return 1;
            }
        }
        for (int32_t j = 0; j < pad; ++j) {
            if(fwrite(&zero_byte, 1, 1, file) != 1){
                return 1;
            }
        }

    }
    return 0;
}
