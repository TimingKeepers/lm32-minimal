#include <stdlib.h>     /* strtol */
#include <sys.h>
#include <shell.h>
#include "i2c.h"
#include <string.h>

static uint8_t i2c_switch_idx = 0;

static inline int i2csw_write(uint8_t data)
{
	return write_i2c_switch(0,i2c_sw_addrs[i2c_switch_idx],data);
}

static inline int i2csw_read()
{
	return read_i2c_switch(0,i2c_sw_addrs[i2c_switch_idx]);
}

static inline int i2csw_select(int idx)
{
	int r = 0;
	int i;
	
	if(idx < 0 || idx >= I2C_SWITCH_N) {
		mprintf("Error: I2c index must be: \n");
		r = -1;
		
		for(i = 0 ; i < I2C_SWITCH_N ; i++)
			mprintf("idx: %d, addr: 0x%x \n",i,i2c_sw_addrs[i]);
		
	} else {
		i2c_switch_idx = idx;
		mprintf("Select I2C mux %d (addr: 0x%x) \n",idx,i2c_sw_addrs[idx]);
	}
	
	return r;
		
}

static inline int i2csw_list()
{
	int r = 0;
	int i;	
	
	mprintf("I2C Switches: \n");
	for(i = 0 ; i < I2C_SWITCH_N ; i++)
			mprintf("idx: %d, addr: 0x%x \n",i,i2c_sw_addrs[i]);
		
	
	return r;
		
}

static void decode_hex(const char *str, uint8_t *value)
{
	*value = (uint8_t)strtol(str, NULL, 0);
};


static int cmd_i2csw(const char *args[])
	{

	uint8_t data;
	uint8_t data2;
	int idx;

	if (!strcasecmp(args[0], "read")) {
		data = i2csw_read();

		if(data >= 0) 
			mprintf("I2C Switch configuration: 0x%x \n", data);
		else
			mprintf("I2C Switch read error \n");
	
	} else {
		if (!strcasecmp(args[0], "write") && args[1]) {
			decode_hex(args[1], &data);

			if (i2csw_write(data) != 0)
				mprintf("I2C Switch write error \n");

			data2 = i2csw_read();

			if(data != data2){
				mprintf("Error writing data: \n");
				mprintf("Expected: %x \n", data);
				mprintf("Found: %x \n", data2);
			}      
		} else {
			if (!strcasecmp(args[0], "select") && args[1]) {
				fromdec(args[1], &idx);
				i2csw_select(idx);
			}
			else {
				if (!strcasecmp(args[0], "list"))
					i2csw_list();
				else {
					mprintf("Wrong wb call! \n");
					mprintf(">i2csw read \n");
					mprintf(">i2csw write <0xdata>\n");
					mprintf(">i2csw select <index>\n");
					mprintf(">i2csw list\n");
				}
			}
		}
	}
   
   return 0;
};


DEFINE_COMMAND(i2csw) = {
	.name = "i2csw",
	.exec = cmd_i2csw,
};
