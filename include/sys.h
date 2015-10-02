#ifndef __SYS_H__
#define __SYS_H__

#include <inttypes.h>
#include <pp-printf.h>
#include <hw/memlayout.h>

#define mprintf pp_printf
#define vprintf pp_vprintf
#define sprintf pp_sprintf

#define UART_BAUDRATE 115200
#define CPU_CLOCK 62500000

#define SFP_PORTS 2

unsigned char *BASE_UART;

#endif
