/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2011,2012 CERN (www.cern.ch)
 * Author: Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 * Author: Grzegorz Daniluk <grzegorz.daniluk@cern.ch>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
#include "inttypes.h"
#include "sys.h"
#include "syscon.h"

#define I2C_DELAY 300

void mi2c_delay()
{
	int i;
	for (i = 0; i < I2C_DELAY; i++)
		asm volatile ("nop");
}

#define M_SDA_OUT(i, x) { gpio_out(i2c_if[i].sda, x); mi2c_delay(); }
#define M_SCL_OUT(i, x) { gpio_out(i2c_if[i].scl, x); mi2c_delay(); }
#define M_SDA_IN(i) gpio_in(i2c_if[i].sda)

void mi2c_start(uint8_t i2cif)
{
	M_SDA_OUT(i2cif, 0);
	M_SCL_OUT(i2cif, 0);
}

void mi2c_repeat_start(uint8_t i2cif)
{
	M_SDA_OUT(i2cif, 1);
	M_SCL_OUT(i2cif, 1);
	M_SDA_OUT(i2cif, 0);
	M_SCL_OUT(i2cif, 0);
}

void mi2c_stop(uint8_t i2cif)
{
	M_SDA_OUT(i2cif, 0);
	M_SCL_OUT(i2cif, 1);
	M_SDA_OUT(i2cif, 1);
}

unsigned char mi2c_put_byte(uint8_t i2cif, unsigned char data)
{
	char i;
	unsigned char ack;

	for (i = 0; i < 8; i++, data <<= 1) {
		M_SDA_OUT(i2cif, data & 0x80);
		M_SCL_OUT(i2cif, 1);
		M_SCL_OUT(i2cif, 0);
	}

	M_SDA_OUT(i2cif, 1);
	M_SCL_OUT(i2cif, 1);

	ack = M_SDA_IN(i2cif);	/* ack: sda is pulled low ->success.     */

	M_SCL_OUT(i2cif, 0);
	M_SDA_OUT(i2cif, 0);

	return ack != 0;
}

void mi2c_get_byte(uint8_t i2cif, unsigned char *data, uint8_t last)
{

	int i;
	unsigned char indata = 0;

	M_SDA_OUT(i2cif, 1);
	/* assert: scl is low */
	M_SCL_OUT(i2cif, 0);

	for (i = 0; i < 8; i++) {
		M_SCL_OUT(i2cif, 1);
		indata <<= 1;
		if (M_SDA_IN(i2cif))
			indata |= 0x01;
		M_SCL_OUT(i2cif, 0);
	}

	if (last) {
		M_SDA_OUT(i2cif, 1);	//noack
		M_SCL_OUT(i2cif, 1);
		M_SCL_OUT(i2cif, 0);
	} else {
		M_SDA_OUT(i2cif, 0);	//ack
		M_SCL_OUT(i2cif, 1);
		M_SCL_OUT(i2cif, 0);
	}

	*data = indata;
}

void mi2c_init(uint8_t i2cif)
{
	M_SCL_OUT(i2cif, 1);
	M_SDA_OUT(i2cif, 1);
}

uint8_t mi2c_devprobe(uint8_t i2cif, uint8_t i2c_addr)
{
	uint8_t ret;
	mi2c_start(i2cif);
	ret = !mi2c_put_byte(i2cif, i2c_addr << 1);
	mi2c_stop(i2cif);

	return ret;
}

int read_i2c_switch(uint8_t i2cif,uint8_t sw_addr) {
	unsigned char d;
	uint8_t sw_read = (sw_addr<<1) | 0x01;
	
	mi2c_start(i2cif);

	if ((mi2c_put_byte(i2cif, sw_read)) < 0) {
		mi2c_stop(i2cif);
		mprintf("Error to read the I2C Switch.\n");
		return -1;
	}

	mi2c_get_byte(i2cif,&d,1);

	mi2c_stop(i2cif);

	return d;
}

int write_i2c_switch(uint8_t i2cif,uint8_t sw_addr, uint8_t value) {
	uint8_t sw_write = (sw_addr<<1);

	mi2c_start(i2cif);

	if ((mi2c_put_byte(i2cif, sw_write)) < 0) {
		mi2c_stop(i2cif);
		mprintf("Error to address the I2C Switch.\n");
		return -1;
	}

	if((mi2c_put_byte(i2cif, value)) < 0) {
		mi2c_stop(i2cif);
		mprintf("Error to update the I2C Switch,\n");
		return -2;
	}

	mi2c_stop(i2cif);

	return 0;
}

int configure_i2c_switch(uint8_t i2cif,uint8_t sw_addr, uint8_t sw_config) {
	int d;

	d = read_i2c_switch(i2cif,sw_addr);

	if (d < 0)
		return -1;

	d = write_i2c_switch(i2cif,sw_addr,sw_config);

	if (d < 0)
		return -2;

	d = read_i2c_switch(i2cif,sw_addr);

	if (d < 0)
		return -3;

	return 0;
}

//void mi2c_scan(uint8_t i2cif)
//{
//    int i;
//
//    //for(i=0;i<0x80;i++)
//    for(i=0x50;i<0x51;i++)
//    {
//     mi2c_start(i2cif);
//     if(!mi2c_put_byte(i2cif, i<<1)) mprintf("found : %x\n", i);
//     mi2c_stop(i2cif);
//
//    }
//    mprintf("Nothing more found...\n");
//}
