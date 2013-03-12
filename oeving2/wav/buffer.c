#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>

// Frees all resources used by the buffer
void buffer_free(buffer_t *buffer) {
    free(buffer->data);
    free(buffer);
}

// Reads a number of bytes from the buffer into an integer
// Uses little endian encoding
int buffer_next_int_le(buffer_t *buffer, int num_bytes) {
    int value = 0;
    
    int i;
    for (i = 0; i < num_bytes; i++) {
        // Read byte
        int data = buffer->data[buffer->position++];
        
        // Shift and add to value
        value = (data << 8*i) | value;
    }
    
    return value;
}

// Reads a number of bytes from the buffer into an integer
int buffer_next_int(buffer_t *buffer, int num_bytes) {
    int value = 0;
    
    int i;
    for (i = 0; i < num_bytes; i++) {
        // Read byte
        int data = buffer->data[buffer->position++];
        
        // Shift previous and add current
        value = (value << 8) | data;
    }
    
    return value;
}

// Reads a variable sized integer from the buffer
int buffer_next_varint(buffer_t *buffer) {
    int value = 0;
    
    while (1) {
        // Read byte
        int data = buffer->data[buffer->position++];
        
        // Mask for the first bit
        int mask = 1 << 7;
        
        // Shift previous and add current
        value = (value << 7) + (data & ~mask);
        
        // If MSD is 0: break
        if ((data & mask) == 0) {
            break;
        }
    }
    
    return value;
}

// Reads a file into a buffer
// Returns NULL if error
buffer_t* buffer_from_file(char *name) {
    // Vars
    FILE *file;
    buffer_t *buffer;
    
    // Open file
    file = fopen(name, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file %s\n", name);
        return NULL;
    }
    
    // Allocate memory to struct
    buffer = (buffer_t*)malloc(sizeof(buffer_t));
    buffer->position = 0;
    
    // Get file length
    fseek(file, 0, SEEK_END);
    buffer->length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate memory to data
    buffer->data = (char*)malloc(buffer->length);
    if (!buffer->data) {
        fprintf(stderr, "Memory error!\n");
        fclose(file);
        free(buffer);
        return NULL;
    }
    
    // Read file contents into buffer
    fread(buffer->data, buffer->length, 1, file);
    fclose(file);
    
    return buffer;
}
