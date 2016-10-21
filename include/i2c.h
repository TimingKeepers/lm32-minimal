#ifndef __I2C_H
#define __I2C_H

uint8_t mi2c_devprobe(uint8_t i2cif, uint8_t i2c_addr);
void mi2c_init(uint8_t i2cif);
void mi2c_start(uint8_t i2cif);
void mi2c_repeat_start(uint8_t i2cif);
void mi2c_stop(uint8_t i2cif);
void mi2c_get_byte(uint8_t i2cif, unsigned char *data, uint8_t last);
unsigned char mi2c_put_byte(uint8_t i2cif, unsigned char data);

void mi2c_delay();
//void mi2c_scan(uint8_t i2cif);

// I2C Switch methods

#define I2C_SWITCH_N 3
#define I2C_SWITCH0_GEN 0
#define I2C_SWITCH1_SFP0_7 1
#define I2C_SWITCH2_SFP8_15 2

extern uint8_t i2c_sw_addrs[I2C_SWITCH_N];

int read_i2c_switch(uint8_t i2cif,uint8_t sw_addr);
int write_i2c_switch(uint8_t i2cif,uint8_t sw_addr, uint8_t value);
int configure_i2c_switch(uint8_t i2cif,uint8_t sw_addr, uint8_t sw_config);

#endif
