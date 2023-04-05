#pragma once

#include "bmp.h"

int32_t insert(bmp_t *BmtFileOut, char *txt_keyFilename, char *msgFilename);

int32_t extract(bmp_t *BmtFileIn, char *txt_keyFilename, char *msgFilename);
