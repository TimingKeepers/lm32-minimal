/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2012 GSI (www.gsi.de)
 * Author: Wesley W. Terpstra <w.terpstra@gsi.de>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
#include <string.h>
#include <sys.h>
#include "hw/memlayout.h"

unsigned char *BASE_MINIC;
unsigned char *BASE_EP;
unsigned char *BASE_SOFTPLL;
unsigned char *BASE_PPS_GEN;
unsigned char *BASE_SYSCON;
unsigned char *BASE_UART;
unsigned char *BASE_ONEWIRE;
unsigned char *BASE_ETHERBONE_CFG;
unsigned char *BASE_WRPC_RAM;

#define SDB_INTERCONNET 0x00
#define SDB_DEVICE      0x01
#define SDB_BRIDGE      0x02
#define SDB_EMPTY       0xFF

typedef struct pair64 {
	uint32_t high;
	uint32_t low;
} pair64_t;

struct sdb_empty {
	int8_t reserved[63];
	uint8_t record_type;
};

struct sdb_product {
	pair64_t vendor_id;
	uint32_t device_id;
	uint32_t version;
	uint32_t date;
	int8_t name[19];
	uint8_t record_type;
};

struct sdb_component {
	pair64_t addr_first;
	pair64_t addr_last;
	struct sdb_product product;
};

struct sdb_device {
	uint16_t abi_class;
	uint8_t abi_ver_major;
	uint8_t abi_ver_minor;
	uint32_t bus_specific;
	struct sdb_component sdb_component;
};

struct sdb_bridge {
	pair64_t sdb_child;
	struct sdb_component sdb_component;
};

struct sdb_interconnect {
	uint32_t sdb_magic;
	uint16_t sdb_records;
	uint8_t sdb_version;
	uint8_t sdb_bus_type;
	struct sdb_component sdb_component;
};

typedef union sdb_record {
	struct sdb_empty empty;
	struct sdb_device device;
	struct sdb_bridge bridge;
	struct sdb_interconnect interconnect;
} sdb_record_t;

static unsigned char *find_device_deep(unsigned int base, unsigned int sdb,
				       unsigned int devid)
{
	sdb_record_t *record = (sdb_record_t *) sdb;
	int records = record->interconnect.sdb_records;
	int i;

	for (i = 0; i < records; ++i, ++record) {
		if (record->empty.record_type == SDB_BRIDGE) {
			unsigned char *out =
			    find_device_deep(base +
					     record->bridge.sdb_component.
					     addr_first.low,
					     base +
					     record->bridge.sdb_child.low,
					     devid);
			if (out)
				return out;
		}
		if (record->empty.record_type == SDB_DEVICE &&
		    record->device.sdb_component.product.device_id == devid) {
			break;
		}
	}

	if (i == records)
		return 0;
	return (unsigned char *)(base +
				 record->device.sdb_component.addr_first.low);
}

static int get_devices_deep(unsigned int base, unsigned int sdb, struct sdb_component * devs, unsigned int * n_devs, const int MAX_DEVS)
{
	sdb_record_t *record = (sdb_record_t *) sdb;
	int records = record->interconnect.sdb_records;
	int i;
	int rcode;

	for (i = 0; i < records; ++i, ++record) {
		if (record->empty.record_type == SDB_BRIDGE)
			rcode = get_devices_deep(base + record->bridge.sdb_component.addr_first.low, base + record->bridge.sdb_child.low,devs,n_devs,MAX_DEVS);
			if (rcode < 0)
				return -1;

		if (record->empty.record_type != SDB_DEVICE)
			continue;

		if (*n_devs < MAX_DEVS) {
			devs[*n_devs] = record->device.sdb_component;
			devs[*n_devs].product.name[19] = 0;
			devs[*n_devs].addr_first.low = base + record->device.sdb_component.addr_first.low;
			devs[*n_devs].addr_last.low = base + record->device.sdb_component.addr_last.low;
			//mprintf("DEV: %s (0x%x-0x%x)\n",devs[*n_devs].product.name,devs[*n_devs].addr_first.low,devs[*n_devs].addr_last.low);
			(*n_devs)++;
		}
		else {
			return -1;
		}
	}
	
	return 0;
}

static void print_devices_deep(unsigned int base, unsigned int sdb)
{
	sdb_record_t *record = (sdb_record_t *) sdb;
	int records = record->interconnect.sdb_records;
	int i;
	char buf[20];

	for (i = 0; i < records; ++i, ++record) {
		if (record->empty.record_type == SDB_BRIDGE)
			print_devices_deep(base +
					   record->bridge.sdb_component.
					   addr_first.low,
					   base +
					   record->bridge.sdb_child.low);

		if (record->empty.record_type != SDB_DEVICE)
			continue;

		memcpy(buf, record->device.sdb_component.product.name, 19);
		buf[19] = 0;
		mprintf("%8x:%8x 0x%8x %s\n",
			record->device.sdb_component.product.vendor_id.low,
			record->device.sdb_component.product.device_id,
			base + record->device.sdb_component.addr_first.low,
			buf);
	}
}

static unsigned char *find_device(unsigned int devid)
{
	return find_device_deep(0, SDB_ADDRESS, devid);
}

void sdb_print_devices(void)
{
	mprintf("SDB memory map:\n");
	print_devices_deep(0, SDB_ADDRESS);
	mprintf("---\n");
}

int sdb_get_devices(struct sdb_component * devs, unsigned int * n_devs, const int MAX_DEVS) {
	//sdb_print_devices();
	return get_devices_deep(0,SDB_ADDRESS,devs,n_devs,MAX_DEVS);
}

void sdb_find_devices(void)
{
	BASE_MINIC =         find_device(0xab28633a);
	BASE_EP =            find_device(0x650c2d4f);
	BASE_SOFTPLL =       find_device(0x65158dc0);
	BASE_PPS_GEN =       find_device(0xde0d8ced);
	BASE_SYSCON =        find_device(0xff07fc47);
	BASE_UART =          find_device(0xe2d13d04);
	BASE_ONEWIRE =       find_device(0x779c5443);
	BASE_ETHERBONE_CFG = find_device(0x68202b22);
	BASE_WRPC_RAM = find_device(0x66cfeb52);
}
