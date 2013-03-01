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

// Test sounds
sound_t *sound_lisa = NULL;
sound_t *sound_lisa2 = NULL;
sound_t *sound_lisa_dark = NULL;
sound_t *sound_sotw1 = NULL;
sound_t *sound_sotw2 = NULL;

// Notes

int main (int argc, char *argv[]) {
    tracks_init();
    initHardware();
    
    // Init sound
    tones_init();
    
    // Build test sounds
    
    sound_lisa = sound_build(22*2,
        C_5, 0.4, 100, silence, 0.1, 0,
        D_5, 0.4, 100, silence, 0.1, 0,
        E_5, 0.4, 100, silence, 0.1, 0,
        F_5, 0.4, 100, silence, 0.1, 0,
        G_5, 0.9, 100, silence, 0.1, 0,
        G_5, 0.9, 100, silence, 0.1, 0,
        A_5, 0.4, 100, silence, 0.1, 0,
        A_5, 0.4, 100, silence, 0.1, 0,
        A_5, 0.4, 100, silence, 0.1, 0,
        A_5, 0.4, 100, silence, 0.1, 0,
        G_5, 1.9, 100, silence, 0.1, 0,
        F_5, 0.4, 100, silence, 0.1, 0,
        F_5, 0.4, 100, silence, 0.1, 0,
        F_5, 0.4, 100, silence, 0.1, 0,
        F_5, 0.4, 100, silence, 0.1, 0,
        E_5, 0.9, 100, silence, 0.1, 0,
        E_5, 0.9, 100, silence, 0.1, 0,
        D_5, 0.4, 100, silence, 0.1, 0,
        D_5, 0.4, 100, silence, 0.1, 0,
        D_5, 0.4, 100, silence, 0.1, 0,
        D_5, 0.4, 100, silence, 0.1, 0,
        C_5, 1.9, 100, silence, 0.1, 0
    );
    
    sound_lisa2 = sound_build(22*2,
        A_5, 0.4, 100, silence, 0.1, 0,
        B_5, 0.4, 100, silence, 0.1, 0,
        C_5, 0.4, 100, silence, 0.1, 0,
        D_5, 0.4, 100, silence, 0.1, 0,
        E_5, 0.9, 100, silence, 0.1, 0,
        E_5, 0.9, 100, silence, 0.1, 0,
        F_5, 0.4, 100, silence, 0.1, 0,
        F_5, 0.4, 100, silence, 0.1, 0,
        F_5, 0.4, 100, silence, 0.1, 0,
        F_5, 0.4, 100, silence, 0.1, 0,
        E_5, 1.9, 100, silence, 0.1, 0,
        D_5, 0.4, 100, silence, 0.1, 0,
        D_5, 0.4, 100, silence, 0.1, 0,
        D_5, 0.4, 100, silence, 0.1, 0,
        D_5, 0.4, 100, silence, 0.1, 0,
        C_5, 0.9, 100, silence, 0.1, 0,
        C_5, 0.9, 100, silence, 0.1, 0,
        B_5, 0.4, 100, silence, 0.1, 0,
        B_5, 0.4, 100, silence, 0.1, 0,
        B_5, 0.4, 100, silence, 0.1, 0,
        B_5, 0.4, 100, silence, 0.1, 0,
        A_5, 1.9, 100, silence, 0.1, 0
    );
    
    sound_lisa_dark = sound_build(22*2,
        C_4, 0.4, 100, silence, 0.1, 0,
        D_4, 0.4, 100, silence, 0.1, 0,
        E_4, 0.4, 100, silence, 0.1, 0,
        F_4, 0.4, 100, silence, 0.1, 0,
        G_4, 0.9, 100, silence, 0.1, 0,
        G_4, 0.9, 100, silence, 0.1, 0,
        A_4, 0.4, 100, silence, 0.1, 0,
        A_4, 0.4, 100, silence, 0.1, 0,
        A_4, 0.4, 100, silence, 0.1, 0,
        A_4, 0.4, 100, silence, 0.1, 0,
        G_4, 1.9, 100, silence, 0.1, 0,
        F_4, 0.4, 100, silence, 0.1, 0,
        F_4, 0.4, 100, silence, 0.1, 0,
        F_4, 0.4, 100, silence, 0.1, 0,
        F_4, 0.4, 100, silence, 0.1, 0,
        E_4, 0.9, 100, silence, 0.1, 0,
        E_4, 0.9, 100, silence, 0.1, 0,
        D_4, 0.4, 100, silence, 0.1, 0,
        D_4, 0.4, 100, silence, 0.1, 0,
        D_4, 0.4, 100, silence, 0.1, 0,
        D_4, 0.4, 100, silence, 0.1, 0,
        C_4, 1.9, 100, silence, 0.1, 0
    );
    
    sound_sotw1 = sound_build(12*2,
        E_5, 0.4, 100, silence, 0.1, 0,
        G_5, 0.4, 100, silence, 0.1, 0,
        A_5, 0.65, 100, silence, 0.1, 0,
        E_5, 0.4, 100, silence, 0.1, 0,
        G_5, 0.4, 100, silence, 0.1, 0,
        Ax5, 0.15, 100, silence, 0.1, 0,
        A_5, 0.9, 100, silence, 0.1, 0,
        E_5, 0.4, 100, silence, 0.1, 0,
        G_5, 0.4, 100, silence, 0.1, 0,
        A_5, 0.65, 100, silence, 0.1, 0,
        G_5, 0.4, 100, silence, 0.1, 0,
        E_5, 1.65, 100, silence, 0.1, 0
    );
    
    sound_sotw2 = sound_build(12*2,
        B_5, 0.4, 100, silence, 0.1, 0,
        D_6, 0.4, 100, silence, 0.1, 0,
        E_6, 0.65, 100, silence, 0.1, 0,
        B_5, 0.4, 100, silence, 0.1, 0,
        D_6, 0.4, 100, silence, 0.1, 0,
        F_6, 0.15, 100, silence, 0.1, 0,
        E_6, 0.9, 100, silence, 0.1, 0,
        B_5, 0.4, 100, silence, 0.1, 0,
        D_6, 0.4, 100, silence, 0.1, 0,
        E_6, 0.65, 100, silence, 0.1, 0,
        D_6, 0.4, 100, silence, 0.1, 0,
        B_5, 1.65, 100, silence, 0.1, 0
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
        track_play(tracks[0], sound_lisa);
        track_play(tracks[1], sound_lisa2);
        current_led = ELEMENT_0;
    }
    
    if (press & ELEMENT_1) {
        track_play(tracks[0], sound_sotw1);
        track_play(tracks[1], sound_sotw2);
        current_led = ELEMENT_1;
    }
    
    if (press & ELEMENT_2) {
        track_play(tracks[2], sound_lisa);
    }
    
    if (press & ELEMENT_3) {
        track_play(tracks[3], sound_lisa);
    }
    
    if (press & ELEMENT_4) {
        track_play(tracks[4], sound_lisa);
    }
    
    if (press & ELEMENT_5) {
        track_play(tracks[5], sound_lisa);
    }
    
    if (press & ELEMENT_6) {
        track_play(tracks[6], sound_lisa);
    }
    
    if (press & ELEMENT_7) {
        track_play(tracks[7], sound_lisa_dark);
    }
    
    // Update LEDS
    pioc->sodr = current_led;
    pioc->codr = ~current_led;
}

// ABDAC interrupt routine
void abdac_isr(void) {
    // Default to silence
    int data = 0;
    
    // Get and advance track data
    int i;
    for (i = 0; i < TRACKS_TOTAL; i++) {
        data += track_advance(tracks[i]);
    }
    
    // Normalize by number of tracks and volume
    short output = (short)(data / (TRACKS_TOTAL*100));
    
    // Send data to ABDAC
    dac->SDR.channel0 = output;
    dac->SDR.channel1 = output;
}
