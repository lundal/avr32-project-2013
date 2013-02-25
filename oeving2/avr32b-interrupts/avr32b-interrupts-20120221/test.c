#include <sys/interrupts.h>

#include <avr32/ap7000.h>

static void
test_interrupt(void)
{
}

int
main(int argc, char **argv)
{
	set_interrupts_base((void *)AVR32_INTC_ADDRESS);

	register_interrupt(test_interrupt, 14, 1, 0);

	init_interrupts();

	while (1)
	{
	}
	return 0;
}
