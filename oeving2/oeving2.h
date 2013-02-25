/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 * Headerfil
 *
 *****************************************************************************/

#ifndef OEVING_2_H /* vanlig måte å unngå headerrekursjon på */
#define OEVING_2_H

#include <avr32/ap7000.h> /* inkludere Atmel-headerfil */
#include <sys/interrupts.h>

/* legg andre "includes" her, sammen med "defines" og prototyper */
#define ABDAC_INT_LEVEL 0
#define BUTTONS_INT_LEVEL 0

/* prototyper */
int main (int argc, char *argv[]);
static void initIntc(void);
static void initButtons(void);
static void initLeds(void);
static void initAudio(void);
static void initHardware (void);

static void button_isr(void);
static void abdac_isr(void);


#endif

//self-defined protoypes, variables and macroes

__int_handler *int_handler(void);

//Struct pointers given in the compendium(3.2.4 p.44)
//pointing to PIO B and C, Power manager and ABDAC
volatile ac32_dac_t *dac = &AVR32_DAC;
volatile ac32_pio_t *piob = &AVR32_PIOC;
volatile ac32_dac_t *pioc = &AVR32_PIOB;
volatile ac32_dsm_t *sm = &AVR32_SM;


