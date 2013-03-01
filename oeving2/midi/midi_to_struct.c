#include "midi_to_struct.h"

#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    buffer_t* buffer = read_file("test.mid");
    
    int i;
    for (i = 0; i < buffer->length; i++) {
        printf("%02X ", (int)buffer->data[i]);
    }
    
    return 0;
}

int parse_int(buffer_t *buffer) {
    int value = 0;
    while (1) {
        // Read byte
        char data = buffer->data[buffer->position++];
        
        // Mask for the first bit
        char mask = 1 << 7;
        
        // Shift previous and add current
        value = (value << 7) + (data & ~mask);
        
        // If MSD is 0: break
        if (data & mask) {
            break;
        }
    }
    
    return value;
}

buffer_t* read_file(char *name) {
    // Vars
    FILE *file;
    buffer_t *buffer;
    
    // Open file
    file = fopen(name, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file %S", name);
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
        fprintf(stderr, "Memory error!");
        fclose(file);
        free(buffer);
        return NULL;
    }
    
    // Read file contents into buffer
    fread(buffer->data, buffer->length, 1, file);
    fclose(file);
    
    return buffer;
}

