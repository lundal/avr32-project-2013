#include "midi2c.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int tpb;
int bpm;

int num_tracks;
int *track_event_type;
buffer_t **tracks;
event_t **events;

int num_channels;
int *channels;

int main (int argc, char *argv[]) {
    if (argc > 1) {
        buffer_t* buffer = read_file(argv[1]);
        parse_midi(buffer, argv[1]);
    }
    else {
        printf("Usage: midi2c <midi file> [ > <file>]\n");
    }
    return 0;
}

void parse_midi(buffer_t *buffer, char *name) {
    // Read data
    int id = parse_int(buffer, 4);
    int length = parse_int(buffer, 4);
    int format = parse_int(buffer, 2);
    num_tracks = parse_int(buffer, 2);
    tpb = parse_int(buffer, 2); // Assume ticks per second (not frames)
    bpm = 120; // Default value
    
    // Allocate memory
    tracks = (buffer_t**)malloc(sizeof(buffer_t*) * num_tracks);
    events = (event_t**)malloc(sizeof(event_t*) * num_tracks);
    
    //num_tracks = 1;
    track_event_type = (int*)malloc(sizeof(int) * num_tracks);
    
    // Read tracks to separate buffers
    int i;
    for (i = 0; i < num_tracks; i++) {
        tracks[i] = read_track(buffer);
        events[i] = next_useful_event(i);
        //printf("Ne
    }
    
    printf("const midi_soundtrack_t %s = {\n", name);
    printf("    .events = {\n");
    
    // To keep track of number of events
    int num_events = 0;
    
    // Vars to keep track of available channels
    num_channels = 12; // Same as in player!
    channels = (int*)malloc(sizeof(int) * num_channels);
    for (i = 0; i < num_channels; i++) {
        channels[i] = -1;
    }
    
    // Get first
    event_t *event = next_event();
    
    // While there are events
    while (event != NULL) {
        double tps = (double)tpb * (double)bpm / 60.0;
        double seconds = (double)event->delta_time / (double)tps;
        
        // Determine correct channel
        int channel = -1;
        if (event->volume > 0) {
            // Find available channel
            for (i = 0; i < num_channels; i++) {
                if (channels[i] == -1) {
                    // Store what the channel is used for
                    channels[i] = (event->channel << 8) + event->note;
                    
                    // Return channel
                    channel = i;
                    break;
                }
            }
        }
        else {
            // Recover channel
            for (i = 0; i < num_channels; i++) {
                int id = (event->channel << 8) + event->note;
                if (channels[i] == id) {
                    // Set available
                    channels[i] = -1;
                    
                    // Return channel
                    channel = i;
                    break;
                }
            }
        }
        
        // Check for channel overflow
        if (channel == -1) {
            fprintf(stderr, "Not enough channels\n");
            return;
        }
        
        printf("        {%e * SAMPLE_RATE, %d, %d, %d},\n", seconds, channel, event->note, event->volume);
        
        // Count events
        num_events++;
        
        // TODO: Cut unnecessary events?
        
        // Get next
        event = next_event();
    }
    
    printf("    },\n");
    printf("    .num_events = %d\n", num_events);
    printf("};\n");
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
    //printf("Track: %02X \n", id);
    // Copy data
    int i;
    for (i = 0; i < track->length; i++) {
        track->data[i] = buffer->data[buffer->position++];
    }
    /*printf("\n");
    printf("Events:\n");
    while (track->position < track->length) {
        printf("Bytes: ");
        for (i = 0; i < 4; i++) {
            printf("%03d(%02X) ", track->data[track->position + i], track->data[track->position + i]);
        }
        printf("\n");
        read_event(1);
    };
    printf("End\n");
    track->position = 0;
    */
    return track;
}

event_t* next_event() {
    int time_to_next = 1000000000;
    int next_event_track = -1;
    
    int i;
    // Find next event
    for (i = 0; i < num_tracks; i++) {
        if (events[i] != NULL) {
            if (events[i]->delta_time < time_to_next) {
                time_to_next = events[i]->delta_time;
                next_event_track = i;
            }
            if (events[i]->delta_time == time_to_next && events[i]->volume == 0) {
                time_to_next = events[i]->delta_time;
                next_event_track = i;
            }
        }
    }
    
    // No event found
    if (next_event_track == -1) {
        return NULL;
    }
    
    // Update delta_times
    for (i = 0; i < num_tracks; i++) {
        if (events[i] != NULL && i != next_event_track) {
            events[i]->delta_time -= time_to_next;
        }
    }
    
    // This is the next event
    event_t *next_event = events[next_event_track];
    
    // Update list with next event from track;
    events[next_event_track] = next_useful_event(next_event_track);
    
    return next_event;
}

event_t* next_useful_event(int track_id) {
    event_t *event = NULL;
    
    // Read events from track until non-NULL or end of track
    while (event == NULL && tracks[track_id]->position < tracks[track_id]->length) {
        event = read_event(track_id);
    }
    
    return event;
}

event_t* read_event(int track_id) {
    buffer_t *buffer = tracks[track_id];
    // Read data
    int delta_time = parse_varint(buffer);
    int first_byte = parse_int(buffer, 1);
    //printf("Event\n");
    // Check if same type
    if ((first_byte & (1 << 7)) == 0) {
        // Move buffer back one byte
        buffer->position -= 1;
    }
    else {
        // Update event type
        track_event_type[track_id] = first_byte;
    }
    
    int event_type = track_event_type[track_id];
    
    int type = (event_type & 0xF0) >> 4;
    int channel = event_type & 0x0F;
    
    event_t* event = NULL;
    
    switch (type) {
        // Note off
        case 0x8: {
            // Read  data
            int note = parse_int(buffer, 1);
            int velocity = parse_int(buffer, 1);
            
            // Store event
            event = (event_t*)malloc(sizeof(event_t));
            event->delta_time = delta_time;
            event->channel = channel;
            event->note = note;
            event->volume = 0;
            //printf("Note off: DT=%d, CH=%01X, NOTE=%02X, VEL=%02X\n", delta_time, channel, note, velocity);
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
            //printf("Note on: DT=%d, CH=%d, NOTE=%d, VEL=%d\n", delta_time, channel, note, velocity);
            
            break;
        }
        // Note aftertouch
        case 0xA: {
            // Read data
            int note = parse_int(buffer, 1);
            int value = parse_int(buffer, 1);
            //printf("Note aftertouch\n");
            
            // Skip
            break;
        }
        // Controller
        case 0xB: {
            // Read data
            int number = parse_int(buffer, 1);
            int data = parse_int(buffer, 1);
            //printf("Controller\n");
            
            // Skip
            break;
        }
        // Program change
        case 0xC: {
            // Read data
            int number = parse_int(buffer, 1);
            //printf("Program change\n");
            
            // Skip
            break;
        }
        // Channel aftertouch
        case 0xD: {
            // Read data
            int value = parse_int(buffer, 1);
            //printf("Channel aftertouch\n");
            
            // Skip
            break;
        }
        // Pitch bend
        case 0xE: {
            // Read data
            int low_val = parse_int(buffer, 1);
            int high_val = parse_int(buffer, 1);
            //printf("Pitch bend\n");
            
            // Skip
            break;
        }
        // SysEx or Meta event
        case 0xF: {
            // SysEx event
            if (channel == 0x0 || channel == 0x7) {
                // Read data
                int sysex_length = parse_varint(buffer);
                //printf("SysEx\n");
                
                // Skip rest
                buffer->position += sysex_length;
            }
            // Meta event
            if (channel == 0xF) {
                // Read data
                int meta_type = parse_int(buffer, 1);
                int meta_length = parse_varint(buffer);
                //printf("Meta\n");
                
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

