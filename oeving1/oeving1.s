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
    
    /* Clear registers to make debugging easier */
    rcall clear_regs

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
    
    /* Enable button interrupts */
    st.w r5[AVR32_PIO_IER], r7
    mov r0, E_ALL
    com r0
    st.w r5[AVR32_PIO_IDR], r0

    /* For the sake of simplicity, EVBA is set to 0 */
    mov r1, 0
    mtsr 4, r1

    /* Set autovector */
    mov r0, intc
    ld.w r1, r0
    mov r0, button_interrupt
    st.w r1[AVR32_INTC_IPR14], r0

    /* Initialize with LED 0 */
    mov r8, 1

    /* Disable LEDS */
    st.w r6[AVR32_PIO_CODR], r7

    /* Enable current LED */
    st.w r6[AVR32_PIO_SODR], r8

    /* Disable Global Interupt Mask */
    csrf 16

infloop:
    
    /*
    mov r12, 1000000
    rcall sleeper
    lsl r8, 1
    rcall update_leds
    */
    
    rjmp infloop



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



/* Button interrupt handler */
button_interrupt:
    
    /* Debounce */
    mov r12, 100000
    rcall sleeper

    /* Read ISR to make sure it knows the interupt was handled */
    ld.w r0, r5[AVR32_PIO_ISR]
    
    /* Read button states */ 
    ld.w r1, r5[AVR32_PIO_PDSR]
    
    /* Copy and invert so that down is 1 and up is 0 */
    mov r2, r1
    com r2
    
    /* Was this event related to the left button? If not then skip (mask event by button) */
    mov r3, E_7
    and r3, r0
    breq button_interrupt_after_left
    
    /* Is the button down? If not then skip (mask state by button) */
    mov r3, E_7
    and r3, r2
    breq button_interrupt_after_left
    
    /* Shift LED one left */
    lsl r8, 1
    
    /* If shifted beyond LED 7, set to LED 0 */
    cp.w r8, E_7
    movhi r8, E_0
    
button_interrupt_after_left:
    
    /* Was this event related to the right button? If not then skip (mask event by button) */
    mov r3, E_0
    and r3, r0
    breq button_interrupt_after_right
    
    /* Is the button down? If not then skip (mask state by button) */
    mov r3, E_0
    and r3, r2
    breq button_interrupt_after_right
    
    /* Shift LED one right */
    lsr r8, 1
    
    /* If shifted beyond LED O, set to LED 7 */
    cp.w r8, E_0
    
    /* Workaround because E_7 is too large for mov{cond} (addlo crashes, therefore sublo) */
    movlo r8, E_7-1
    sublo r8, -1
    
button_interrupt_after_right:
    
    /* Disable all LEDS */
    st.w r6[AVR32_PIO_CODR], r7
    
    /* Enable current LED */
    st.w r6[AVR32_PIO_SODR], r8
    
    rete



/* Clear all registers */
clear_regs:
    
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

    ret SP



/* Update LEDS */
update_leds:
    
    /* Disable all LEDS */
    st.w r6[AVR32_PIO_CODR], r7
    
    /* Enable current LED */
    st.w r6[AVR32_PIO_SODR], r8

    ret SP



/* Sleeps for the duration of r12 */
sleeper:
    
    /* Set sleep time */
    mov r0, r12
    
sleeper_start:
    
    /* Subtract one */
    sub r0, 1
    
    /* check if done */
    cp.w r0, 0
    brle sleeper_end
    
    /* Sleep more! */
    rjmp sleeper_start
    
sleeper_end:
    
    ret SP



/* piob address */
piob:
    .int AVR32_PIOB

/* pioc address */
pioc:
    .int AVR32_PIOC

/* intc address */
intc:
    .int AVR32_INTC

/******************************************************************************
 * 
 * Data segment: Includes all variables
 * 
 *****************************************************************************/
.data

