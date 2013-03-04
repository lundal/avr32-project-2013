#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdint.h>

#define SAMPLE_RATE 46875/2 // Oscillator 1 / 256
#define SAMPLE_AMPLITUDE 32000.0 // Approx the max value of an int16_t

// Structure for a sample
typedef struct {
    int16_t n_points;
    int16_t *points;
} sample_t;

// Prototypes
sample_t* sample_gen_silence();
sample_t* sample_gen_square(int16_t frequency);
sample_t* sample_gen_sin(int16_t frequency);

#endif
