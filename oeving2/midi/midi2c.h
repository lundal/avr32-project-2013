#ifndef MIDI2C_H
#define MIDI2C_H

// Event types
#define NOTE_OFF 0x8
#define NOTE_ON 0x9
#define NOTE_AFTERTOUCH 0xA
#define CONTROLLER 0xB
#define PROGRAM_CHANGE 0xC
#define CHANNEL_AFTERTOUCH 0xD
#define PITCH_BEND 0xE
#define SYSEX_OR_META 0xF

// SysEx types
#define SYSEX_BEGIN 0x0
#define SYSEX_CONTINUE 0x7

// Meta types
#define META 0xF
#define META_SEQUENCE_NUMBER 0x00
#define META_TEXT_EVENT 0x01
#define META_COPYRIGHT_NOTICE 0x02
#define META_TRACK_NAME 0x03
#define META_INSTRUMENT_NAME 0x04
#define META_LYRICS 0x05
#define META_MARKER 0x06
#define META_CUE_POINT 0x07
#define META_CHANNEL_PREFIX 0x20
#define META_END_OF_TRACK 0x2F
#define META_SET_TEMPO 0x51
#define META_SMPTE_OFFSET 0x54
#define META_TIME_SIGNATURE 0x58
#define META_KEY_SIGNATURE 0x59
#define META_SEQUENCER_SPESIFIC 0x7F

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
int channel_find(int id, int vol);
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
