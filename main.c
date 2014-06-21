/*
 * 
 * Copyright (C) 2014 UGR (www.ugr.es)
 * Author: Miguel Jimenez Lopez
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
 
#include <stdio.h>
#include <inttypes.h>

#include <stdarg.h>

#include <sys.h>
#include "uart.h"
#include "shell.h"

static void lm32_initialize()
{

sdb_find_devices();
uart_init_sw();
uart_init_hw();

mprintf("LM32: starting up...\n");

}

int main(void)
{

lm32_initialize();

shell_init();

mprintf("LM32: shell initialized! \n");

for(;;) {
shell_interactive();
}

}
