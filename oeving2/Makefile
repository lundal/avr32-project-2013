###############################################################################
#
# Excercise 2 UCSysDes Makefile
#
###############################################################################

# Compiler
CC = avr32-gcc
CCFLAGS = -Wall -g -Iinclude -O3 -c
LDFLAGS = -Wall -g -pedantic -Llib -lavr32b-interrupts -lm

# AVR32 programs
AVR32PROGRAM = avr32program
AVR32DEBUG = avr32-gdb

# File names 
SRC = all_in_one.c
#SRC = oeving2.c sound.c tones.c track.c
OBJ = $(SRC:.c=.o)
EXE = oeving2.elf

###############################################################################

# Link ELF from object files
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compile object files from code
.c.o:
	$(CC) $(CCFLAGS) $< -o $@

# Upload to STK1000
.PHONY: upload
upload: $(EXE)
	$(AVR32PROGRAM) halt
	sleep 3
	$(AVR32PROGRAM) program -e -f0,8Mb $<

# Start debugger
.PHONY: debug
debug: upload
	$(AVR32DEBUG)

# Remove all generated files
.PHONY: clean
clean:
	rm -rf *.o *.elf
