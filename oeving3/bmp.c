#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    
    // ***********************
    // * NOTE: Assume 24 bpp *
    // ***********************
    
    // Allocate memory
    BYTE *data = (BYTE *)malloc(info_header.image_size);
    
    // Verify allocation
    if (data == NULL) {
        fclose(file);
        return NULL;
    }
    
    // For each line
    int i;
    for (i = 0; i < info_header.image_height; i++) {
        // Get line start
        BYTE *line = &data[i * info_header.image_width * BMP_BPP];
        
        // Read a line of bitmap data
        fread(line, 1, info_header.image_width * BMP_BPP, file);
        
        // Seek to next line
        int seek_dist = info_header.image_width % 4;
        fseek(file, seek_dist, SEEK_CUR);
    }
    
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

bmp_image *bmp_copy(bmp_image *image) {
    // Calculate size
    int size = image->width * image->height * BMP_BPP;
    
    // Allocate memory
    BYTE *data = (BYTE *)malloc(size);
    
    // Verify allocation
    if (data == NULL) {
        return NULL;
    }
    
    // Copy data
    memcpy(data, image->data, size);
    
    // Create struct
    bmp_image *image_n = (bmp_image *)malloc(sizeof(bmp_image));
    image_n->width = image->width;
    image_n->height = image->height;
    image_n->data = data;
    
    // Return image
    return image_n;
}

void bmp_tint(bmp_image *image, char r, char g, char b) {
    // Calculate size
    int size = image->width * image->height * BMP_BPP;
    
    // For every pixel
    int i;
    for (i = 0; i < size; i += 3) {
        // Read
        char b_image = image->data[i + 0];
        char g_image = image->data[i + 1];
        char r_image = image->data[i + 2];
        
        // If transparent (fuchsia)
        if (r == 0xFF && g == 0x00 && b == 0xFF) {
            // Skip
            continue;
        }
        
        // Calculate color
        char b_out = (char)(((int)b * (int)b_image) / 255);
        char g_out = (char)(((int)g * (int)g_image) / 255);
        char r_out = (char)(((int)r * (int)r_image) / 255);
        
        // Write
        image->data[i + 0] = b_out;
        image->data[i + 1] = g_out;
        image->data[i + 2] = r_out;
    }
}
