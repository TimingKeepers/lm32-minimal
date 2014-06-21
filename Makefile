# Miguel Jimenez Lopez for UGR 2014.
# Original: Makefile from wrpc-sw repository (OHWR), Tomasz Wlostowski for CERN, 2011,2012

CROSS_COMPILE ?= lm32-elf-
export CROSS_COMPILE

CC =		$(CROSS_COMPILE)gcc
LD =		$(CROSS_COMPILE)ld
OBJDUMP =	$(CROSS_COMPILE)objdump
OBJCOPY =	$(CROSS_COMPILE)objcopy
SIZE =		$(CROSS_COMPILE)size

obj-y = arch/lm32/crt0.o arch/lm32/irq.o
obj-y += main.o
LDS-y   = arch/lm32/ram.ld

cflags-y =	-ffreestanding -I. -Iinclude
cflags-y +=	-I$(CURDIR)/pp_printf

CFLAGS_PLATFORM  = -mmultiply-enabled -mbarrel-shift-enabled
LDFLAGS_PLATFORM = -mmultiply-enabled -mbarrel-shift-enabled \
	-nostdlib -T $(LDS-y)

include pp_printf/printf.mk
include dev/dev.mk
include shell/shell.mk

CFLAGS = $(CFLAGS_PLATFORM) $(cflags-y) -Wall \
	-ffunction-sections -fdata-sections -Os \
	-include include/trace.h

# This povokes a problem!
#CFLAGS += -ggdb

LDFLAGS = $(LDFLAGS_PLATFORM) \
	-Wl,--gc-sections -Os -lgcc -lc

OBJS = $(obj-y)

OUTPUT-y   = main_run
OUTPUT := $(OUTPUT-y)

all: build_tools $(OUTPUT).elf $(OUTPUT).bin $(OUTPUT).ram

$(OUTPUT).elf: $(LDS-y) $(OUTPUT).o
	${CC} -o $@ $(OUTPUT).o $(LDFLAGS)
	${OBJDUMP} -d $(OUTPUT).elf > $(OUTPUT)_disasm.S
	$(SIZE) $@

$(OUTPUT).o: $(OBJS)
	$(LD) --gc-sections -e _start -r $(OBJS) -T bigobj.lds -o $@
	
$(OUTPUT).bin: $(OUTPUT).elf
	${OBJCOPY} -O binary $^ $@

$(OUTPUT).ram: $(OUTPUT).bin
	./tools/genraminit $(OUTPUT).bin 0 > $@
	
build_tools:
	$(MAKE) -C tools

clean:
	rm -f $(OBJS) $(OUTPUT).elf $(OUTPUT).bin $(OUTPUT).ram $(LDS) *.o *.S *~

%.o:		%.c
	${CC} $(CFLAGS) $(PTPD_CFLAGS) $(INCLUDE_DIR) $(LIB_DIR) -c $*.c -o $@
