#include "shell.h"
#include "sdb.h"
#include "hw/memlayout.h"

static int cmd_sdb(const char *args[])
{
	sdb_print_devices();
	return 0;
}

DEFINE_COMMAND(sdb) = {
	.name = "sdb",
	.exec = cmd_sdb,
};
