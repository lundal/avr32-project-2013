# AVR32 code project

This project was a group project as part of a course in [microcontroller system design](http://www.idi.ntnu.no/emner/tdt4258/start) 
(renamed Energy Efficient Computer Systems).

We were all given an STK1000 development board with an AVR32 processor for the occasion.

The project had three parts, all of which had seperate goals and report.

## Part 1
In the first part of the problem we had to control the LEDs with the buttons using assembly and interrupt. The requirements
were as follows:

> Write an assembly program which enables a user to move a \paddle" to the left
or to the right on the row of LED{diodes by pressing buttons on the STK1000
board. The paddle can be represented as a single lighted spot (one LED{diode)
and by pressing button 0 the lighted spot moves to the right while by pressing
button 2, it moves to the left.
It is required that you write an interrupt routine (an interrupt handler) for
reading the buttons, while the LED{diodes must be updated in the main loop of
the program. It is also a requirement that you use a Makele for the assignment
and you can debug the program with GDB through JTAGICE.

We ended up implementing some extra features that makes the LED's do some fancy moves.

## Part 2
The requirements for this exercise was:

> Write a C-program which runs directly on the STK1000 board (without support
of an operating system) and which plays different sound effects when different
buttons are pressed. [...] You have to make at least three different sound effects
[...] We recommend that you use the microcontroller's internal ABDAC. [...] 
The requirement is that you use an interrupt routine to pass the samples to
the ABDAC.

So we implemented a MIDI player! :)

## Part 3

In this final part it all comes together as a game. The formal requirements were:

> Make a driver for the use of buttons and LEDs on the STK1000. It should be
implemented as a kernel module. You are free to make a driver as you wish, but
the minimum requirements are to support your needs for the game to work. [...]
Complete the game. Use /dev/fb0 directly for writing to LCD screen. Use
/dev/dsp for producing the sound. Use your own driver for reading the status
of the buttons on STK1000. Use also your own driver for the control of the LED
diodes. These can be used, for example, to show how many lives a player has
left or some other status information about the game. Or you can just blink in
some nice repetitive way.

We used most of the time implementing a modular component based game engine. For more details,
please refer to the report in the reports folder.
