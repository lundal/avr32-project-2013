#include "sample.h"

#include <stdlib.h>
#include <math.h>

// Generates different waveforms with specified frequency
sample_t* sample_gen(int16_t freq, int8_t wavetype) {
    // Allocate memory for sample
    sample_t *sample = (sample_t*)malloc(sizeof(sample_t));
    
    // Calculate sample points in one period
    sample->n_points = SAMPLE_RATE / freq;
    
    // Allocate memory for sample points
    sample->points = (int16_t*)malloc(sizeof(int16_t) * sample->n_points);
    
    // For every sample point
    int16_t i;
	for (i = 0; i < sample->n_points; i++) {
		double val = 0.0;
		switch (wavetype) {
			case WAVE_SQUARE: {
                // Calculate square value
                val = (i > sample->n_points/2) ? 1.0 : -1.0;
                
                break;
            }
            case WAVE_SINE: {
                // Calculate progress ratio
                double r = (double)i / (double)sample->n_points;
                
                // Calculate sinus value
                val = sin(2.0 * M_PI * r);
                
                break;
            }
		}
		// Store adjusted value
		sample->points[i] = (int16_t)(val * SAMPLE_AMPLITUDE);
    }
    return sample;
}

