/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2012 CERN (www.cern.ch)
 * Author: Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
/* SFP Detection / managenent functions */

#include <stdio.h>
#include <inttypes.h>

#include "syscon.h"
#include "i2c.h"
#include "sfp.h"

int sfp_present(int port)
{
	if(port == 0)
		return !gpio_in(GPIO_SFP0_DET);
	else if(port==1)
		return !gpio_in(GPIO_SFP1_DET);
	
	return 0;
}

int sfp_read_part_id(char *part_id, int port)
{
	int i;
	uint8_t data = 0, sum = 0;
	unsigned char sw_conf, sw_conf_dfl;

	if(port==0) {

	sw_conf_dfl = (unsigned char) read_i2c_switch(SFP0_I2C,i2c_sw_addrs[I2C_SWITCH2_SFP8_15]);
	//sw_conf = sw_conf_dfl | 0x40;
	sw_conf = 0x40;
	configure_i2c_switch(SFP0_I2C,i2c_sw_addrs[I2C_SWITCH2_SFP8_15],sw_conf);
	
	mi2c_init(SFP0_I2C);

	mi2c_start(SFP0_I2C);
	mi2c_put_byte(SFP0_I2C, 0xA0);
	mi2c_put_byte(SFP0_I2C, 0x00);
	mi2c_repeat_start(SFP0_I2C);
	mi2c_put_byte(SFP0_I2C, 0xA1);
	mi2c_get_byte(SFP0_I2C, &data, 1);
	mi2c_stop(SFP0_I2C);

	sum = data;

	mi2c_start(SFP0_I2C);
	mi2c_put_byte(SFP0_I2C, 0xA1);
	for (i = 1; i < 63; ++i) {
		mi2c_get_byte(SFP0_I2C, &data, 0);
		sum = (uint8_t) ((uint16_t) sum + data) & 0xff;
		if (i >= 40 && i <= 55) {	//Part Number 
			part_id[i - 40] = data;
		}

	}
	mi2c_get_byte(SFP0_I2C, &data, 1);	//final word, checksum
	mi2c_stop(SFP0_I2C);
		
	//sw_conf &= ~0x40;
	sw_conf = sw_conf_dfl;
	configure_i2c_switch(SFP0_I2C,i2c_sw_addrs[I2C_SWITCH2_SFP8_15],sw_conf);


	} else if (port == 1){
		
		sw_conf_dfl = (unsigned char) read_i2c_switch(SFP1_I2C,i2c_sw_addrs[I2C_SWITCH2_SFP8_15]);
		//sw_conf = sw_conf_dfl | 0x80;
		sw_conf = 0x80;
		configure_i2c_switch(SFP1_I2C,i2c_sw_addrs[I2C_SWITCH2_SFP8_15],sw_conf);

		mi2c_init(SFP1_I2C);

		mi2c_start(SFP1_I2C);
		mi2c_put_byte(SFP1_I2C, 0xA0);
		mi2c_put_byte(SFP1_I2C, 0x00);
		mi2c_repeat_start(SFP1_I2C);
		mi2c_put_byte(SFP1_I2C, 0xA1);
		mi2c_get_byte(SFP1_I2C, &data, 1);
		mi2c_stop(SFP1_I2C);

		sum = data;

		mi2c_start(SFP1_I2C);
		mi2c_put_byte(SFP1_I2C, 0xA1);
		for (i = 1; i < 63; ++i) {
			mi2c_get_byte(SFP1_I2C, &data, 0);
			sum = (uint8_t) ((uint16_t) sum + data) & 0xff;
			if (i >= 40 && i <= 55) //Part Number
				part_id[i - 40] = data;
		}
		mi2c_get_byte(SFP1_I2C, &data, 1);	//final word, checksum
		mi2c_stop(SFP1_I2C);
	
		//sw_conf &= ~0x80;
		sw_conf = sw_conf_dfl;
		configure_i2c_switch(SFP1_I2C,i2c_sw_addrs[I2C_SWITCH2_SFP8_15],sw_conf);
	
	}

	if (sum == data)
		return 0;

	return -1;
}
