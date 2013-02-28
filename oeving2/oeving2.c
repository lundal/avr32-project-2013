/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include "oeving2.h"
#include "include/sys/interrupts.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

int current_led = ELEMENT_0;

// Store the sample rate (calculated later)
int sample_rate = 1000;

// Sound tracks
track_t *track_0 = NULL;
track_t *track_1 = NULL;
track_t *track_2 = NULL;
track_t *track_3 = NULL;

// Test sounds
sound_t *sound_0 = NULL;
sound_t *sound_1 = NULL;
sound_t *sound_2 = NULL;

// Notes
sample_t *A1 = NULL;
sample_t *B1 = NULL;
sample_t *C1 = NULL;
sample_t *D1 = NULL;
sample_t *E1 = NULL;
sample_t *F1 = NULL;
sample_t *G1 = NULL;
sample_t *A2 = NULL;
sample_t *B2 = NULL;
sample_t *C2 = NULL;
sample_t *D2 = NULL;
sample_t *E2 = NULL;
sample_t *F2 = NULL;
sample_t *G2 = NULL;
sample_t *silent = NULL;

int main (int argc, char *argv[]) {
    // Init tracks
    track_0 = track_new();
    track_1 = track_new();
    track_2 = track_new();
    track_3 = track_new();
    
    initHardware();
    
    // Generate test samples
    A1 = generate_square_sample(220);
    B1 = generate_square_sample(247);
    C1 = generate_square_sample(262);
    D1 = generate_square_sample(294);
    E1 = generate_square_sample(330);
    F1 = generate_square_sample(349);
    G1 = generate_square_sample(392);
    A2 = generate_square_sample(440);
    B2 = generate_square_sample(494);
    C2 = generate_square_sample(524);
    D2 = generate_square_sample(588);
    E2 = generate_square_sample(660);
    F2 = generate_square_sample(698);
    G2 = generate_square_sample(784);
    silent = generate_silent_sample();
    
    // Build test sounds
    sound_0 = build_sound(2, A2, 1.0, 10, silent, 1.0, 100);
    sound_1 = build_sound(1, A2, 1.0, 10);
    sound_2 = build_sound(44,
        C1, 0.4, 100, silent, 0.1, 0,
        D1, 0.4, 100, silent, 0.1, 0,
        E1, 0.4, 100, silent, 0.1, 0,
        F1, 0.4, 100, silent, 0.1, 0,
        G1, 0.9, 100, silent, 0.1, 0,
        G1, 0.9, 100, silent, 0.1, 0,
        A2, 0.4, 100, silent, 0.1, 0,
        A2, 0.4, 100, silent, 0.1, 0,
        A2, 0.4, 100, silent, 0.1, 0,
        A2, 0.4, 100, silent, 0.1, 0,
        G1, 1.9, 100, silent, 0.1, 0,
        F1, 0.4, 100, silent, 0.1, 0,
        F1, 0.4, 100, silent, 0.1, 0,
        F1, 0.4, 100, silent, 0.1, 0,
        F1, 0.4, 100, silent, 0.1, 0,
        E1, 0.9, 100, silent, 0.1, 0,
        E1, 0.9, 100, silent, 0.1, 0,
        D1, 0.4, 100, silent, 0.1, 0,
        D1, 0.4, 100, silent, 0.1, 0,
        D1, 0.4, 100, silent, 0.1, 0,
        D1, 0.4, 100, silent, 0.1, 0,
        C1, 1.9, 100, silent, 0.1, 0
    );
    
    while(1);
    return 0;
}

// Initializes the hardware
void initHardware (void) {
    initIntc();
    initLeds();
    initButtons();
    initAudio();
}

// Initializes the interrupt controller
void initIntc(void) {
    set_interrupts_base((void *)AVR32_INTC_ADDRESS);
    init_interrupts();
}

// Initializes the buttons
void initButtons(void) {
    // Set interrupt routine
    register_interrupt( button_isr, AVR32_PIOB_IRQ/32, AVR32_PIOB_IRQ % 32, BUTTONS_INT_LEVEL);
    
    piob->per = 0xFF;
    piob->puer = 0xFF;
    piob->ier = 0xFF;
}

// Initializes the LEDs
void initLeds(void) {
    pioc->per = 0xFF;
    pioc->oer = 0xFF;
    pioc->codr = 0xFF;
}

// Initializes the ABDAC
void initAudio(void) {
    // Set interrupt routine
    register_interrupt( abdac_isr, AVR32_ABDAC_IRQ/32, AVR32_ABDAC_IRQ % 32, ABDAC_INT_LEVEL);
    
    // Release pins from PIO
    piob->PUER.p20 = 1;
    piob->PUER.p21 = 1;
    // Release pins from PIO
    piob->PDR.p20 = 1;
    piob->PDR.p21 = 1;
    
    // Set pins to peripheral A (ABDAC)
    piob->ASR.p20 = 1;
    piob->ASR.p21 = 1;
    
    // Connect and activate oscillator 1
    pm->GCCTRL[6].pllsel = 0;
    pm->GCCTRL[6].oscsel = 1;
    pm->GCCTRL[6].div = 0;
    pm->GCCTRL[6].diven = 0;
    pm->GCCTRL[6].cen = 1;
    
    // Set sample rate = clock / 256
    sample_rate = 12000000 / 256; // ~48kHz
    
    // Divide by 2*(div+1) if enabled
    //sample_rate = sample_rate / (2*(0+1);
    
    // Activate ABDAC
    dac->CR.en = 1;

    // Activate interrupts
    dac->IER.tx_ready = 1;
}

// Button interrupt routine
void button_isr(void) {
    // Debounce
    int i = 500;
    while (i > 0) {
        i--;
    }
    
    // Read states
    int event_states = piob->isr;
    int button_states = ~piob->pdsr;
    
    int press = event_states & button_states;
    
    if (press & ELEMENT_0) {
        track_play(track_0, sound_2);
        current_led = ELEMENT_0;
    }
    
    if (press & ELEMENT_1) {
        track_play(track_1, sound_2);
        current_led = ELEMENT_1;
    }
    
    if (press & ELEMENT_2) {
        track_play(track_2, sound_2);
        current_led = ELEMENT_2;
    }
    
    if (press & ELEMENT_3) {
        track_play(track_3, sound_2);
        current_led = ELEMENT_3;
    }
    
    // Update LEDS
    pioc->sodr = current_led;
    pioc->codr = ~current_led;
}

// ABDAC interrupt routine
void abdac_isr(void) {
    // Default to silence
    short data = 0;
    
    // Get and advance track data
    data += track_advance(track_0)/4;
    data += track_advance(track_1)/4;
    data += track_advance(track_2)/4;
    data += track_advance(track_3)/4;
    
    // Send data to ABDAC
    dac->SDR.channel0 = data;
    dac->SDR.channel1 = data;
}

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

static sound_t* build_sound(int n_samples, ...) {
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
        sample_t* sample = va_arg(args, sample_t*);
        double duration = va_arg(args, double);
        int volume = va_arg(args, int);
        
        sound->samples[i] = sample;
        sound->sample_reps[i] = (int)(duration * (double)sample_rate / (double)sample->n_points);
        sound->sample_vol[i] = volume;
    }
    
    return sound;
}


short track_advance(track_t *track) {
    // Default to silence
    short data = 0;

    // If it has a sound
    if (track->sound != NULL) {
        
        // Get current sample point (assume it exists)
        data = track->sound->samples[track->current_sample]->points[track->current_sample_point];
        
        // Adjust volume
        data = (short)((int)data * track->sound->sample_vol[track->current_sample] / 100);
        
        track->current_sample_point++;
        
        if (track->current_sample_point == track->sound->samples[track->current_sample]->n_points) {
            track->current_sample_point = 0;
            track->current_sample_iteration++;
        
            if (track->current_sample_iteration == track->sound->sample_reps[track->current_sample]) {
                track->current_sample_iteration = 0;
                track->current_sample++;
            
                if (track->current_sample == track->sound->n_samples) {
                    track->current_sample = 0;
                    track->sound = NULL;
                }
            } 
        } 
    }
    
    return data;
}

track_t* track_new() {
    track_t* track = (track_t*)malloc(sizeof(track_t));
    track_play(track, NULL);
    return track;
}

void track_play(track_t *track, sound_t *sound) {
    track->sound = sound;
    track->current_sample = 0;
    track->current_sample_iteration = 0;
    track->current_sample_point = 0;
}
