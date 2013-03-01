#include "sound.h"

#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

// Combines samples into a track
// Arguments should be on the repeating form:
// Sample(sample_t*), Duration(float, seconds), Volume(int, %), ...
sound_t* sound_build(int16_t n_samples, ...) {
    // Allocate memory for sound
    sound_t *sound = (sound_t*)malloc(sizeof(sound_t));
    
    // Set samples
    sound->n_samples = n_samples;
    
    // Allocate memory for arrays
    sound->samples = (sample_t**)malloc(sizeof(sample_t*) * sound->n_samples);
    sound->sample_reps = (int16_t*)malloc(sizeof(int16_t) * sound->n_samples);
    sound->sample_vol = (int16_t*)malloc(sizeof(int16_t) * sound->n_samples);
    
    // Loop through arguments
    va_list args;
    va_start(args, n_samples);
    int16_t i;
    for (i = 0; i < n_samples; i++) {
        // Get next arguments
        sample_t* sample = va_arg(args, sample_t*);
        double duration = va_arg(args, double);
        int16_t volume = (int16_t)va_arg(args, int); //va_args are passed as int
        
        // Store data
        sound->samples[i] = sample;
        sound->sample_reps[i] = (int16_t)(duration * (double)SAMPLE_RATE / (double)sample->n_points);
        sound->sample_vol[i] = volume;
    }
    
    return sound;
}

// Generates a silent sample
sample_t* sample_gen_silence() {
    // Allocate memory for sample
    sample_t *sample = (sample_t*)malloc(sizeof(sample_t));
    
    // Use some sample points
    sample->n_points = 100;
    
    // Allocate memory for sample points
    sample->points = (int16_t*)malloc(sizeof(int16_t) * sample->n_points);
    
    // For every sample point
    int16_t i;
    for (i = 0; i < sample->n_points; i++) {
        // Store zero
        sample->points[i] = 0;
    }
    
    return sample;
}

// Generates a square wave with specified frequency
sample_t* sample_gen_square(int16_t freq) {
    // Allocate memory for sample
    sample_t *sample = (sample_t*)malloc(sizeof(sample_t));
    
    // Calculate sample points in one period
    sample->n_points = SAMPLE_RATE / freq;
    
    // Allocate memory for sample points
    sample->points = (int16_t*)malloc(sizeof(int16_t) * sample->n_points);
    
    // For every sample point
    int16_t i;
    for (i = 0; i < sample->n_points; i++) {
        // Calculate square value
        int16_t val = (i > sample->n_points/2) ? 1 : -1;
        
        // Store adjusted value
        sample->points[i] = (int16_t)(val * SAMPLE_AMPLITUDE);
    }
    
    return sample;
}

// Generates a sine wave with specified frequency
sample_t* sample_gen_sin(int16_t freq) {
    // Allocate memory for sample
    sample_t *sample = (sample_t*)malloc(sizeof(sample_t));
    
    // Calculate sample points in one period
    sample->n_points = SAMPLE_RATE / freq;
    
    // Allocate memory for sample points
    sample->points = (int16_t*)malloc(sizeof(int16_t) * sample->n_points);
    
    // For every sample point
    int16_t i;
    for (i = 0; i < sample->n_points; i++) {
        // Calculate progress ratio
        float r = (float)i / (float)sample->n_points;
        
        // Calculate sinus value
        float val = sinf(2.0 * M_PI * r);
        
        // Store adjusted value
        sample->points[i] = (int16_t)(val * SAMPLE_AMPLITUDE);
    }
    
    return sample;
}
