#ifndef FONT_HEADER
#define FONT_HEADER

#include "bmp.h"

#define FONT (bmp_image **)
#define FONT_SET_SIZE 0x80

// Prototypes
FONT font_load(char *folder);

#endif
