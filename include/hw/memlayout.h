#ifndef __REGS_H
#define __REGS_H

#define SDB_ADDRESS 0x70000

extern unsigned char *BASE_UART;
extern unsigned char *BASE_SYSCON;

#define FMC_EEPROM_ADR 0x51

void sdb_find_devices(void);
void sdb_print_devices(void);

#endif
