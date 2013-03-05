
typedef struct {
    int delta_time;
    int channel;
    int note;
    int volume;
} event_t;


typedef struct {
    unsigned char *data;
    long length;
    long position;
} buffer_t;

// Prototypes
int main (int argc, char *argv[]);
void parse_midi(buffer_t *buffer, char *name);
buffer_t* read_track(buffer_t *buffer);
event_t* next_event();
event_t* next_useful_event(int track_id);
event_t* read_event(int track_id);
int parse_int(buffer_t *buffer, int num_bytes);
int parse_varint(buffer_t *buffer);
buffer_t* read_file(char *name);
