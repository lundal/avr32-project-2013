#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

void flip_endian_word(WORD *value) {
    BYTE *bytes = (BYTE *)value;
    BYTE temp;
    temp = bytes[0];
    bytes[0] = bytes[3];
    bytes[3] = temp;
    temp = bytes[1];
    bytes[1] = bytes[2];
    bytes[2] = temp;
}

void flip_endian_half(HALF *value) {
    BYTE *bytes = (BYTE *)value;
    BYTE temp = bytes[0];
    bytes[0] = bytes[1];
    bytes[1] = temp;
}

bmp_image *bmp_load(char *filename) {
    // Open file
    FILE *file = fopen(filename, "rb");
    
    // Verify that file was opened
    if (file == NULL) {
        return NULL;
    }
    
    // Read file header
    bmp_file_header file_header;
    fread(&file_header, 1, sizeof(bmp_file_header), file);
        
    // Correct endianness
    flip_endian_half(&file_header.file_type);
    flip_endian_word(&file_header.file_size);
    flip_endian_half(&file_header.reserved1);
    flip_endian_half(&file_header.reserved2);
    flip_endian_word(&file_header.data_offset);
    
    // Verify that this is a BMP file
    if (file_header.file_type != 0x4D42) {
        fclose(file);
        return NULL;
    }
    
    // Read info header
    bmp_info_header info_header;
    fread(&info_header, 1, sizeof(bmp_info_header), file);
    
    // Correct endianness
    flip_endian_word(&info_header.header_size);
    flip_endian_word(&info_header.image_width);
    flip_endian_word(&info_header.image_height);
    flip_endian_half(&info_header.color_planes);
    flip_endian_half(&info_header.bpp);
    flip_endian_word(&info_header.compression);
    flip_endian_word(&info_header.image_size);
    flip_endian_word(&info_header.resolution_horizontal);
    flip_endian_word(&info_header.resolution_vertical);
    flip_endian_word(&info_header.colors_total);
    flip_endian_word(&info_header.colors_important);
    
    // Move to bitmap data
    fseek(file, file_header.data_offset, SEEK_SET);
    
    // NOTE: Assume 24 bpp and width multiple of 4
    
    // Allocate memory
    BYTE *data = (BYTE *)malloc(info_header.image_size);
    
    // Verify allocation
    if (data == NULL) {
        fclose(file);
        return NULL;
    }
    
    // Read bitmap data
    fread(data, 1, info_header.image_size, file);
    
    // Verify that data was read
    if (data == NULL) {
        fclose(file);
        return NULL;
    }
    
    // Close file
    fclose(file);
    
    // Create struct
    bmp_image *image = (bmp_image *)malloc(sizeof(bmp_image));
    image->width = (int)info_header.image_width;
    image->height = (int)info_header.image_height;
    image->data = data;
    
    // Return image
    return image;
}

