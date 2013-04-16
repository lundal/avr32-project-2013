#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

bmp_image *bmp_load(char *filename) {
    // Open file
    FILE *file = fopen(filename, "rb");
    
    // Check that file was opened
    if (file == NULL) {
        return NULL;
    }
    
    // Read file header
    bmp_file_header file_header;
    fread(&file_header, sizeof(bmp_file_header), 1, file);

    // Verify that this is a BMP file
    if (file_header.file_type != 0x4D42) {
        fclose(file);
        return NULL;
    }
    
    // Read info header
    bmp_info_header info_header;
    fread(&info_header, sizeof(bmp_info_header), 1, file);
    
    // Move to bitmap data
    fseek(file, file_header.data_offset, SEEK_SET);
    
    // NOTE: Assume 24 bpp and width multiple of 4
    
    // Allocate memory
    unsigned char *data = (unsigned char *)malloc(info_header.image_size);
    
    // Verify allocation
    if (data == NULL) {
        fclose(file);
        return NULL;
    }
    
    // Read bitmap data
    fread(data, info_header.image_size, file);
    
    // Verify that data was read
    if (data == NULL) {
        fclose(file);
        return NULL;
    }
    
    // Close file
    fclose(file);
    
    // Create struct
    bmp_image *image = (bmp_image *)malloc(sizeof(bmp_image);
    image->width = info_header.image_width;
    image->height = info_header.image_height;
    image->data = data;
    
    // Return image
    return image;
}

