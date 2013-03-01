#include "sound.h"
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

// Combines samples into a track
// Arguments should be on the repeating form:
// Sample(sample_t*), Duration(float, seconds), Volume(int, %), ...
sound_t* build_sound(int n_samples, ...) {
    // Allocate memory for sound
    sound_t *sound = (sound_t*)malloc(sizeof(sound_t));
    
    // Set samples
    sound->n_samples = n_samples;
    
    // Allocate memory for arrays
    sound->samples = (sample_t**)malloc(sizeof(sample_t*) * sound->n_samples);
    sound->sample_reps = (int*)malloc(sizeof(int) * sound->n_samples);
    sound->sample_vol = (int*)malloc(sizeof(int) * sound->n_samples);
    
    // Loop through arguments
    va_list args;
    va_start(args, n_samples);
    int i;
    for (i = 0; i < n_samples; i++) {
        // Get next arguments
        sample_t* sample = va_arg(args, sample_t*);
        double duration = va_arg(args, double);
        int volume = va_arg(args, int);
        
        // Store data
        sound->samples[i] = sample;
        sound->sample_reps[i] = (int)(duration * (double)sample_rate / (double)sample->n_points);
        sound->sample_vol[i] = volume;
    }
    
    return sound;
}

// Generates a silent sample
sample_t* generate_silent_sample() {
    // Allocate memory for sample
    sample_t *sample = (sample_t*)malloc(sizeof(sample_t));
    
    // Use 50 sample points
    sample->n_points = 50;
    
    // Allocate memory for sample points
    sample->points = (short*)malloc(sizeof(short) * sample->n_points);
    
    // For every sample point
    int i;
    for (i = 0; i < sample->n_points; i++) {
        // Store zero
        sample->points[i] = (short)0;
    }
    
    return sample;
}

// Generates a square wave with specified frequency
sample_t* generate_square_sample(int freq) {
    // Allocate memory for sample
    sample_t *sample = (sample_t*)malloc(sizeof(sample_t));
    
    // Calculate sample points in one period
    sample->n_points = sample_rate / freq;
    
    // Allocate memory for sample points
    sample->points = (short*)malloc(sizeof(short) * sample->n_points);
    
    // For every sample point
    int i;
    for (i = 0; i < sample->n_points; i++) {
        // Calculate square value
        int val = (i > sample->n_points/2) ? 1 : -1;
        
        // Store adjusted value
        sample->points[i] = (short)(val * 32000.0);
    }
    
    return sample;
}

// Generates a sine wave with specified frequency
sample_t* generate_sin_sample(int freq) {
    // Allocate memory for sample
    sample_t *sample = (sample_t*)malloc(sizeof(sample_t));
    
    // Calculate sample points in one period
    sample->n_points = sample_rate / freq;
    
    // Allocate memory for sample points
    sample->points = (short*)malloc(sizeof(short) * sample->n_points);
    
    // For every sample point
    int i;
    for (i = 0; i < sample->n_points; i++) {
        // Calculate progress ratio
        float r = (float)i / (float)sample->n_points;
        
        // Calculate sinus value
        float val = sinf(2.0 * M_PI * r);
        
        // Store adjusted value
        sample->points[i] = (short)(val * 32000.0);
    }
    
    return sample;
}
