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

int base_wbrw = 0;

static int cmd_setbase(const char *args[])
{
int addr;

	if (!args[0]) {
		mprintf("BASE_WB=0x%x\n",base_wbrw);
	}
	else {
		fromhex(args[0],&addr);
		base_wbrw = addr;
		mprintf("BASE_WB=0x%x\n",base_wbrw);
	}

	return 0;
}

DEFINE_COMMAND(setbase) = {
	.name = "setbase",
	.exec = cmd_setbase,
};

