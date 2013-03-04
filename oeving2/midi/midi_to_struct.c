#include "midi_to_struct.h"

#include <stdlib.h>
#include <stdio.h>

int division;

int num_tracks;
buffer_t **tracks;
event_t **events;

int main (int argc, char *argv[]) {
    buffer_t* buffer = read_file("test.mid");
    
    parse_midi(buffer);
    
    return 0;
}

void parse_midi(buffer_t *buffer) {
    // Read data
    int id = parse_int(buffer, 4);
    int length = parse_int(buffer, 4);
    int format = parse_int(buffer, 2);
    num_tracks = parse_int(buffer, 2);
    division = parse_int(buffer, 2);
    
    printf("Midi - Format: %d, Tracks: %d, Division: %d\n", format, num_tracks, division);
    
    // Allocate memory
    tracks = (buffer_t**)malloc(sizeof(buffer_t*) * num_tracks);
    events = (event_t**)malloc(sizeof(event_t*) * num_tracks);
    
    // Read tracks to separate buffers
    int i;
    for (i = 0; i < num_tracks; i++) {
        tracks[i] = read_track(buffer);
        events[i] = next_useful_event(tracks[i]);
    }
    
    // TODO: Calculate sample_points per tick
    
    // Get first
    event_t *event = next_event();
    
    // While there are events
    while (event != NULL) {
        
        printf("Event - DT=%d, CH=%d, NOTE=%d, VOL=%d.\n", event->delta_time, event->channel, event->note, event->volume);
        
        // Get next
        event = next_event();
    }
    
    // TODO: Output to struct
    
    printf("Midi - End\n");
}

buffer_t* read_track(buffer_t *buffer) {
    // Read data
    int id = parse_int(buffer, 4);
    int length = parse_int(buffer, 4);
    
    // Create buffer
    buffer_t *track = (buffer_t*)malloc(sizeof(buffer_t));
    track->position = 0;
    track->length = length;
    track->data = (char*)malloc(track->length);
    
    // Copy data
    int i;
    for (i = 0; i < track->length; i++) {
        track->data[i] = buffer->data[buffer->position++];
    }
    
    return track;
}

event_t* next_event() {
    int time_to_next = 1000000000;
    int next_event_id = -1;
    
    int i;
    
    // Find next event
    for (i = 0; i < num_tracks; i++) {
        if (events[i] != NULL) {
            if (events[i]->delta_time < time_to_next) {
                time_to_next = events[i]->delta_time;
                next_event_id = i;
            }
        }
    }
    
    // No event found
    if (next_event_id == -1) {
        return NULL;
    }
    
    // Update delta_times
    for (i = 0; i < num_tracks; i++) {
        if (events[i] != NULL && i != next_event_id) {
            events[i]->delta_time -= time_to_next;
        }
    }
    
    // This is the next event
    event_t *next_event = events[next_event_id];
    
    // Update list with next event from track;
    events[next_event_id] = next_useful_event(tracks[next_event_id]);
    
    return next_event;
}

event_t* next_useful_event(buffer_t *buffer) {
    event_t *event = NULL;
    
    // Read events from track until non-NULL or end of track
    while (event == NULL && buffer->position < buffer->length) {
        event = read_event(buffer);
    }
    
    return event;
}

event_t* read_event(buffer_t *buffer) {
    // Read data 
    int delta_time = parse_varint(buffer);
    int first_byte = parse_int(buffer, 1);
    int type = (first_byte & 0xF0) >> 4;
    int channel = first_byte & 0x0F;
    
    event_t* event = NULL;
    
    switch (type) {
        // Note off
        case 0x8: {
            // Read data
            int note = parse_int(buffer, 1);
            int velocity = parse_int(buffer, 1);
            
            // Store event
            event = (event_t*)malloc(sizeof(event_t));
            event->delta_time = delta_time;
            event->channel = channel;
            event->note = note;
            event->volume = 0;
            
            break;
        }
        // Note on
        case 0x9: {
            // Read data
            int note = parse_int(buffer, 1);
            int velocity = parse_int(buffer, 1);
            
            // Store event
            event = (event_t*)malloc(sizeof(event_t));
            event->delta_time = delta_time;
            event->channel = channel;
            event->note = note;
            event->volume = velocity;
            
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
    
    return event;
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

