/*
 * 
 * Copyright (C) 2014 UGR (www.ugr.es)
 * Author: Miguel Jimenez Lopez
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
 
#include "include/sys.h"

static void lm32_initialize()
{

sdb_find_devices();
uart_init_sw();
uart_init_hw();

mprintf("LM32: starting up...\n");

}

int main() {
	lm32_initialize();
	mprintf("LM32: Hello, world!\n");
	return 0;
}
