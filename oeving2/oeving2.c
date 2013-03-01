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
track_t *track_4 = NULL;
track_t *track_5 = NULL;
track_t *track_6 = NULL;
track_t *track_7 = NULL;

// Test sounds
sound_t *sound_lisa = NULL;
sound_t *sound_lisa2 = NULL;
sound_t *sound_sotw = NULL;

// Notes
sample_t *A_1 = NULL;
sample_t *Ax1 = NULL;
sample_t *B_1 = NULL;
sample_t *C_1 = NULL;
sample_t *Cx1 = NULL;
sample_t *D_1 = NULL;
sample_t *Dx1 = NULL;
sample_t *E_1 = NULL;
sample_t *F_1 = NULL;
sample_t *Fx1 = NULL;
sample_t *G_1 = NULL;
sample_t *Gx1 = NULL;
sample_t *A_2 = NULL;
sample_t *Ax2 = NULL;
sample_t *B_2 = NULL;
sample_t *C_2 = NULL;
sample_t *Cx2 = NULL;
sample_t *D_2 = NULL;
sample_t *Dx2 = NULL;
sample_t *E_2 = NULL;
sample_t *F_2 = NULL;
sample_t *Fx2 = NULL;
sample_t *G_2 = NULL;
sample_t *Gx2 = NULL;
sample_t *silent = NULL;

int main (int argc, char *argv[]) {
    // Init tracks
    track_0 = track_new();
    track_1 = track_new();
    track_2 = track_new();
    track_3 = track_new();
    track_4 = track_new();
    track_5 = track_new();
    track_6 = track_new();
    track_7 = track_new();
    
    initHardware();
    
    // Generate test samples
    A_1 = generate_square_sample(220);
    Ax1 = generate_square_sample(233);
    B_1 = generate_square_sample(247);
    C_1 = generate_square_sample(262);
    Cx1 = generate_square_sample(277);
    D_1 = generate_square_sample(294);
    Dx1 = generate_square_sample(311);
    E_1 = generate_square_sample(330);
    F_1 = generate_square_sample(349);
    Fx1 = generate_square_sample(370);
    G_1 = generate_square_sample(392);
    Gx1 = generate_square_sample(415);
    A_2 = generate_square_sample(440);
    Ax2 = generate_square_sample(466);
    B_2 = generate_square_sample(494);
    C_2 = generate_square_sample(524);
    Cx2 = generate_square_sample(554);
    D_2 = generate_square_sample(588);
    Dx2 = generate_square_sample(622);
    E_2 = generate_square_sample(660);
    F_2 = generate_square_sample(698);
    Fx2 = generate_square_sample(740);
    G_2 = generate_square_sample(784);
    Gx2 = generate_square_sample(830);
    silent = generate_silent_sample();
    
    // Build test sounds
    sound_lisa = build_sound(22*2,
        C_1, 0.4, 100, silent, 0.1, 0,
        D_1, 0.4, 100, silent, 0.1, 0,
        E_1, 0.4, 100, silent, 0.1, 0,
        F_1, 0.4, 100, silent, 0.1, 0,
        G_1, 0.9, 100, silent, 0.1, 0,
        G_1, 0.9, 100, silent, 0.1, 0,
        A_2, 0.4, 100, silent, 0.1, 0,
        A_2, 0.4, 100, silent, 0.1, 0,
        A_2, 0.4, 100, silent, 0.1, 0,
        A_2, 0.4, 100, silent, 0.1, 0,
        G_1, 1.9, 100, silent, 0.1, 0,
        F_1, 0.4, 100, silent, 0.1, 0,
        F_1, 0.4, 100, silent, 0.1, 0,
        F_1, 0.4, 100, silent, 0.1, 0,
        F_1, 0.4, 100, silent, 0.1, 0,
        E_1, 0.9, 100, silent, 0.1, 0,
        E_1, 0.9, 100, silent, 0.1, 0,
        D_1, 0.4, 100, silent, 0.1, 0,
        D_1, 0.4, 100, silent, 0.1, 0,
        D_1, 0.4, 100, silent, 0.1, 0,
        D_1, 0.4, 100, silent, 0.1, 0,
        C_1, 1.9, 100, silent, 0.1, 0
    );
    
    sound_lisa2 = build_sound(22*2,
        A_1, 0.4, 100, silent, 0.1, 0,
        B_1, 0.4, 100, silent, 0.1, 0,
        C_1, 0.4, 100, silent, 0.1, 0,
        D_1, 0.4, 100, silent, 0.1, 0,
        E_1, 0.9, 100, silent, 0.1, 0,
        E_1, 0.9, 100, silent, 0.1, 0,
        F_1, 0.4, 100, silent, 0.1, 0,
        F_1, 0.4, 100, silent, 0.1, 0,
        F_1, 0.4, 100, silent, 0.1, 0,
        F_1, 0.4, 100, silent, 0.1, 0,
        E_1, 1.9, 100, silent, 0.1, 0,
        D_1, 0.4, 100, silent, 0.1, 0,
        D_1, 0.4, 100, silent, 0.1, 0,
        D_1, 0.4, 100, silent, 0.1, 0,
        D_1, 0.4, 100, silent, 0.1, 0,
        C_1, 0.9, 100, silent, 0.1, 0,
        C_1, 0.9, 100, silent, 0.1, 0,
        B_1, 0.4, 100, silent, 0.1, 0,
        B_1, 0.4, 100, silent, 0.1, 0,
        B_1, 0.4, 100, silent, 0.1, 0,
        B_1, 0.4, 100, silent, 0.1, 0,
        A_1, 1.9, 100, silent, 0.1, 0
    );
    
    sound_sotw = build_sound(12*2,
        E_1, 0.4, 100, silent, 0.1, 0,
        G_1, 0.4, 100, silent, 0.1, 0,
        A_2, 0.65, 100, silent, 0.1, 0,
        E_1, 0.4, 100, silent, 0.1, 0,
        G_1, 0.4, 100, silent, 0.1, 0,
        Ax2, 0.15, 100, silent, 0.1, 0,
        A_2, 0.9, 100, silent, 0.1, 0,
        E_1, 0.4, 100, silent, 0.1, 0,
        G_1, 0.4, 100, silent, 0.1, 0,
        A_2, 0.65, 100, silent, 0.1, 0,
        G_1, 0.4, 100, silent, 0.1, 0,
        E_1, 1.65, 100, silent, 0.1, 0
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
        track_play(track_0, sound_lisa);
        track_play(track_1, sound_lisa2);
    }
    
    if (press & ELEMENT_1) {
        track_play(track_2, sound_sotw);
    }
    
    if (press & ELEMENT_2) {
        track_play(track_2, sound_lisa);
    }
    
    if (press & ELEMENT_3) {
        track_play(track_3, sound_lisa);
    }
    
    if (press & ELEMENT_4) {
        track_play(track_4, sound_lisa);
    }
    
    if (press & ELEMENT_5) {
        track_play(track_5, sound_lisa);
    }
    
    if (press & ELEMENT_6) {
        track_play(track_6, sound_lisa);
    }
    
    if (press & ELEMENT_7) {
        track_play(track_7, sound_lisa);
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
    data += track_advance(track_0)/8;
    data += track_advance(track_1)/8;
    data += track_advance(track_2)/8;
    data += track_advance(track_3)/8;
    data += track_advance(track_4)/8;
    data += track_advance(track_5)/8;
    data += track_advance(track_6)/8;
    data += track_advance(track_7)/8;
    
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
