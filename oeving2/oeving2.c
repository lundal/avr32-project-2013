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

int current_button = ELEMENT_3;

// Store the sample rate
int sample_rate = 1000;

// Currently playing sample
sample_t *current_sample = NULL;
int current_sample_point = 0;

int main (int argc, char *argv[]) {
    initHardware();
    
    //Generate a test sample
    //current_sample = generate_sin_wave(1000, 0.5, 0.2);
    //current_sample_point = 0;
    //generate_sin_wave(440, 0.5, 0.1);
    current_sample = generate_sin_sample(440);
    //current_sample = generate_square_sample(440);
    
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
    
    // Right press
    if (press & ELEMENT_0) {
        current_button = current_button >> 1;
        if (current_button < ELEMENT_0) {
            current_button = ELEMENT_7;
        }
    }
    
    // Left press
    if (press & ELEMENT_2) {
        current_button = current_button << 1;
        if (current_button > ELEMENT_7) {
            current_button = ELEMENT_0;
        }
    }
    
    // Update LEDS
    pioc->sodr = current_button;
    pioc->codr = ~current_button;
}

// ABDAC interrupt routine
void abdac_isr(void) {
    //Generate noise
    //short data = (short) rand();
    
    
    // NULL data
    short data = 0;
    
    // If there is a sample loaded
    if (current_sample != NULL) {
        // If done playing: Remove it!
        if (current_sample_point >= current_sample->n_points) {
            //current_sample = NULL;
            current_sample_point = 0;
        }
        // Copy data and update progress
        else {
            data = current_sample->points[current_sample_point];
            current_sample_point++;
        }
    }
    
    // Send data to ABDAC
    dac->SDR.channel0 = data;
    dac->SDR.channel1 = data;
}

// Gererates a sample with a sinus wave. Frequency in hz, amplitude from 0 to 1, length in seconds.
sample_t* generate_sin_wave(int freq, float amplitude, float length) {

    sample_t *sample = (sample_t*)malloc(sizeof(sample_t));
    sample->n_points = (int)( length * (float)sample_rate );
    sample->points = (short*)malloc(sizeof(short) * sample->n_points);
    float period = sample_rate / (float)freq;
    int i;
    for (i = 0; i < sample->n_points; i++) {
        // Generate wave
        float sinval = sinf( 2.0 * M_PI * (float)i / period );
        short point = (short)( sinval * amplitude * 32768.0 );
        /*
        // Fade in
        int fade_in = 100;
        if (i < fade_in) {
            point = point * i / fade_in;
        }
        
        // Fade out
        int fade_out = 100;
        int remaining = sample->n_points - i;
        if (remaining < fade_out) {
            point = point * remaining / fade_out;
        }
        */
        sample->points[i] = point;
    }
    
    return sample;
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
        sample->points[i] = (short)(val * 20000.0);
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
        sample->points[i] = (short)(val * 20000.0);
    }
    
    return sample;
}
