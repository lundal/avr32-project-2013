###############################################################################
#
# Excercise 3 UCSysDes Makefile
#
###############################################################################

# Compiler
CC = avr32-linux-gcc
CCFLAGS = -Wall -g -O3 -c
LDFLAGS = -Wall -g -pedantic -lm

# File names 
SRC = bmp.c graphics.c font.c engine.c component.c engine_test.c
OBJ = $(SRC:.c=.o)
EXE = game.out

###############################################################################

# Link executable from object files
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compile object files from code
.c.o:
	$(CC) $(CCFLAGS) $< -o $@

# Remove all generated files
.PHONY: clean
clean:
	rm -rf *.o *.out
