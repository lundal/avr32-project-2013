/******************************************************************************
 * 
 * Assignment 1: UCSysDes
 * 
 * Register conventions used:
 * r0-r3: Temporary data. Will likely be overidden by subroutines.
 * r4-r11: Important data. Subroutines should not modify these without pushing to stack!
 * 
 * Used registers:
 * r5 - piob pointer
 * r6 - pioc pointer
 * r7 - All elements
 * 
 *****************************************************************************/

.include "io.s"  /* Include useful constants */

/* Symbols for elements 0 through 7: Can be added to target multiple */
E_0 = 0b00000001
E_1 = 0b00000010
E_2 = 0b00000100
E_3 = 0b00001000
E_4 = 0b00010000
E_5 = 0b00100000
E_6 = 0b01000000
E_7 = 0b10000000
E_ALL = 0b11111111



/******************************************************************************
 * 
 * Text segment: Includes all program code
 * 
 *****************************************************************************/
.text

.globl  _start

/* Program starts here */
_start:
    /* Clear all registers */
    mov r0, 0
    mov r1, 0
    mov r2, 0
    mov r3, 0
    mov r4, 0
    mov r5, 0
    mov r6, 0
    mov r7, 0
    mov r8, 0
    mov r9, 0
    mov r10, 0
    mov r11, 0
    mov r12, 0
    
    /* Load piob pointer */
    mov r0, piob
    ld.w r5, r0
    
    /* Load pioc pointer */
    mov r0, pioc
    ld.w r6, r0
    
    /* Load all elements to r7 */
    mov r7, E_ALL
    
    /* Enable all buttons */
    st.w r5[AVR32_PIO_PER], r7
    st.w r5[AVR32_PIO_PUER], r7
    
    /* Enable all LEDS */
    st.w r6[AVR32_PIO_PER], r7
    st.w r6[AVR32_PIO_OER], r7
    
    
    rjmp test 



/* Try to move the light with button 0 and 1 */
test:
    /* Start with LED 0 */
    mov r8, E_O
loop:
    /* Disable LEDS */
    st.w r6[AVR32_PIO_CODR], r7

    /* Enable current LED */
    st.w r6[AVR32_PIO_SODR], r8

    /* Read button states */
    ld.w r0, r5[AVR32_PIO_PDSR]
    
    /* Copy and invert so that down is 1 and up is 0 */
    mov r1, r0
    com r1
    
    /* Mask button 0 */
    mov r2, E_0
    and r2, r1

    /* Loop if button is up */
    cp.w r2, 0 /* Not actually needed because AND will set Z, but kept for clarity */
    breq loop
    
    /* Shift LED one left */
    lsl r8, 1
    
    /* If shifted beyond LED 7, set to LED 0 */
    cp.w r8, E_7
    movgt r8, E_0
    
    /* Loop */
    rjmp loop



/* Eternal polling loop that enables LEDS over buttons that are down */
bled:
    /* Read button states */ 
    ld.w r0, r5[AVR32_PIO_PDSR]
    
    /* Copy and invert so that down is 1 and up is 0 */
    mov r1, r0
    com r1
    
    /* Disable LEDS over buttons that are up */
    st.w r6[AVR32_PIO_CODR], r0
    
    /* Enable LEDS over buttons that are down */
    st.w r6[AVR32_PIO_SODR], r1
    
    /* Loop */
    rjmp bled



/* piob address */
piob:
    .int AVR32_PIOB

/* pioc address */
pioc:
    .int AVR32_PIOC



/******************************************************************************
 * 
 * Data segment: Includes all variables
 * 
 *****************************************************************************/
.data

