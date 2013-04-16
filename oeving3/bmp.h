
#define WORD int
#define HALF short
#define BYTE char

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
    unsigned char *data;
} bmp_image;
