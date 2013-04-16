#include "bmp.h"

bmp_image bmp_load(char *filename) {
    // Open file
    FILE *file = fopen(filename, "rb");
    
    // Check that file was opened
    if (file == NULL) {
        return NULL;
    }
    
    // Read file header
    b
}
