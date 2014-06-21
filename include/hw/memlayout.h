#ifndef __REGS_H
#define __REGS_H

#define SDB_ADDRESS 0x00c00000

extern unsigned char *BASE_MINIC;
extern unsigned char *BASE_EP;
extern unsigned char *BASE_SOFTPLL;
extern unsigned char *BASE_PPS_GEN;
extern unsigned char *BASE_SYSCON;
extern unsigned char *BASE_UART;
extern unsigned char *BASE_ONEWIRE;
extern unsigned char *BASE_ETHERBONE_CFG;
extern unsigned char *BASE_WRPC_RAM;

#define FMC_EEPROM_ADR 0x50

void sdb_find_devices(void);
void sdb_print_devices(void);

#endif
