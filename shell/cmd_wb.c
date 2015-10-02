/*
Command to read/write from/to the WR-LEN wishbone registers
eml. emilio<AT>sevensols.com
*/

#include <stdlib.h>     /* strtol */
#include "shell.h"
#include <inttypes.h>
#include <sys.h>
#include <string.h>

static inline void wb_write(uint32_t reg, uint32_t data)
{
	*(volatile uint32_t *)(reg) = data;		//Instruction to write into the physical memory
}

static inline uint32_t wb_read(uint32_t reg)
{
	return *(volatile uint32_t *)(reg);		//Instruction to read from the physical memory
}

static void decode_hex(const char *str, uint32_t *value)
{
	unsigned long long_value= strtoul(str, NULL, 0);
	*value = (uint32_t)long_value;
};


static int cmd_wb(const char *args[])
{

  uint32_t address;
  uint32_t data;
  //uint32_t data2;

  if (!strcasecmp(args[0], "read") && args[1]) {
      /* Read WB register */
      decode_hex(args[1],&address);
      data = wb_read(address); 
      pp_printf("0x%08x \n", data);
	
	} else if (!strcasecmp(args[0], "write") && args[1] && args[2]) {
		/* Write WB resgister */
      decode_hex(args[1], &address);
      decode_hex(args[2], &data);
      wb_write(address, data);

/*
      if(0){ // The data is not checked after the writing.
		  data2 = wb_read(address);

		  if(data != data2){
			mprintf("Error writing data: \n");
			mprintf("Expected: %08x \n", data);
			mprintf("Found: %08x \n", data2);
		  }
      } */

   } else {
     pp_printf("Wrong wb call! \n");
     pp_printf(">wb read <0xaddress> \n");
     pp_printf(">wb write <0xaddress> <0xdata>\n");
   }

  return 0;
};


DEFINE_COMMAND(wb) = {
	.name = "wb",
	.exec = cmd_wb,
};
