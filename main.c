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
#include <i2c.h>
#include <syscon.h>
#include "uart.h"
#include "shell.h"

int32_t sfp_alpha[SFP_PORTS] = {73622176, 73622176};
int32_t sfp_deltaTx[SFP_PORTS] = {46407, 46407};
int32_t sfp_deltaRx[SFP_PORTS] = {167843, 167843};
uint32_t cal_phase_transition[SFP_PORTS] = {2389, 2389};

static void lm32_initialize()
{
	
unsigned char d;

sdb_find_devices();
uart_init_sw();
uart_init_hw();

timer_init(1);

mprintf("WRS-Z16: starting up...\n");

}

int main(void)
{

lm32_initialize();

shell_init();

mprintf("WRS-Z16: shell initialized! \n");

for(;;) {
shell_interactive();
}

}
