#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
#include "stego.h"

int main(int argc, char **argv){


    if (strcmp(argv[1], "crop-rotate") == 0) {
        if(argc < 8) {
            return 1;
        }
        char *inputFile = argv[2];

        int32_t x = atoi(argv[4]);
        int32_t y = atoi(argv[5]);
        int32_t w = atoi(argv[6]);
        int32_t h = atoi(argv[7]);

        bmp_t bmpFile;
        if(load_bmp(inputFile, &bmpFile)){
            return 1;
        }

        bmp_t bmpFile_crop;
        if(crop(&bmpFile, x, y, w, h, &bmpFile_crop)){
            return 1;
        }

        bmp_t bmpFile_rotate;
        if(rotate(&bmpFile_crop, &bmpFile_rotate)){
            return 1;
        }

        char *outputFile = argv[3];

        if(save_bmp(&bmpFile_rotate, outputFile)){
            return 1;
        }

        free(bmpFile.pixels);
        free(bmpFile_crop.pixels);
        free(bmpFile_rotate.pixels);
    }
    else if(strcmp(argv[1], "insert") == 0){
        char *inputFile = argv[2];
        char *outFile = argv[3];
        char *txt_keyFile = argv[4];
        char *msgFile = argv[5];

        bmp_t BmpFile;
        if(load_bmp(inputFile, &BmpFile)){

            return 1;
        }
        int x = insert(&BmpFile, txt_keyFile, msgFile);
        if(x){
            return x;
        }
        if(save_bmp(&BmpFile, outFile)){

            return 3;
        }
        free(BmpFile.pixels);
    }

    else if(strcmp(argv[1], "extract") == 0){
        char *inFile = argv[2];
        char *txt_keyFile = argv[3];
        char *msgFile = argv[4];

        bmp_t BmpFile;
        if(load_bmp(inFile, &BmpFile)){

            return 1;
        }

        if(extract(&BmpFile, txt_keyFile, msgFile)){
            return 1;
        }
        free(BmpFile.pixels);

    }
    else{

        return 4;
    }





}
