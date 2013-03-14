#include "tones.h"

#include <stdlib.h>
#include <math.h>

// Generates tones
void tones_init(int16_t wave_type) {
    // Allocate memory
    tones = (sample_t**)malloc(sizeof(sample_t*) * 12);
    
    // Generate silent sample
    silence = sample_gen(100, WAVE_FLAT);
    
    // Generate tones (C5 - B5)
    tones[ 0] = sample_gen(262, wave_type);
    tones[ 1] = sample_gen(277, wave_type);
    tones[ 2] = sample_gen(294, wave_type);
    tones[ 3] = sample_gen(311, wave_type);
    tones[ 4] = sample_gen(330, wave_type);
    tones[ 5] = sample_gen(349, wave_type);
    tones[ 6] = sample_gen(370, wave_type);
    tones[ 7] = sample_gen(392, wave_type);
    tones[ 8] = sample_gen(415, wave_type);
    tones[ 9] = sample_gen(440, wave_type);
    tones[10] = sample_gen(466, wave_type);
    tones[11] = sample_gen(494, wave_type);
}
