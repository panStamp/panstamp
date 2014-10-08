#
# board/chip specific options
#

TOOLCHAIN = msp430-
#TOOLCHAIN = ../../../../tools/msp430/bin/msp430-
CORE = msp430
CORE_FLAGS = -mmcu=cc430f5137
F_CPU ?= 6500000
#F_CPU ?= 12000000
C_DEFS = -DFABOOH -DF_CPU=$(F_CPU) -DSHOW_CLOCKS=1
CXX_DEFS = -DFABOOH -DF_CPU=$(F_CPU) -DSHOW_CLOCKS=1
AS_DEFS =
OPTIMIZATION += -Os -g
LD_SCRIPT = $(FBD)$(BOARDDIR)/dummy.ld
LD_FLAGS = $(CORE_FLAGS) -mdisable-watchdog
LD_FLAGS += -Wl,--gc-sections,-Map=$(OUT_DIR_F)$(PROJECT).map,-umain
LD_FLAGS += -ffixed-r6
LD=$(CC)

BOOTLOADER = mspdebug
BOARD_SN ?=
BL_ARGS = rf2500 $(BOARD_SN) "prog $(OUT_DIR)/$(PROJECT).hex"
