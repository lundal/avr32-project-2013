#ifndef BMP_H
#define BMP_H

#define WORD unsigned int
#define HALF unsigned short
#define BYTE unsigned char

#define WORD_S sizeof(WORD)
#define HALF_S sizeof(HALF)
#define BYTE_S sizeof(BYTE)

#define BMP_BPP 3

typedef struct __attribute__((__packed__)) {
    HALF file_type;
    WORD file_size;
    HALF reserved1;
    HALF reserved2;
    WORD data_offset;
} bmp_file_header;

typedef struct __attribute__((__packed__)) {
    WORD header_size;
    WORD image_width;
    WORD image_height;
    HALF color_planes;
    HALF bpp;
    WORD compression;
    WORD image_size;
    WORD resolution_horizontal;
    WORD resolution_vertical;
    WORD colors_total;
    WORD colors_important;
} bmp_info_header;

typedef struct {
    int width;
    int height;
    BYTE *data;
} bmp_image;

// Prototypes
bmp_image *bmp_load(char *filename);
bmp_image *bmp_copy(bmp_image *image);
void bmp_tint(bmp_image *image, char r, char g, char b);

#endif
