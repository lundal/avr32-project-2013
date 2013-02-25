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

/*
ACTIVATED_BUTTONS is supposed to denote the following:
7-5 are for 3 individual music pieces
4-3 are the volume buttons
2 is the panic button, i.e the music, if it's playing, will stop and be ready for a new song should a button to play it, be pressed.
*/
#define ACTIVATED_BUTTONS 0xfc


//__int_handler *int_handler(void);
__int_handler *abdac_handler(void);//I'm thinking this might be a good thing
__int_handler *button_handler(void);



//Struct pointers given in the compendium(3.2.4 p.44)
//pointing to PIO B and C, Power manager and ABDAC
volatile avr32_dac_t *dac = &AVR32_DAC;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_dac_t *piob = &AVR32_PIOB;
volatile avr32_sm_t *sm = &AVR32_SM;


