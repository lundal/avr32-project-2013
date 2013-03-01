#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    return 0;
}

buffer_t* read_file(char *name) {
    // Vars
    FILE *file;
    unsigned long fileLen;
    buffer_t *buffer;
    
    // Open file
    file = fopen(name, "rb");
    if (!file) {
        fprintf(stderr, "Unable to opin file %S", name);
        return NULL;
    }
    
    // Get file length
    fseek(file, 0, SEEK_END);
    fileLen = ftell(tile);
    fseek(file, 0, SEEK_SET);
    
    // Allocate memory to struct
    buffer = (buffer_t*)malloc(sizeof(buffer_t));
    
    // Allocate memory to data
    buffer->data = (unsigned char*)malloc(fileLen);
    if (!buffer->data) {
        fprintf(stderr, "Memory error!");
        fclose(file);
        free(buffer);
        return NULL;
    }
    
    // Read file contents into buffer
    fread(buffer->data, fileLen, 1, file);
    fclose(file);
    
    return buffer;
}

