#include "sample.h"
#include "tones.h"
#include "midi.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/soundcard.h>

// Midi sound files
#include "hes_a_pirate.c"

char buffer[100];

int dsp_bits = 16;
int dsp_rate = 22000;

int sound_file;
FILE *audio_file;

int main (int argc, char *argv[]) {
    // Init sound
    tones_init(WAVE_SQUARE);
    //tones_init(WAVE_SINE);
    midi_init();
    midi_play(&hes_a_pirate);
    
    // Open audio device
    sound_file = open("/dev/dsp", O_WRONLY);
    
    // Set proper playback parameters
    ioctl(sound_file, SOUND_PCM_WRITE_BITS, &dsp_bits);
    ioctl(sound_file, SOUND_PCM_WRITE_RATE, &dsp_rate);
    
    // Main loop
    while (1) {
        /*
        int i;
        for (i = 0; i < 100; i++) {
            // Get data from midi player
            int16_t data = midi_tick();
            
            // Convert to char
            char val = (char)(data>>8);
            
            // Add to buffer
            buffer[i] = val;
        }
        // Write to audio device
        write(sound_file, buffer, 100);
        */
        
        // Get data from midi player
        int16_t data = midi_tick();
        
        // Write to audio device
        write(sound_file, &data, 2);
        
    }
    
    // Close audio device
    close(sound_file);
    
    return 0;
}
