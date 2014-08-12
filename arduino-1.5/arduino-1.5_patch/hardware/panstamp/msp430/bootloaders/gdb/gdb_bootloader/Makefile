#
# fabooh example makefile
#

PYTHON := $(shell which python2 || which python)
BOARD = panstamp
PROJECT ?= gdb_bootloader

include ../common.mk

LD_FLAGS += -L ./ -T gdb_bootloader.x

gdb_bootloader.txt: $(OUT_DIR_F)$(PROJECT).elf
	$(PYTHON) ../tools/memory.py -i $< -o $@

test:
#	$(TOOLCHAIN)gdb -b 9600 -ex 'set remotetimeout 1' -ex 'target remote /dev/ttyUSB0' blink.elf
	$(TOOLCHAIN)gdb -b 38400 -ex 'set debug remote 0' -ex 'target remote /dev/ttyUSB0' blink.elf
#	$(TOOLCHAIN)gdb -b 38400 -ex 'set debug remote 0' -ex 'target remote /dev/ttyUSB0' blink.elf -ex 'erase' -ex 'load'

