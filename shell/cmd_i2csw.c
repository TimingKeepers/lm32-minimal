#include <stdlib.h>     /* strtol */
#include <sys.h>
#include <shell.h>
#include "i2c.h"
#include <string.h>

static inline int i2csw_write(uint8_t data)
{
	return write_i2c_switch(0,I2C_SWITCH_ADDR,data);
}

static inline int i2csw_read()
{
	return read_i2c_switch(0,I2C_SWITCH_ADDR);
}

static void decode_hex(const char *str, uint8_t *value)
{
	*value = (uint8_t)strtol(str, NULL, 0);
};


static int cmd_i2csw(const char *args[])
{

  uint8_t data;
  uint8_t data2;

  if (!strcasecmp(args[0], "read")) {
      data = i2csw_read();
      
      if(data >= 0) 
      	mprintf("I2C Switch configuration: 0x%x \n", data);
      else
	mprintf("I2C Switch read error \n");
	
	} else if (!strcasecmp(args[0], "write") && args[1]) {
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
     mprintf("Wrong wb call! \n");
     mprintf(">i2csw read \n");
     mprintf(">i2csw write <0xdata>\n");
   }
   
   return 0;
};


DEFINE_COMMAND(i2csw) = {
	.name = "i2csw",
	.exec = cmd_i2csw,
};
