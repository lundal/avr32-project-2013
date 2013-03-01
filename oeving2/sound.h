#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

#define SAMPLE_RATE 46875 // Oscillator 1 / 256
#define SAMPLE_AMPLITUDE 32000.0 // Approx the max value of an int16_t

// Structure for a sample
typedef struct {
    int16_t n_points;
    int16_t *points;
} sample_t;

// Structure for an array of sample pointers, repetitions and volumes
typedef struct {
    int16_t n_samples;
    sample_t **samples;
    int16_t *sample_reps;
    int16_t *sample_vol;
    int16_t *sample_pitch;
} sound_t;

// Set samplerate (updated when initAudio is called in oeving2.c)
int sample_rate;

// Prototypes
sound_t* sound_build(int16_t n_samples, ...);
sample_t* sample_gen_silence();
sample_t* sample_gen_square(int16_t frequency);
sample_t* sample_gen_sin(int16_t frequency);

#endif
