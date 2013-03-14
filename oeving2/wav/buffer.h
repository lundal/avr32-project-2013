#ifndef BUFFER_H
#define BUFFER_H

typedef struct {
    unsigned char *data;
    long length;
    long position;
} buffer_t;

// Prototypes
void buffer_free(buffer_t *buffer);
int buffer_next_int_le(buffer_t *buffer, int num_bytes);
int buffer_next_int(buffer_t *buffer, int num_bytes);
int buffer_next_varint(buffer_t *buffer);
buffer_t* buffer_from_file(char *name);

#endif
