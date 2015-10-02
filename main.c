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

/*initialize I2C bus*/
mi2c_init(FMC_I2C);

/* INIT PCA9548A  */
d = read_i2c_switch(FMC_I2C,I2C_SWITCH_ADDR);

if (d >= 0)
	mprintf("I2C Switch initial configuration: 0x%x!!\n",(unsigned char) d);

if (configure_i2c_switch(FMC_I2C,I2C_SWITCH_ADDR,I2C_SWITCH_DFL_CFG) != 0)
	mprintf("Error: I2C Switch could not be configured properly \n");
else {
	d = read_i2c_switch(FMC_I2C,I2C_SWITCH_ADDR);

	if (d >= 0)
		mprintf("I2C Switch configuration updated: 0x%x!!\n",(unsigned char) d);
}

eeprom_present(FMC_I2C, FMC_EEPROM_ADR);

mprintf("Zen: starting up...\n");

}

int main(void)
{

lm32_initialize();

shell_init();

mprintf("Zen: shell initialized! \n");

for(;;) {
shell_interactive();
}

}
