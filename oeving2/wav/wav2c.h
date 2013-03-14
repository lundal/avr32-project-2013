#ifndef WAV2C_H
#define WAV2C_H

typedef struct {
    int *sample_points;
    int num_points;
    int sample_rate;
    int bytes_per_point;
} wave_t;

// Prototypes
int main (int argc, char *argv[]);
void print_wave_as_byte_array(wave_t *wav, char *name, int target_rate);
wave_t* next_wave(buffer_t *buffer);

#endif
