#include <stdio.h>
#include <stdlib.h>

#include "stego.h"
#include "bmp.h"

char char_code(char ch){
    if('A' <= ch && ch <= 'Z')
        return ch - 'A';
    else if(ch == ' ')
        return 26;
    else if(ch == '.')
        return 27;
    else if(ch == ',')
        return 28;
    else return 127;
}

char char_decode(char ch){
    if(0 <= ch && ch <= 25)
        return ch + 'A';
    else if(ch == 26)
        return ' ';
    else if(ch == 27)
        return '.';
    else if(ch == 28)
        return ',';
    else return 127;
}

int32_t insert(bmp_t *BmpFileOut, char *txt_keyFilename, char *msgFilename){
    FILE *txt_key = fopen(txt_keyFilename, "r");
    if(txt_key == NULL){
        return 1;
    }

    FILE *msg = fopen(msgFilename, "r");
    if(msg == NULL){
        return 2;
    }

    char buf;
    int32_t w = BmpFileOut->infoheader.biWidth;
    int32_t h = BmpFileOut->infoheader.biHeight;

    while(fscanf(msg, "%c", &buf) == 1){
        if(buf == '\r' || buf == '\n')
            break;
        char decode = char_code(buf);

        if(decode == 127) {
            return 3;
        }
        int x, y;
        char chanel;
        for(int i = 0; i < 5; i++){
            fscanf(txt_key, "%d %d %c\n", &x, &y, &chanel);
            int ind = x + (h - 1 - y) * w;
            if(chanel == 'R'){
                BmpFileOut->pixels[ind].r &= 0xfe;
                BmpFileOut->pixels[ind].r ^= ((decode >> i) & 1);
            }
            else if(chanel == 'G'){
                BmpFileOut->pixels[ind].g &= 0xfe;
                BmpFileOut->pixels[ind].g ^= ((decode >> i) & 1);
            }
            else if(chanel == 'B'){

                BmpFileOut->pixels[ind].b &= 0xfe;
                BmpFileOut->pixels[ind].b ^= ((decode >> i) & 1);
            }
            else {

                return 4;
            }
        }

    }
    return 0;
}

int32_t extract(bmp_t *BmpFileIn, char *txt_keyFilename, char *msgFilename){
    FILE *txt_key = fopen(txt_keyFilename, "r");
    FILE *msg = fopen(msgFilename, "w");
    if(txt_key == NULL){
        return 1;
    }
    if(msg == NULL){
        return 1;
    }
    int8_t sym_in_msg = 0;
    int8_t chanel;
    int32_t offset;
    int32_t x, y;
    int32_t w = BmpFileIn->infoheader.biWidth;
    int32_t h = BmpFileIn->infoheader.biHeight;

    while(fscanf(txt_key, "%d %d %c\n", &x, &y, &chanel) == 3){
        for(int i = 0; i < 5; i++) {
            if(chanel == 'R')
                offset = BmpFileIn->pixels[(h - y - 1) * w + x].r & 1;
            else if(chanel == 'G')
                offset = BmpFileIn->pixels[(h - y - 1) * w + x].g & 1;
            else if(chanel == 'B')
                offset = BmpFileIn->pixels[(h - y - 1) * w + x].b & 1;
            else
                return 1;
            sym_in_msg += (1 << i) * offset;
            if(i < 4)
                if(fscanf(txt_key, "%d %d %c\n", &x, &y, &chanel) != 3) {
                    fprintf( msg,  "\n");
                    return 0;
                }
        }
        fprintf( msg, "%c", char_decode(sym_in_msg));
        sym_in_msg = 0;

    }
    fprintf( msg,  "\n");
    return 0;
}

