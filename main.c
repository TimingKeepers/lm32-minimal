/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2011,2012 CERN (www.cern.ch)
 * Copyright (C) 2014, UGR (www.ugr.es)
 * Author: Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 * Author: Grzegorz Daniluk <grzegorz.daniluk@cern.ch>
 * Author: Miguel Jimenez Lopez <klyone@ugr.es>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
#include <stdio.h>
#include <inttypes.h>

#include <stdarg.h>

#include <sys.h>
#include "uart.h"
#include "shell.h"

static void obp_initialize()
{

	sdb_find_devices();
	uart_init_sw();
	uart_init_hw();

	mprintf("OBP: starting up...\n");

}

static void print_found_devices() {
	mprintf("DEVICE: ADDR_1B / ADDR_4B \n\n");
	mprintf("MINIC: 0x%x / 0x%x \n",(unsigned int)BASE_MINIC, (unsigned int)BASE_MINIC/4);
	mprintf("EP: 0x%x / 0x%x \n",(unsigned int)BASE_EP, (unsigned int)BASE_EP/4);
	mprintf("SOFTPLL: 0x%x / 0x%x \n",(unsigned int)BASE_SOFTPLL, (unsigned int)BASE_SOFTPLL/4);
	mprintf("PPS_GEN: 0x%x / 0x%x \n",(unsigned int)BASE_PPS_GEN, (unsigned int)BASE_PPS_GEN/4);
	mprintf("SYSCON: 0x%x / 0x%x \n",(unsigned int)BASE_SYSCON, (unsigned int)BASE_SYSCON/4);
	mprintf("UART: 0x%x / 0x%x \n",(unsigned int)BASE_UART, (unsigned int)BASE_UART/4);
	mprintf("ONEWIRE: 0x%x / 0x%x \n",(unsigned int)BASE_ONEWIRE, (unsigned int)BASE_ONEWIRE/4);
	mprintf("ETHERBONE_CFG: 0x%x / 0x%x \n",(unsigned int)BASE_ETHERBONE_CFG, (unsigned int)BASE_ETHERBONE_CFG/4);
	mprintf("WRPC_RAM: 0x%x / 0x%x \n",(unsigned int)BASE_WRPC_RAM, (unsigned int)BASE_WRPC_RAM/4);
}

int main(void)
{

obp_initialize();

mprintf("OBP: Found devices: \n");

print_found_devices();

shell_init();

mprintf("OBP: shell initialized! \n");

for(;;) {
	shell_interactive();
}

}

