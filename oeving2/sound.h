#ifndef SOUND_H
#define SOUND_H

#define SAMPLE_AMPLITUDE 32000.0

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
sound_t* sound_build(int n_samples, ...);
sample_t* sample_gen_silence();
sample_t* sample_gen_square(int frequency);
sample_t* sample_gen_sin(int frequency);

#endif
