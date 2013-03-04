/*****************************************************************************
 * 
 * Øving 2 Code file
 *
 *****************************************************************************/

#include "oeving2.h"

#include "include/sys/interrupts.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "sample.h"
#include "tones.h"
#include "midi.h"

int32_t current_led = ELEMENT_0;

// Test sounds

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
        {0.75 * SAMPLE_RATE, 0,   0,   0},
        {1.00 * SAMPLE_RATE, 0,   0,   0},
    }
};

const midi_soundtrack_t sotw2 = {
    28,
    {
        {0.00 * SAMPLE_RATE, 0, D_5, 100},
        {0.00 * SAMPLE_RATE, 1, G_5, 100},

        {0.50 * SAMPLE_RATE, 0, F_5, 100},
        {0.00 * SAMPLE_RATE, 1, Ax5, 100},

        {0.50 * SAMPLE_RATE, 0, G_5, 100},
        {0.00 * SAMPLE_RATE, 1, C_6, 100},

        {0.75 * SAMPLE_RATE, 0, D_5, 100},
        {0.00 * SAMPLE_RATE, 1, G_5, 100},

        {0.50 * SAMPLE_RATE, 0, F_5, 100},
        {0.00 * SAMPLE_RATE, 1, Ax5, 100},

        {0.50 * SAMPLE_RATE, 0, Gx5, 100},
        {0.00 * SAMPLE_RATE, 1, Cx6, 100},

        {0.25 * SAMPLE_RATE, 0, G_5, 100},
        {0.00 * SAMPLE_RATE, 1, C_6, 100},

        {1.00 * SAMPLE_RATE, 0, D_5, 100},
        {0.00 * SAMPLE_RATE, 1, G_5, 100},

        {0.50 * SAMPLE_RATE, 0, F_5, 100},
        {0.00 * SAMPLE_RATE, 1, Ax5, 100},

        {0.50 * SAMPLE_RATE, 0, G_5, 100},
        {0.00 * SAMPLE_RATE, 1, C_6, 100},

        {0.75 * SAMPLE_RATE, 0, F_5, 100},
        {0.00 * SAMPLE_RATE, 1, Ax5, 100},

        {0.50 * SAMPLE_RATE, 0, D_5, 100},
        {0.00 * SAMPLE_RATE, 1, G_5, 100},

        {0.75 * SAMPLE_RATE, 0,   0,   0},
        {0.00 * SAMPLE_RATE, 1,   0,   0},

        {1.00 * SAMPLE_RATE, 0,   0,   0},
        {0.00 * SAMPLE_RATE, 1,   0,   0},
    }
};

int main (int argc, char *argv[]) {
    // Init sound
    tones_init();
    midi_init();
    midi_play(&sotw);
    
    // Init hardware
    initHardware();
    
    // Busywaiting is FUN!
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
    pm->GCCTRL[6].cen = 1;
    
    // Activate ABDAC
    dac->CR.en = 1;

    // Activate interrupts
    dac->IER.tx_ready = 1;
}

// Button interrupt routine
void button_isr(void) {
    // Debounce
    int16_t i = 500;
    while (i > 0) {
        i--;
    }
    
    // Read states
    int32_t event_states = piob->isr;
    int32_t button_states = ~piob->pdsr;
    
    int32_t press = event_states & button_states;
    
    if (press & ELEMENT_0) {
        midi_play(&sotw);
        current_led = ELEMENT_0;
    }
    
    if (press & ELEMENT_1) {
        midi_play(&sotw2);
        current_led = ELEMENT_1;
    }
    
    if (press & ELEMENT_2) {
        current_led = ELEMENT_2;
    }
    
    if (press & ELEMENT_3) {
        current_led = ELEMENT_3;
    }
    
    if (press & ELEMENT_4) {
        current_led = ELEMENT_4;
    }
    
    if (press & ELEMENT_5) {
        current_led = ELEMENT_5;
    }
    
    if (press & ELEMENT_6) {
        current_led = ELEMENT_6;
    }
    
    if (press & ELEMENT_7) {
        current_led = ELEMENT_7;
    }
    
    // Update LEDS
    pioc->sodr = current_led;
    pioc->codr = ~current_led;
}

// ABDAC interrupt routine
void abdac_isr(void) {
    // Get data from midi player
    int16_t output = midi_tick();
    
    // Send data to ABDAC
    dac->SDR.channel0 = output;
    dac->SDR.channel1 = output;
}
