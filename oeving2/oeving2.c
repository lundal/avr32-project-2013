/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 *****************************************************************************/

#include "oeving2.h"
#include "include/sys/interrupts.h"

int current_button = ELEMENT_3;

int main (int argc, char *argv[]) {
    initHardware();
    
    while(1);
    return 0;
}

/* funksjon for å initialisere maskinvaren, må utvides */
void initHardware (void) {
    initIntc();
    initLeds();
    initButtons();
    //initAudio();
}

void initIntc(void) {
    set_interrupts_base((void *)AVR32_INTC_ADDRESS);
    init_interrupts();
}

void initButtons(void) {
    register_interrupt( button_isr, AVR32_PIOB_IRQ/32, AVR32_PIOB_IRQ % 32, BUTTONS_INT_LEVEL);
    piob->per = 0xFF;
    piob->puer = 0xFF;
    piob->ier = 0xFF;
    /* (...) */
}

void initLeds(void) {
    pioc->per = 0xFF;
    pioc->oer = 0xFF;
    pioc->codr = 0xFF;
    /* (...) */
}

void initAudio(void) {
    register_interrupt( abdac_isr, AVR32_ABDAC_IRQ/32, AVR32_ABDAC_IRQ % 32, ABDAC_INT_LEVEL);
    /* (...) */
}

void button_isr(void) {
    // Debounce
    volatile int i = 1000;
    while (i > 0) {
        i--;
    }
    
    //Read states
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
    dac->CR.en = 1;
}


