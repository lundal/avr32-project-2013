#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

void reverse(void *array, int length) {
    BYTE *data = (BYTE *)array;
    int i;
    int temp;
    for (i = 0; i < length/2; i++) {
        temp = data[i];
        data[i] = data[length-i-1];
        data[length-i-1] = temp;
    }
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
    reverse(&file_header.file_type, HALF_S);
    reverse(&file_header.file_size, WORD_S);
    reverse(&file_header.reserved1, HALF_S);
    reverse(&file_header.reserved2, HALF_S);
    reverse(&file_header.data_offset, WORD_S);
    
    // Verify that this is a BMP file
    if (file_header.file_type != 0x4D42) {
        fclose(file);
        return NULL;
    }
    
    // Read info header
    bmp_info_header info_header;
    fread(&info_header, 1, sizeof(bmp_info_header), file);
    
    // Correct endianness
    reverse(&info_header.header_size, WORD_S);
    reverse(&info_header.image_width, WORD_S);
    reverse(&info_header.image_height, WORD_S);
    reverse(&info_header.color_planes, HALF_S);
    reverse(&info_header.bpp, HALF_S);
    reverse(&info_header.compression, WORD_S);
    reverse(&info_header.image_size, WORD_S);
    reverse(&info_header.resolution_horizontal, WORD_S);
    reverse(&info_header.resolution_vertical, WORD_S);
    reverse(&info_header.colors_total, WORD_S);
    reverse(&info_header.colors_important, WORD_S);
    
    // Move to bitmap data
    fseek(file, file_header.data_offset, SEEK_SET);
    
    // ***********************************************
    // * NOTE: Assume 24 bpp and width multiple of 4 *
    // ***********************************************
    
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

