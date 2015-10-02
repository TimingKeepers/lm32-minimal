#ifndef __EEPROM_H
#define __EEPROM_H

#ifdef CONFIG_LEN_BOARD
#define EE_BASE_FRU_ID 0
#define EE_BASE_FRU 14
#define FRU_TYPELEN_MASK 0xC0
#endif

#define SFP_SECTION_PATTERN 0xdeadbeef
#define SFPS_MAX 4
#define SFP_PN_LEN 16
#define EE_BASE_CAL_1 4*1024
#define EE_BASE_CAL_2 4*1024+4
#define EE_BASE_SFP 4*1024+8
#define EE_BASE_INIT 16+4*1024+SFPS_MAX*30  //Base address increased

#define EE_RET_I2CERR -1
#define EE_RET_DBFULL -2
#define EE_RET_CORRPT -3
#define EE_RET_POSERR -4

extern int32_t sfp_alpha[2];
extern int32_t sfp_deltaTx[2];
extern int32_t sfp_deltaRx[2];
extern uint32_t cal_phase_transition[2];
extern uint8_t has_eeprom;

extern uint64_t w1_sensor_rom;

struct s_sfpinfo {
	char pn[SFP_PN_LEN];
	int32_t alpha;
	uint8_t port_position;		//WR-LEN parameter.
	int32_t dTx;
	int32_t dRx;
	uint8_t chksum;

} __attribute__ ((__packed__));

uint8_t eeprom_present(uint8_t i2cif, uint8_t i2c_addr);

int8_t eeprom_read_board_fru(uint8_t i2cif, uint8_t i2c_addr);

int32_t eeprom_sfpdb_erase(uint8_t i2cif, uint8_t i2c_addr);
int32_t eeprom_sfp_section(uint8_t i2cif, uint8_t i2c_addr, size_t size,
			   uint16_t * section_sz);
int8_t eeprom_match_sfp(uint8_t i2cif, uint8_t i2c_addr, struct s_sfpinfo *sfp, uint8_t port);

int8_t eeprom_phtrans(uint8_t i2cif, uint8_t i2c_addr, uint32_t * val,
		      uint8_t write, uint8_t port);

int8_t eeprom_init_erase(uint8_t i2cif, uint8_t i2c_addr);
int8_t eeprom_init_add(uint8_t i2cif, uint8_t i2c_addr, const char *args[]);
int32_t eeprom_init_show(uint8_t i2cif, uint8_t i2c_addr);
int8_t eeprom_init_readcmd(uint8_t i2cif, uint8_t i2c_addr, uint8_t *buf,
			   uint8_t bufsize, uint8_t next);

int32_t eeprom_get_sfp(uint8_t i2cif, uint8_t i2c_addr, struct s_sfpinfo * sfp,
                       uint8_t add, uint8_t pos);

#endif
