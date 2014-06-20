#ifndef __REGS_H
#define __REGS_H

#define SDB_ADDRESS 0x30000

extern unsigned char *BASE_UART;

#define FMC_EEPROM_ADR 0x50

void sdb_find_devices(void);
void sdb_print_devices(void);

#endif
