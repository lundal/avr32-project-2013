#include "midi_to_struct.h"

#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    buffer_t* buffer = read_file("test.mid");
    
    /*
    int i;
    for (i = 0; i < buffer->length; i++) {
        printf("%02X ", (int)buffer->data[i]);
    }
    */
    
    parse_midi(buffer);
    
    return 0;
}

void parse_midi(buffer_t *buffer) {
    // Read data
    int id = parse_int(buffer, 4);
    int length = parse_int(buffer, 4);
    int format = parse_int(buffer, 2);
    int tracks = parse_int(buffer, 2);
    int division = parse_int(buffer, 2);
    
    // TODO: Set sample_points per tick
    printf("Midi - Format: %d, Tracks: %d, Division: %d\n", format, tracks, division);
    
    // Parse tracks
    int i;
    for (i = 0; i < tracks; i++) {
        parse_track(buffer);
    }
    
    printf("Midi - End\n");
}

void parse_track(buffer_t *buffer) {
    // Read data
    int id = parse_int(buffer, 4);
    int length = parse_int(buffer, 4);
    
    printf("Track - Length: %d\n", length);
    
    // Parse events
    int data_end = buffer->position + length;
    while (buffer->position < data_end) {
        parse_event(buffer); 
    }
    
    printf("Track - End\n");
}

void parse_event(buffer_t *buffer) {
    // Read data
    int delta_time = parse_varint(buffer);
    int first_byte = parse_int(buffer, 1);
    int type = (first_byte & 0xF0) >> 4;
    int channel = first_byte & 0x0F;
    
    switch (type) {
        // Note off
        case 0x8: {
            // Read data
            int note = parse_int(buffer, 1);
            int velocity = parse_int(buffer, 1);
            
            // TODO
            printf("Delta %d, Channel %d, Note %d off (%d)\n", delta_time, channel, note, velocity);
            break;
        }
        // Note on
        case 0x9: {
            // Read data
            int note = parse_int(buffer, 1);
            int velocity = parse_int(buffer, 1);
            
            // TODO
            printf("Delta %d, Channel %d, Note %d on (%d)\n", delta_time, channel, note, velocity);
            break;
        }
        // Note aftertouch
        case 0xA: {
            // Read data
            int note = parse_int(buffer, 1);
            int value = parse_int(buffer, 1);
            
            // Skip
            break;
        }
        // Controller
        case 0xB: {
            // Read data
            int number = parse_int(buffer, 1);
            int data = parse_int(buffer, 1);
            
            // Skip
            break;
        }
        // Program change
        case 0xC: {
            // Read data
            int number = parse_int(buffer, 1);
            
            // Skip
            break;
        }
        // Channel aftertouch
        case 0xD: {
            // Read data
            int value = parse_int(buffer, 1);
            
            // Skip
            break;
        }
        // Pitch bend
        case 0xE: {
            // Read data
            int low_val = parse_int(buffer, 1);
            int high_val = parse_int(buffer, 1);
            
            // Skip
            break;
        }
        // SysEx or Meta event
        case 0xF: {
            // SysEx event
            if (channel == 0x0 || channel == 0x7) {
                // Read data
                int sysex_length = parse_varint(buffer);
                
                // Skip rest
                buffer->position += sysex_length;
            }
            // Meta event
            if (channel == 0xF) {
                // Read data
                int meta_type = parse_int(buffer, 1);
                int meta_length = parse_varint(buffer);
                
                //TODO: Parse Set Tempo?
                
                // Skip rest
                buffer->position += meta_length;
            }
            break;
        }
    }
}

int parse_int(buffer_t *buffer, int num_bytes) {
    int value = 0;
    
    int i;
    for (i = 0; i < num_bytes; i++) {
        // Read byte
        int data = buffer->data[buffer->position++];
        
        // Shift previous and add current
        value = (value << 8) + data;
    }
    
    return value;
}

int parse_varint(buffer_t *buffer) {
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

