#ifndef SOUND_H
#define SOUND_H

// Structure for a sample
typedef struct {
    int n_points;
    short *points;
} sample_t;

// Structure for an array of sample pointers, repetitions and volumes
typedef struct {
    int n_samples;
    sample_t **samples;
    int *sample_reps;
    int *sample_vol;
} sound_t;

// Set samplerate (updated when initAudio is called in oeving2.c)
int sample_rate;

// Prototypes
sound_t* build_sound(int n_samples, ...);
sample_t* generate_square_sample(int frequency);
sample_t* generate_sin_sample(int frequency);
sample_t* generate_silent_sample();

#endif
