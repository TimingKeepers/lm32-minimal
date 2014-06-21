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
#include "sdb.h"

#define MAX_SDB_DEVS 30

extern int base_wbrw;
extern int sdb_get_devices(struct sdb_component * devs, unsigned int * n_devs, const int MAX_DEVS);

struct sdb_component devs[MAX_SDB_DEVS];
unsigned int n_devs = 0;

static int cmd_setbsdb(const char *args[])
{
int index;
int i;
uint32_t * v;
char * name;

	n_devs = 0;

	if (!args[0]) {
		if(sdb_get_devices(devs,&n_devs,MAX_SDB_DEVS) == 0) {
			
			mprintf("N devs: %d\n",n_devs);
		
			for(i = 0 ; i < n_devs ; i++) {
				name = (char *) devs[i].product.name;
				v = (uint32_t *) &(devs[i].addr_first);
				v++;
				mprintf("dev: %s - 0x%x\n",name,*v);
			}
		
			mprintf("BASE_WB=0x%x\n",base_wbrw);
		}
		else {
			return -2;
		}
	}
	else {
		fromdec(args[0],&index);
		
		if(sdb_get_devices(devs,&n_devs,MAX_SDB_DEVS) == 0) {
			
			v = (uint32_t *) &(devs[index].addr_first);
			
			v++;
		
			base_wbrw = *v;
		
			mprintf("BASE_WB=0x%x\n",base_wbrw);
		}
		else {
			return -2;
		}
	}

	return 0;
}

DEFINE_COMMAND(setbsdb) = {
	.name = "setbsdb",
	.exec = cmd_setbsdb,
};

