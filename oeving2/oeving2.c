/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include "oeving2.h"
#include "include/sys/interrupts.h"
#include <stdio.h>
#include <stdlib.h>

#include "sound.h"
#include "tones.h"
#include "track.h"

int current_led = ELEMENT_0;

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
    
    // Init notes
    tones_init();
    
    // Generate test samples
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
    //data += track_advance(track_5)/8;
    //data += track_advance(track_6)/8;
    //data += track_advance(track_7)/8;
    
    // Send data to ABDAC
    dac->SDR.channel0 = data;
    dac->SDR.channel1 = data;
}
