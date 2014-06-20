/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2011 CERN (www.cern.ch)
 * Author: Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
#include "irq.h"

void _irq_entry() {}

void disable_irq()
{
	unsigned int ie, im;
	unsigned int Mask = ~1;

	/* disable peripheral interrupts in case they were enabled */
	asm volatile ("rcsr %0,ie":"=r" (ie));
	ie &= (~0x1);
	asm volatile ("wcsr ie, %0"::"r" (ie));

	/* disable mask-bit in im */
	asm volatile ("rcsr %0, im":"=r" (im));
	im &= Mask;
	asm volatile ("wcsr im, %0"::"r" (im));

}

void enable_irq()
{
	unsigned int ie, im;
	unsigned int Mask = 1;

	/* disable peripheral interrupts in-case they were enabled */
	asm volatile ("rcsr %0,ie":"=r" (ie));
	ie &= (~0x1);
	asm volatile ("wcsr ie, %0"::"r" (ie));

	/* enable mask-bit in im */
	asm volatile ("rcsr %0, im":"=r" (im));
	im |= Mask;
	asm volatile ("wcsr im, %0"::"r" (im));

	ie |= 0x1;
	asm volatile ("wcsr ie, %0"::"r" (ie));
}
