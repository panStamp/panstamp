MCU = cc430f5137
SRCS ?= rfloader.cpp cc430radio.cpp rf1a.cpp pmm.cpp swap.cpp

IDIR = -I../../../../tools/msp430/msp430/include

FLAGS = -g -Os -Wall -T rfloader.x -mmcu=$(MCU)
FLAGS += -ffunction-sections -fdata-sections -DF_CPU=12000000L
CPPFLAGS += $(FLAGS) $(IDIR) -I../../libraries/panStamp -c -MMD -fno-rtti -fno-enforce-eh-specs -fno-exceptions 
CPPFLAGS += -fwrapv -fomit-frame-pointer -std=gnu++98 -g -ggdb3

LDFLAGS = $(FLAGS) -mdisable-watchdog
LDFLAGS += -Wl,--gc-sections,-Map=$(PROG).map,-umain
LDFLAGS += -ffixed-r6

#CROSS = ../../../../tools/msp430/bin/msp430-
CROSS = ~/applications/arduino-1.5.6-r2/hardware/tools/msp430/bin/msp430-
CC = $(CROSS)gcc
CXX = $(CROSS)g++
AS = $(cross)gcc -x assembler-with-cpp
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy
SIZE = $(CROSS)size
LD = $(CC)

PROG := $(firstword $(SRCS:.cpp=))
OBJS := $(SRCS:.cpp=.o)

all: $(PROG).elf $(PROG).hex $(PROG).lst

%.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $(PROG).elf $(OBJS)
	$(SIZE) $<

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $<

%.lst: %.elf
	$(OBJDUMP) -DS $< > $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

clean:
	-rm -f $(PROG).elf $(PROG).lst $(PROG).hex $(OBJS) *.d *~

flash:
	./msp430-gdb -b 38400 -ex 'set debug remote 0' -ex 'target remote /dev/ttyUSB1' $(PROG).elf -ex 'erase' -ex 'load' -ex 'quit'
