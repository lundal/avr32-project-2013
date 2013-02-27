/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include "oeving2.h"
#include "include/sys/interrupts.h"
#include <stdio.h>
#include <stdlib.h>

int current_button = ELEMENT_3;
int sample_rate = 48000;

int main (int argc, char *argv[]) {
    initHardware();
    
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
    pm->GCCTRL[6].div = 1;
    pm->GCCTRL[6].diven = 1;
    pm->GCCTRL[6].cen = 1;
    
    // Activate ABDAC
    dac->CR.en = 1;

    // Activate interrupts
    dac->IER.tx_ready = 1;
}

void button_isr(void) {
    // Debounce
    volatile int i = 1000;
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

void abdac_isr(void) {
    short data = (short) rand();
    
    // Send to ABDAC
    dac->SDR.channel0 = data;
    dac->SDR.channel1 = data;
}


