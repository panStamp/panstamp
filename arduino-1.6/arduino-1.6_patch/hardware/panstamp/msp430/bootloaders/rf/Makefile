
# Configure serial port and path to the toolchain, acording to your settings
SERPORT = /dev/ttyUSB1
ARDUINO_ROOT = ~/applications/arduino-1.6.1/
TOOLCHAIN = $(ARDUINO_ROOT)/hardware/tools/msp430/bin/msp430-
JAVA_BSL = $(ARDUINO_ROOT)/app/BsLoader/dist/BsLoader.jar

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

CC = $(TOOLCHAIN)gcc
CXX = $(TOOLCHAIN)g++
AS = $(TOOLCHAIN)gcc -x assembler-with-cpp
OBJDUMP = $(TOOLCHAIN)objdump
OBJCOPY = $(TOOLCHAIN)objcopy
SIZE = $(TOOLCHAIN)size
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

gdb-flash:
	$(TOOLCHAIN)gdb -b 38400 -ex 'set debug remote 0' -ex 'target remote $(SERPORT)' $(PROG).elf -ex 'erase' -ex 'load' -ex 'quit'

bsl-flash:
	java -jar $(JAVA_BSL) $(PROG).hex $(SERPORT) --verif-off --verbose-on
