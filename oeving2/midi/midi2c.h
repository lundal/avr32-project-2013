#ifndef MIDI2C_H
#define MIDI2C_H

#define NUM_CHANNELS 12

typedef struct {
    unsigned char *data;
    long length;
    long position;
} buffer_t;

typedef struct {
    int delta_time;
    int type_channel;
    buffer_t *buffer;
} track_t;

// Prototypes
int main (int argc, char *argv[]);
void parse_midi(buffer_t *buffer, char *name);
track_t* track_create(buffer_t *buffer);
void events_process();
track_t* track_next();
int channel_find(int id);
int channel_recover(int id);
double seconds_from_ticks(int ticks);
int type_from_value(int value);
int channel_from_value(int value);
void track_process_info(track_t *track);
int track_process_event(track_t *track);
int parse_int(buffer_t *buffer, int num_bytes);
int parse_varint(buffer_t *buffer);
buffer_t* read_file(char *name);

#endif
