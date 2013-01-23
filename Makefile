#
# In most cases, you only need to modify this first section.
#
EXE  = object
OBJS = main.o uart.o shell.o crctab.o xmodem.o mx25l32.o spi_raw.o
LIBS = 
MCU = atmega16
SMPMCU = m16
CFLAGS += -mmcu=$(MCU) -Os


all : gperf $(EXE).hex

include ./Makefile.gperf
include ./make.common


#
# Implicit rule will make the .c into a .o
# Implicit rule is $(CC) -c $(CPPFLAGS) $(CFLAGS)
# See Section 10.2 of Gnu Make manual
# 
$(EXE).hex: $(EXE)
	$(OBJCOPY) -j .text -j .data -O ihex $(EXE) $@
	$(SIZE) $(EXE).hex

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)


#
# download with avrdude
# clean and distclean
#
.PHONY: download clean distclean

download: all
	sudo avrdude -p $(SMPMCU) -c usbasp -e -U flash:w:$(EXE).hex

clean: gperf_clean generic_clean

distclean: clean
	rm -f $(EXE).hex


#
# Include the rule for making dependency files.
# The '-' in front of the second include suppresses
# error messages when make cannot find the .d files.
# It will just regenerate them.
# See Section 4.14 of Gnu Make.
#

include ./make.deprules

-include $(OBJS:.o=.d)
