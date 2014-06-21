/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2014 UGR (www.ugr.es)
 * Author: Miguel Jimenez Lopez <klyone@ugr.es>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */

#include <string.h>
#include <sys.h>
#include "shell.h"

static int cmd_read(const char *args[])
{
int addr;
int val;
int * addr_r;

	if (args[0]) {
		fromhex(args[0],&addr);
		
		addr_r = (int *) BASE_WRPC_RAM + addr;
		
		val = *addr_r;
		
		mprintf("M[0x%x]=0x%x\n",(unsigned int) addr_r,val);
	}
	else {
		return -1;
	}

	return 0;
}

DEFINE_COMMAND(read) = {
	.name = "read",
	.exec = cmd_read,
};

