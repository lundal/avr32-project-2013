CROSS_COMPILE = avr32-

CC = gcc
CFLAGS = -O2 -g -std=gnu99 -Wall -Wextra -Wpadded -Wpointer-arith \
	-Wno-unused-parameter

CPP = $(CC) -E
CPPFLAGS = -MMD -I.

CXX = g++
CXXFLAGS = $(CFLAGS)

AS = as
ASFLAGS =

LD = ld
LDFLAGS =
# -Wl,-Tavr32elf_ap7000.x
LDLIBS =
LOADLIBES =

AR = ar
ARFLAGS = cr

RANLIB = ranlib
RANLIBFLAGS =

ifeq ($(V),1)
TOOLCHAIN_QUIET = 
else
TOOLCHAIN_QUIET = @
endif

COMPILE.c = $(TOOLCHAIN_QUIET)echo 'CC      $@' ; \
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

COMPILE.cc = $(TOOLCHAIN_QUIET)echo 'CXX     $@' ; \
	$(CROSS_COMPILE)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

COMPILE.s = $(TOOLCHAIN_QUIET)echo 'AS      $@' ; \
	$(CROSS_COMPILE)$(AS) $(ASFLAGS) $(TARGET_MACH)

COMPILE.S = $(TOOLCHAIN_QUIET)echo 'AS      $@' ; \
	$(CROSS_COMPILE)$(CC) $(ASFLAGS) $(CPPFLAGS) $(TARGET_MACH) -c

LINK.o = $(TOOLCHAIN_QUIET)echo 'LD      $@' ; \
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(TARGET_ARCH)
TOOLCHAIN_LINK = $(LINK.o) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)
TOOLCHAIN_LINK.so = $(LINK.o) -shared $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

ARCHIVE.o = $(TOOLCHAIN_QUIET)echo 'AR      $%' ; \
	$(CROSS_COMPILE)$(AR) $(ARFLAGS)
TOOLCHAIN_ARCHIVE = $(ARCHIVE.o) $@ $%

(%): %
	$(TOOLCHAIN_ARCHIVE)

TOOLCHAIN_RANLIB = $(TOOLCHAIN_QUIET)echo 'RANLIB  $@' ; \
	$(CROSS_COMPILE)$(RANLIB) $(RANLIBFLAGS) $@

%.a: %.a()
	$(TOOLCHAIN_RANLIB)
libavr32b_interrupts_SRCS = \
	evba.S \
	interrupts.c \

libavr32b_interrupts_OBJS := \
	$(patsubst %.c,%.o,$(filter %.c,$(libavr32b_interrupts_SRCS))) \
	$(patsubst %.S,%.o,$(filter %.S,$(libavr32b_interrupts_SRCS))) \
	$(patsubst %.s,%.o,$(filter %.s,$(libavr32b_interrupts_SRCS))) \

libavr32b_interrupts_DEPS := \
	$(patsubst %.c,%.d,$(filter %.c,$(libavr32b_interrupts_SRCS))) \
	$(patsubst %.S,%.d,$(filter %.S,$(libavr32b_interrupts_SRCS))) \
	$(patsubst %.s,%.d,$(filter %.s,$(libavr32b_interrupts_SRCS))) \

test_SRCS = \
	test.c \

test_OBJS := $(patsubst %.c,%.o,$(test_SRCS))
test_DEPS := $(patsubst %.c,%.d,$(test_SRCS))


.PHONY: all
all: libavr32b-interrupts.a

test: $(test_OBJS) libavr32b-interrupts.a
	$(TOOLCHAIN_LINK)

libavr32b-interrupts.a: libavr32b-interrupts.a($(libavr32b_interrupts_OBJS))
	$(TOOLCHAIN_RANLIB)

-include $(libavr32b_interrupts_DEPS)

$(libavr32b_interrupts_OBJS): Makefile

.PHONY: clean
clean:
	$(RM) $(libavr32b_interrupts_OBJS) $(libavr32b_interrupts_DEPS) \
		libavr32b-interrupts.a
	$(RM) $(test_OBJS) $(test_DEPS) test

.PHONY: distclean
distclean:

DATE := $(shell date +%Y%m%d)
.PHONY: snapshot
snapshot:
	git archive --format=tar --prefix=avr32b-interrupts-$(DATE)/ HEAD | \
		gzip > avr32b-interrupts-$(DATE).tar.gz
