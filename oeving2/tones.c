#include "tones.h"

#include <stdlib.h>
#include <math.h>

// Generates tones
void tones_init() {
    // Allocate memory
    tones = (sample_t**)malloc(sizeof(sample_t*) * 12);
    
    // Generate silent sample
    silence = sample_gen_silence();
    
    // Generate tones (C5 - B5)
    tones[ 0] = sample_gen_square(262);
    tones[ 1] = sample_gen_square(277);
    tones[ 2] = sample_gen_square(294);
    tones[ 3] = sample_gen_square(311);
    tones[ 4] = sample_gen_square(330);
    tones[ 5] = sample_gen_square(349);
    tones[ 6] = sample_gen_square(370);
    tones[ 7] = sample_gen_square(392);
    tones[ 8] = sample_gen_square(415);
    tones[ 9] = sample_gen_square(440);
    tones[10] = sample_gen_square(466);
    tones[11] = sample_gen_square(494);
    tones[ 0] = sample_gen_square(262);
    /*
    tones[ 1] = sample_gen_sin(277);
    tones[ 2] = sample_gen_sin(294);
    tones[ 3] = sample_gen_sin(311);
    tones[ 4] = sample_gen_sin(330);
    tones[ 5] = sample_gen_sin(349);
    tones[ 6] = sample_gen_sin(370);
    tones[ 7] = sample_gen_sin(392);
    tones[ 8] = sample_gen_sin(415);
    tones[ 9] = sample_gen_sin(440);
    tones[10] = sample_gen_sin(466);
    tones[11] = sample_gen_sin(494);
    */
}
