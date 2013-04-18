#ifndef FONT_HEADER
#define FONT_HEADER

#include "bmp.h"

#define FONT bmp_image**
#define FONT_SET_SIZE 0x80

// Prototypes
FONT font_load(char *folder);
FONT font_copy(FONT font);
void font_tint(FONT font, char r, char g, char b);

#endif
