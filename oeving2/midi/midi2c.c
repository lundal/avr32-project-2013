#include "midi2c.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Time vars
int tpb;
int bpm = 120;

// Track and event vars
int num_events = 0;
int num_tracks;
track_t **tracks;

// Channel vars
int num_channels = 12;
int *channels;

int main (int argc, char *argv[]) {
    if (argc > 1) {
        char *name = argv[1];
        char *filename = strcat(strdup(name), ".mid");
        buffer_t* buffer = read_file(filename);
        parse_midi(buffer, name);
    }
    else {
        printf("Usage: midi2c <midi file name without extension> [ > <file>]\n");
    }
    return 0;
}

// Parses a midi file stored in a buffer and outputs it to stdout
// name is the soundtrack variable name in the output
void parse_midi(buffer_t *buffer, char *name) {
    // Read data
    int id = parse_int(buffer, 4);
    int length = parse_int(buffer, 4);
    int format = parse_int(buffer, 2);
    num_tracks = parse_int(buffer, 2);
    tpb = parse_int(buffer, 2); // Assume ticks per second (never seen frames used anyway)
    
    // Vars to remember available channels
    channels = (int*)malloc(sizeof(int) * num_channels);
    int i;
    for (i = 0; i < num_channels; i++) {
        channels[i] = -1;
    }
    
    // Allocate memory to tracks
    tracks = (track_t**)malloc(sizeof(track_t*) * num_tracks);
    
    // Create tracks from buffer
    for (i = 0; i < num_tracks; i++) {
        tracks[i] = track_create(buffer);
    }
    
    // Print struct start
    printf("const midi_soundtrack_t %s = {\n", name);
    printf(".events = {\n");
    
    // Process all events
    events_process();
    
    // Print count and struct end
    printf("},\n");
    printf(".num_events = %d\n", num_events);
    printf("};\n");
}

// Creates a track by reading from specified buffer
track_t* track_create(buffer_t *buffer) {
    // Read data
    int id = parse_int(buffer, 4);
    int length = parse_int(buffer, 4);
    
    // Create track
    track_t *track = (track_t*)malloc(sizeof(track_t));
    track->type_channel = 0;
    
    // Create buffer
    track->buffer = (buffer_t*)malloc(sizeof(buffer_t));
    track->buffer->position = 0;
    track->buffer->length = length;
    track->buffer->data = (char*)malloc(track->buffer->length);
    
    // Copy to buffer
    int i;
    for (i = 0; i < track->buffer->length; i++) {
        track->buffer->data[i] = buffer->data[buffer->position++];
    }
    
    // Process first event info
    track_process_info(track);
    
    return track;
}

// Processes all events from all tracks in chronological order
void events_process() {
    while (1) {
        // Get track with soonest event
        track_t *track = track_next();
        
        // If no tracks with events
        if (track == NULL) {
            // Done!
            return;
        }
        
        // Process the event (and get result)
        int res = track_process_event(track);
        
        // If something went wrong
        if (res == -1) {
            // Stop
            fprintf(stderr, "Stopping!\n");
            //TODO: Delete file?
            return;
        }
        
        // Get time
        int delta_time = track->delta_time;
        
        // If event consumed time
        if (res == 1) {
            // Decrease delta time of other events by it
            int i;
            for (i = 0; i < num_tracks; i++) {
                tracks[i]->delta_time -= delta_time;
            }
        }
        
        // Advance used track
        track_process_info(track);
        
        // If event didn't consume time
        if (res == 0) {
            // Increase delta time of next event in the track by it
            track->delta_time += delta_time;
        }
    }
}

// Gets the track with the soonest event
track_t* track_next() {
    // Set time to a rediculously large number
    int time_to_next = 1000000000;
    track_t *next_track = NULL;
    
    // Find track with least delta_time
    int i;
    for (i = 0; i < num_tracks; i++) {
        // Simplify access to vars
        track_t *track = tracks[i];
        buffer_t *buffer = track->buffer;
        
        // If track buffer har more data
        if (buffer->position < buffer->length ) {
            // If the event on this track comes sooner (but prioritize note off events if the time is the same)
            if ( (track->delta_time < time_to_next) || ( (track->delta_time == time_to_next) && type_from_value(track->type_channel) == NOTE_OFF) ) {
                // Set this track as next
                time_to_next = track->delta_time;
                next_track = track;
            }
        }
    }
    
    return next_track;
}

// Finds an available channel and mark it as used
// Returns -1 if a channel is not found
int channel_find(int id) {
    int i;
    for (i = 0; i < num_channels; i++) {
        if (channels[i] == -1) {
            // Store what the channel is used for
            channels[i] = id;
            
            // Return channel
            return i;
        }
    }
    return -1;
}

// Finds the channel used by id and mark it as unused
// Returns -1 if the channel is not found
int channel_recover(int id) {
    int i;
    for (i = 0; i < num_channels; i++) {
        if (channels[i] == id) {
            // Set available
            channels[i] = -1;
            
            // Return channel
            return i;
        }
    }
    return -1;
}

// Calculates seconds from ticks
double seconds_from_ticks(int ticks) {
    double tps = (double)tpb * (double)bpm / 60.0;
    double seconds = (double)ticks / (double)tps;
    return seconds;
}

// Get the type value from a type_channel byte (first 4 bits)
int type_from_value(int value) {
    return (value & 0xF0) >> 4;
}

// Get the channel value from a type_channel byte (last 4 bits)
int channel_from_value(int value) {
    return value & 0x0F;
}

// Updates the track with delta_time and type_channel for next event
void track_process_info(track_t *track) {
    // Update delta time
    track->delta_time = parse_varint(track->buffer);
    
    // Read type and channel
    int type_channel = parse_int(track->buffer, 1);
    
    // If repeated event
    if (type_channel < 0x80) {
        // Backtrack buffer (read value is a parameter for next event)
        track->buffer->position--;
    }
    else {
        // Update type and channel
        track->type_channel = type_channel;
    }
}

// Processes the next event from a track
// Returns 1 if the event consumes the delta_time, 0 if it does not and -1 if error
int track_process_event(track_t *track) {
    // Get event type and channel
    int type = type_from_value(track->type_channel);
    int channel = channel_from_value(track->type_channel);
    
    // Simplify access to vars
    buffer_t *buffer = track->buffer;
    
    // Determine event
    switch (type) {
        case NOTE_OFF: {
            // Read  data
            int note = parse_int(buffer, 1);
            int velocity = parse_int(buffer, 1);
            
            // Recover channel
            int id = (channel << 8) + note;
            int channel = channel_recover(id);
            
            // Check for errors
            if (channel == -1) {
                fprintf(stderr, "Note off event for note that is not on!\n");
                return -1;
            }
            
            // Add event
            printf("{%e * SAMPLE_RATE, %d, %d, %d},\n", seconds_from_ticks(track->delta_time), channel, note, 0);
            num_events++;
            
            return 1;
        }
        case NOTE_ON: {
            // Read data
            int note = parse_int(buffer, 1);
            int velocity = parse_int(buffer, 1);
            
            // Find available channel
            int id = (channel << 8) + note;
            int channel = channel_find(id);
            
            // Check for errors
            if (channel == -1) {
                fprintf(stderr, "Channel overflow!\n");
                return -1;
            }
            
            // Add event
            printf("{%e * SAMPLE_RATE, %d, %d, %d},\n", seconds_from_ticks(track->delta_time), channel, note, velocity);
            num_events++;
            
            return 1;
        }
        case NOTE_AFTERTOUCH: {
            // Read data
            int note = parse_int(buffer, 1);
            int value = parse_int(buffer, 1);
            
            printf("// Note aftertouch\n");
            
            // Skip
            return 0;
        }
        case CONTROLLER: {
            // Read data
            int number = parse_int(buffer, 1);
            int data = parse_int(buffer, 1);
            
            printf("//Controller\n");
            
            // Skip
            return 0;
        }
        case PROGRAM_CHANGE: {
            // Read data
            int number = parse_int(buffer, 1);
            
            printf("//Program change\n");
            
            // Skip
            return 0;
        }
        case CHANNEL_AFTERTOUCH: {
            // Read data
            int value = parse_int(buffer, 1);
            
            printf("//Channel aftertouch\n");
            
            // Skip
            return 0;
        }
        case PITCH_BEND: {
            // Read data
            int low_val = parse_int(buffer, 1);
            int high_val = parse_int(buffer, 1);
            
            printf("//Pitch bend\n");
            
            // Skip
            return 0;
        }
        case SYSEX_OR_META: {
            // SysEx event
            if (channel == SYSEX_BEGIN || channel == SYSEX_CONTINUE) {
                // Read data
                int sysex_length = parse_varint(buffer);
            
                printf("//SysEx\n");
                
                // Skip rest
                buffer->position += sysex_length;
                
                return 0;
            }
            
            // Meta event
            if (channel == META) {
                // Read data
                int meta_type = parse_int(buffer, 1);
                int meta_length = parse_varint(buffer);
                
                // Determine type
                switch (meta_type) {
                    case META_SEQUENCE_NUMBER:
                    case META_TEXT_EVENT:
                    case META_COPYRIGHT_NOTICE:
                    case META_TRACK_NAME:
                    case META_INSTRUMENT_NAME:
                    case META_LYRICS:
                    case META_MARKER:
                    case META_CUE_POINT:
                    case META_CHANNEL_PREFIX:
                    case META_END_OF_TRACK:
                    case META_SMPTE_OFFSET:
                    case META_TIME_SIGNATURE:
                    case META_KEY_SIGNATURE:
                    case META_SEQUENCER_SPESIFIC: {
                        // Skip
                        buffer->position += meta_length;
                        
                        printf("//Meta\n");
                        
                        return 0;
                    }
                    case META_SET_TEMPO: {
                        // Microseconds per quarter note
                        int mspqn = parse_int(buffer, 3);
                        
                        // Microseconds per minute
                        int mspm = 60000000;
                        
                        // Calculate bpm
                        bpm = mspm / mspqn;
                        
                        printf("//Set tempo\n");
                        
                        return 0;
                    }
                }
            }
        }
    }
    
    // No event was matched
    fprintf(stderr, "Unknown event! (TYPE=%01X, CH=%01X)\n", type, channel);
    
    return -1;
}

// Reads a number of bytes from the buffer into an integer
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

// Reads a variable sized integer from the buffer
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

// Reads a file into a buffer
// Return NULL if error
buffer_t* read_file(char *name) {
    // Vars
    FILE *file;
    buffer_t *buffer;
    
    // Open file
    file = fopen(name, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file %s", name);
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

