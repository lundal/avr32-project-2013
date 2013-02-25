/*
 * C functions for libavr32b-interrupts - a library to
 * emulate an older version of avr32b framework.
 *
 * written by Ole Henrik Jahren <olehenja@alumni.ntnu.no>
 */

#include <stdint.h>
#include <sys/interrupts.h>
#include <avr32/ap7000.h>

#define IRQ_OFFSET_GROUP0 0
#define IRQ_OFFSET_GROUP1 (IRQ_OFFSET_GROUP0 + AVR32_INTC_NUM_IRQS_PER_GRP0)
#define IRQ_OFFSET_GROUP2 (IRQ_OFFSET_GROUP1 + AVR32_INTC_NUM_IRQS_PER_GRP1)
#define IRQ_OFFSET_GROUP3 (IRQ_OFFSET_GROUP2 + AVR32_INTC_NUM_IRQS_PER_GRP2)
#define IRQ_OFFSET_GROUP4 (IRQ_OFFSET_GROUP3 + AVR32_INTC_NUM_IRQS_PER_GRP3)
#define IRQ_OFFSET_GROUP5 (IRQ_OFFSET_GROUP4 + AVR32_INTC_NUM_IRQS_PER_GRP4)
#define IRQ_OFFSET_GROUP6 (IRQ_OFFSET_GROUP5 + AVR32_INTC_NUM_IRQS_PER_GRP5)
#define IRQ_OFFSET_GROUP7 (IRQ_OFFSET_GROUP6 + AVR32_INTC_NUM_IRQS_PER_GRP6)
#define IRQ_OFFSET_GROUP8 (IRQ_OFFSET_GROUP7 + AVR32_INTC_NUM_IRQS_PER_GRP7)
#define IRQ_OFFSET_GROUP9 (IRQ_OFFSET_GROUP8 + AVR32_INTC_NUM_IRQS_PER_GRP8)
#define IRQ_OFFSET_GROUP10 (IRQ_OFFSET_GROUP9 + AVR32_INTC_NUM_IRQS_PER_GRP9)
#define IRQ_OFFSET_GROUP11 (IRQ_OFFSET_GROUP10 + AVR32_INTC_NUM_IRQS_PER_GRP10)
#define IRQ_OFFSET_GROUP12 (IRQ_OFFSET_GROUP11 + AVR32_INTC_NUM_IRQS_PER_GRP11)
#define IRQ_OFFSET_GROUP13 (IRQ_OFFSET_GROUP12 + AVR32_INTC_NUM_IRQS_PER_GRP12)
#define IRQ_OFFSET_GROUP14 (IRQ_OFFSET_GROUP13 + AVR32_INTC_NUM_IRQS_PER_GRP13)
#define IRQ_OFFSET_GROUP15 (IRQ_OFFSET_GROUP14 + AVR32_INTC_NUM_IRQS_PER_GRP14)
#define IRQ_OFFSET_GROUP16 (IRQ_OFFSET_GROUP15 + AVR32_INTC_NUM_IRQS_PER_GRP15)
#define IRQ_OFFSET_GROUP17 (IRQ_OFFSET_GROUP16 + AVR32_INTC_NUM_IRQS_PER_GRP16)
#define IRQ_OFFSET_GROUP18 (IRQ_OFFSET_GROUP17 + AVR32_INTC_NUM_IRQS_PER_GRP17)
#define IRQ_OFFSET_GROUP19 (IRQ_OFFSET_GROUP18 + AVR32_INTC_NUM_IRQS_PER_GRP18)
#define IRQ_OFFSET_GROUP20 (IRQ_OFFSET_GROUP19 + AVR32_INTC_NUM_IRQS_PER_GRP19)
#define IRQ_OFFSET_GROUP21 (IRQ_OFFSET_GROUP20 + AVR32_INTC_NUM_IRQS_PER_GRP20)
#define IRQ_OFFSET_GROUP22 (IRQ_OFFSET_GROUP21 + AVR32_INTC_NUM_IRQS_PER_GRP21)
#define IRQ_OFFSET_GROUP23 (IRQ_OFFSET_GROUP22 + AVR32_INTC_NUM_IRQS_PER_GRP22)
#define IRQ_OFFSET_GROUP24 (IRQ_OFFSET_GROUP23 + AVR32_INTC_NUM_IRQS_PER_GRP23)
#define IRQ_OFFSET_GROUP25 (IRQ_OFFSET_GROUP24 + AVR32_INTC_NUM_IRQS_PER_GRP24)
#define IRQ_OFFSET_GROUP26 (IRQ_OFFSET_GROUP25 + AVR32_INTC_NUM_IRQS_PER_GRP25)
#define IRQ_OFFSET_GROUP27 (IRQ_OFFSET_GROUP26 + AVR32_INTC_NUM_IRQS_PER_GRP26)
#define IRQ_OFFSET_GROUP28 (IRQ_OFFSET_GROUP27 + AVR32_INTC_NUM_IRQS_PER_GRP27)
#define IRQ_OFFSET_GROUP29 (IRQ_OFFSET_GROUP28 + AVR32_INTC_NUM_IRQS_PER_GRP28)
#define IRQ_OFFSET_GROUP30 (IRQ_OFFSET_GROUP29 + AVR32_INTC_NUM_IRQS_PER_GRP29)
#define IRQ_OFFSET_GROUP31 (IRQ_OFFSET_GROUP30 + AVR32_INTC_NUM_IRQS_PER_GRP30)
#define IRQ_OFFSET_GROUP32 (IRQ_OFFSET_GROUP31 + AVR32_INTC_NUM_IRQS_PER_GRP31)
#define IRQ_COUNT (IRQ_OFFSET_GROUP32 + AVR32_INTC_NUM_IRQS_PER_GRP32)

static const uint8_t irq_offset_lookup[AVR32_INTC_NUM_INT_GRPS] =
{
	IRQ_OFFSET_GROUP0,
	IRQ_OFFSET_GROUP1,
	IRQ_OFFSET_GROUP2,
	IRQ_OFFSET_GROUP3,
	IRQ_OFFSET_GROUP4,
	IRQ_OFFSET_GROUP5,
	IRQ_OFFSET_GROUP6,
	IRQ_OFFSET_GROUP7,
	IRQ_OFFSET_GROUP8,
	IRQ_OFFSET_GROUP9,
	IRQ_OFFSET_GROUP10,
	IRQ_OFFSET_GROUP11,
	IRQ_OFFSET_GROUP12,
	IRQ_OFFSET_GROUP13,
	IRQ_OFFSET_GROUP14,
	IRQ_OFFSET_GROUP15,
	IRQ_OFFSET_GROUP16,
	IRQ_OFFSET_GROUP17,
	IRQ_OFFSET_GROUP18,
	IRQ_OFFSET_GROUP19,
	IRQ_OFFSET_GROUP20,
	IRQ_OFFSET_GROUP21,
	IRQ_OFFSET_GROUP22,
	IRQ_OFFSET_GROUP23,
	IRQ_OFFSET_GROUP24,
	IRQ_OFFSET_GROUP25,
	IRQ_OFFSET_GROUP26,
	IRQ_OFFSET_GROUP27,
	IRQ_OFFSET_GROUP28,
	IRQ_OFFSET_GROUP29,
	IRQ_OFFSET_GROUP30,
	IRQ_OFFSET_GROUP31,
	IRQ_OFFSET_GROUP32,
};

static const uint8_t line_count_lookup[AVR32_INTC_NUM_INT_GRPS] =
{
	AVR32_INTC_NUM_IRQS_PER_GRP0,
	AVR32_INTC_NUM_IRQS_PER_GRP1,
	AVR32_INTC_NUM_IRQS_PER_GRP2,
	AVR32_INTC_NUM_IRQS_PER_GRP3,
	AVR32_INTC_NUM_IRQS_PER_GRP4,
	AVR32_INTC_NUM_IRQS_PER_GRP5,
	AVR32_INTC_NUM_IRQS_PER_GRP6,
	AVR32_INTC_NUM_IRQS_PER_GRP7,
	AVR32_INTC_NUM_IRQS_PER_GRP8,
	AVR32_INTC_NUM_IRQS_PER_GRP9,
	AVR32_INTC_NUM_IRQS_PER_GRP10,
	AVR32_INTC_NUM_IRQS_PER_GRP11,
	AVR32_INTC_NUM_IRQS_PER_GRP12,
	AVR32_INTC_NUM_IRQS_PER_GRP13,
	AVR32_INTC_NUM_IRQS_PER_GRP14,
	AVR32_INTC_NUM_IRQS_PER_GRP15,
	AVR32_INTC_NUM_IRQS_PER_GRP16,
	AVR32_INTC_NUM_IRQS_PER_GRP17,
	AVR32_INTC_NUM_IRQS_PER_GRP18,
	AVR32_INTC_NUM_IRQS_PER_GRP19,
	AVR32_INTC_NUM_IRQS_PER_GRP20,
	AVR32_INTC_NUM_IRQS_PER_GRP21,
	AVR32_INTC_NUM_IRQS_PER_GRP22,
	AVR32_INTC_NUM_IRQS_PER_GRP23,
	AVR32_INTC_NUM_IRQS_PER_GRP24,
	AVR32_INTC_NUM_IRQS_PER_GRP25,
	AVR32_INTC_NUM_IRQS_PER_GRP26,
	AVR32_INTC_NUM_IRQS_PER_GRP27,
	AVR32_INTC_NUM_IRQS_PER_GRP28,
	AVR32_INTC_NUM_IRQS_PER_GRP29,
	AVR32_INTC_NUM_IRQS_PER_GRP30,
	AVR32_INTC_NUM_IRQS_PER_GRP31,
	AVR32_INTC_NUM_IRQS_PER_GRP32,
};

extern void _irq_level0(void);
extern void _irq_level1(void);
extern void _irq_level2(void);
extern void _irq_level3(void);
extern void _evba();

static const uintptr_t irq_level_lookup[] =
{
	(uintptr_t)&_irq_level0,
	(uintptr_t)&_irq_level1,
	(uintptr_t)&_irq_level2,
	(uintptr_t)&_irq_level3,
};

static volatile struct avr32_intc_t *intc = \
	(volatile struct avr32_intc_t *)AVR32_INTC_ADDRESS;

static __int_handler irq_handler_lookup[IRQ_COUNT];

__int_handler
register_interrupt(__int_handler handler, int group,
		int line, int priority)
{
	unsigned int offset;

	if (group >= AVR32_INTC_NUM_INT_GRPS)
		panic();
	offset = irq_offset_lookup[group] + line;
	if (offset >= IRQ_COUNT)
		panic();
	irq_handler_lookup[offset] = handler;
	if (priority > 3)
		panic();
	intc->ipr[group] = (((unsigned int)priority) << 30)|
		(irq_level_lookup[priority] - (uintptr_t)_evba);
	return handler;
}

void
_do_irq(unsigned int level)
{
	unsigned int group;

	switch (level)
	{
	case 0:
		group = intc->icr0;
		break;
	case 1:
		group = intc->icr1;
		break;
	case 2:
		group = intc->icr2;
		break;
	case 3:
		group = intc->icr3;
		break;
	default:
		group = 65;
		panic();
	}

	if (group >= AVR32_INTC_NUM_INT_GRPS)
		panic();

	for (uint32_t line = 0; line < line_count_lookup[group]; ++line)
	{
		if ((intc->irr[group] & (1 << line)))
		{
			unsigned int offset;

			offset = irq_offset_lookup[group] + line;

			if (offset >= IRQ_COUNT)
				panic();
			irq_handler_lookup[offset]();
		}
	}
}

void
set_interrupts_base(void *addr)
{
	intc = (volatile struct avr32_intc_t *)addr;
}

void
init_interrupts(void)
{
	unsigned long sr;
	__builtin_mtsr(AVR32_EVBA, (uintptr_t)&_evba);

	for (unsigned long i = 0; i < AVR32_INTC_NUM_INT_GRPS; ++i)
	{
		unsigned long ipr, prio;
		ipr = intc->ipr[i];
		prio = (ipr >> 30) & 0x3;
		
		intc->ipr[i] = (prio << 30)|
			(irq_level_lookup[prio] - (uintptr_t)_evba);
	}
	sr = __builtin_mfsr(AVR32_SR);
	sr &= ~((1 << AVR32_SR_I0M)|
		(1 << AVR32_SR_I1M)|
		(1 << AVR32_SR_I2M)|
		(1 << AVR32_SR_I3M));
	__builtin_mtsr(AVR32_SR, sr);
	sr &= ~(1 << AVR32_SR_GM);
	__builtin_mtsr(AVR32_SR, sr);
}

void
panic()
{
	while (1)
	{
		__builtin_breakpoint();
	}
}
