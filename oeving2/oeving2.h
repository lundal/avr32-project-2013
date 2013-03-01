/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 * Headerfil
 *
 *****************************************************************************/

#ifndef OEVING_2_H
#define OEVING_2_H

// Include Atmel header files
#include <avr32/ap7000.h>
#include <sys/interrupts.h>

// Interrupt priority levels
#define ABDAC_INT_LEVEL 0
#define BUTTONS_INT_LEVEL 0

// Convenient references for buttons and LEDs
#define ELEMENT_0 0x01
#define ELEMENT_1 0x02
#define ELEMENT_2 0x04
#define ELEMENT_3 0x08
#define ELEMENT_4 0x10
#define ELEMENT_5 0x20
#define ELEMENT_6 0x40
#define ELEMENT_7 0x80
#define ELEMENT_ALL 0xFF

// Convenient pointers to I/O devices
volatile avr32_abdac_t *dac = &AVR32_ABDAC;
volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_pm_t *pm = &AVR32_PM;

// Prototypes
int main (int argc, char *argv[]);
static void initHardware (void);
static void initIntc(void);
static void initButtons(void);
static void initLeds(void);
static void initAudio(void);
static void button_isr(void);
static void abdac_isr(void);

#endif
