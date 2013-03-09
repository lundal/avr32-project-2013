#include "sample.c"
#include "tones.c"
#include "midi.c"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const midi_soundtrack_t sotw = {
    14,
    {
        {0.00 * SAMPLE_RATE, 0, E_5, 100},
        {0.50 * SAMPLE_RATE, 0, G_5, 100},
        {0.50 * SAMPLE_RATE, 0, A_5, 100},
        {0.75 * SAMPLE_RATE, 0, E_5, 100},
        {0.50 * SAMPLE_RATE, 0, G_5, 100},
        {0.50 * SAMPLE_RATE, 0, Ax5, 100},
        {0.25 * SAMPLE_RATE, 0, A_5, 100},
        {1.00 * SAMPLE_RATE, 0, E_5, 100},
        {0.50 * SAMPLE_RATE, 0, G_5, 100},
        {0.50 * SAMPLE_RATE, 0, A_5, 100},
        {0.75 * SAMPLE_RATE, 0, G_5, 100},
        {0.50 * SAMPLE_RATE, 0, E_5, 100},
        {1.75 * SAMPLE_RATE, 0,   0,   0},
        {1.00 * SAMPLE_RATE, 0,   0,   0},
    }
};

int main (int argc, char *argv[]) {
    // Init sound
    tones_init();
    midi_init();
    midi_play(&sotw);
    
    int32_t i;
    for (i = 0; i < 10; i++) {
        // Get data from midi player
        int16_t output = midi_tick();
        printf("ToDac: %d\n", output);
    }
    return 0;
}
