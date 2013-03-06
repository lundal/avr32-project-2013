#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdint.h>

#define SAMPLE_RATE 46875/2 // Oscillator 1 / 256
#define SAMPLE_AMPLITUDE 32000.0 // Approx the max value of an int16_t
#define WAVE_SQUARE 0
#define WAVE_SINE 1

// Structure for a sample
typedef struct {
    int16_t n_points;
    int16_t *points;
} sample_t;

// Prototypes
sample_t* sample_gen_silence();
sample_t* sample_gen(int16_t frequency, int8_t wavetype);

#endif
