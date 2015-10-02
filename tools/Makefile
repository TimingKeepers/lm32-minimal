CFLAGS = -Wall -ggdb -I../include
LDFLAGS = -lutil
ALL    = genraminit genramvhd

AS		= as
LD		= ld
CC		= gcc
CPP		= $(CC) -E
AR		= ar
NM		= nm
STRIP		= strip
OBJCOPY		= objcopy
OBJDUMP		= objdump

all:	$(ALL)

%:	%.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@
	
clean:
	rm -f $(ALL) *.o *~
