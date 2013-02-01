/******************************************************************************
 * 
 * Assignment 1: UCSysDes
 * 
 * Register conventions used:
 * r0-r3: Temporary data. Will likely be overidden by subroutines.
 * r4-r11: Important data. Subroutines should not modify these without pushing to stack!
 * r12: Parameter and return register
 * r13: Stack pointer
 * r14: Link register
 * r15: Program counter
 * 
 * Used registers:
 * r4 - piob pointer
 * r5 - pioc pointer
 * r6 - intc pointer
 * r8 - Current LED
 * r10 - Event button source
 * r11 - Event button state
 * 
 * Known issues:
 * If an interrupt is triggered (which causes a sleep) while a sleep is in
 * progress, the program will crash.
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
    
    /* Initialize */
    rcall init
    
    /* Start main program */
    rcall main



/* Initializes registers and components */
init:

    /* Load pointers */
    lddpc r4, piob
    lddpc r5, pioc
    lddpc r6, intc
    
    /* Initialize stack */
    lddpc sp, stack
    
    /* Load all elements */
    mov r0, E_ALL
    
    /* Enable all buttons */
    st.w r4[AVR32_PIO_PER], r0
    st.w r4[AVR32_PIO_PUER], r0
    
    /* Enable all LEDS */
    st.w r5[AVR32_PIO_PER], r0
    st.w r5[AVR32_PIO_OER], r0
    
    /* Enable button interrupts */
    st.w r4[AVR32_PIO_IER], r0
    com r0
    st.w r4[AVR32_PIO_IDR], r0
    
    /* For simplicity, set EVBA to 0 */
    mov r0, 0
    mtsr 4, r0
    
    /* Set button interrupt autovector */
    mov r0, button_interrupt
    st.w r6[AVR32_INTC_IPR14], r0
    
    /* Initialize with LED 0 */
    mov r8, E_0
    
    /* Disable Global Interupt Mask */
    csrf 16
    
    ret SP



/* Main program */
main:
    
    /* Set LEDS */
    mov r12, r8
    rcall set_leds
    
    /* Wait for interrupt */
    sleep 1
    
    main_button_0:
        
        /* Was the event related to button 0? If not then skip (mask event by button) */
        mov r0, E_0
        and r0, r10
        breq main_button_1
        
        /* Was the button pressed? If not then skip (mask state by button) */
        mov r0, E_0
        and r0, r11
        breq main_button_1
        
        /* Handle event */
        rcall cycle_led_right
        
    main_button_1:
    main_button_2:
        
        /* Was the event related to button 2? If not then skip (mask event by button) */
        mov r0, E_2
        and r0, r10
        breq main_button_3
        
        /* Was the button pressed? If not then skip (mask state by button) */
        mov r0, E_2
        and r0, r11
        breq main_button_3
        
        /* Handle event */
        rcall cycle_led_left
        
    main_button_3:
    main_button_4:
    main_button_5:
    main_button_6:
    main_button_7:
        
        /* Was the event related to button 7? If not then skip (mask event by button) */
        mov r0, E_7
        and r0, r10
        breq main_end
        
        /* Was the button pressed? If not then skip (mask state by button) */
        mov r0, E_7
        and r0, r11
        breq main_end
        
        /* Handle event */
        rcall flash_left
        
    main_end:
    
    /* Loop */
    rjmp main



/* Cycle LED one left */
cycle_led_left:
    
    /* Shift LED one left */
    lsl r8, 1
    
    /* If shifted beyond LED 7, set to LED 0 */
    cp.w r8, E_7
    movhi r8, E_0
    
    ret SP



/* Cycle LED one right */
cycle_led_right:
    
    /* Shift LED one right */
    lsr r8, 1
    
    /* If shifted beyond LED O, set to LED 7 */
    cp.w r8, E_0
    
    /* Workaround because E_7 is too large for mov{cond} (addlo crashes, therefore sublo) */
    movlo r8, E_7-1
    sublo r8, -1
    
    ret SP



/* Flash left */
flash_left:
    
    /* Backup registers */
    st.w --sp, r8
    st.w --sp, lr
    
    /* Time between each iteration */
    mov r1, 200000
    
    /* Number of iterations */
    mov r2, 64
    
    /* Starting LED */
    mov r8, E_0
    
    /* Clear LEDS */
    mov r12, 0
    rcall set_leds
    
    /* Sleep for a while */
    mov r12, r1
    rcall sleeper
    
    flash_left_start:
        
        /* Count down */
        sub r2, 1
        
        /* Check if done */
        cp.w r2, 0
        brlt flash_left_end
        
        /* Set LEDS */
        mov r12, r8
        rcall set_leds
        
        /* Sleep for a while */
        mov r12, r1
        rcall sleeper
        
        /* Cycle one left */
        rcall cycle_led_left
        
        /* Continue */
        rjmp flash_left_start
        
    flash_left_end:
    
    /* Restore registers */
    ld.w lr, sp++
    ld.w r8, sp++
    
    /* Restore LEDS */
    mov r12, r8
    rcall set_leds
    
    ret SP



/* Button interrupt handler */
button_interrupt:
    /* Backup registers (r0 is used by sleeper) */
    st.w --sp, r0
    st.w --sp, lr

    /* Debounce */
    mov r12, 1000
    rcall sleeper

    /* Read ISR to make sure it knows the interupt was handled */
    ld.w r10, r4[AVR32_PIO_ISR]
    
    /* Read button states */ 
    ld.w r11, r4[AVR32_PIO_PDSR]
    
    /* Invert button states so that down is 1 and up is 0 */
    com r11
    
    /* Restore registers */
    st.w lr, sp++
    ld.w r0, sp++

    /* Be lazy: Let the main loop handle the rest */
    rete



/* Clear all registers. Useful for debugging */
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



/* Enables and disables LEDS specified by r12 (1 means on, 0 means off) */
set_leds:
    
    /* Enable specified LEDS */
    st.w r5[AVR32_PIO_SODR], r12
    
    /* Disable other LEDS */
    com r12
    st.w r5[AVR32_PIO_CODR], r12
    
    ret SP



/* Sleeps for the duration of r12 */
sleeper:
    
    /* Set sleep time */
    mov r0, r12
    
    sleeper_start:
    
    /* Count down */
    sub r0, 1
    
    /* Check if done */
    cp.w r0, 0
    brlt sleeper_end
    
    /* Sleep more! */
    rjmp sleeper_start
    
    sleeper_end:
    
    ret SP



/* PIOB address */
piob:
    .int AVR32_PIOB

/* PIOC address */
pioc:
    .int AVR32_PIOC

/* INTC address */
intc:
    .int AVR32_INTC

/* Initial stack pointer address */
stack:
    .int _stack



/******************************************************************************
 * 
 * Data segment: Includes all variables
 * 
 *****************************************************************************/
.data



