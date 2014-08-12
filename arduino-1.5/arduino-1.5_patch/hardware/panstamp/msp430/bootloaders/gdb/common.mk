#=============================================================================#
# common.mk - this file mostly based on the Chopin make. It is heavily
#             modifed for use with fabooh and multiple chip targets
#
# author: Freddie Chopin, http://www.freddiechopin.info/
# last change: 2010-10-13
#
# this makefile is based strongly on many examples found in the network
#
# 04-17-2013 - rick@kimballsoftware use this as common.mk
#              include in your own directory, see example/basic/blink
#              for information how to do this
# 04-26-2013 - rick@kimballsoftware, modified for multi boards and 
#              different chip architectures
#=============================================================================#

#=============================================================================#
# project configuration
#=============================================================================#

# PROJECT = sample
# project name, we don't set it here, configure in Makefile of user code
# see examples for proper Makefile setup

#=============================================================================#
# fabooh TLD (Top Level Directory)
#=============================================================================#
FBD := $(dir $(lastword $(MAKEFILE_LIST)))

# which board will be used as default
BOARD ?= panstamp
BOARDDIR = board/$(BOARD)
BOARDS = $(notdir $(wildcard $(FBD)board/*))
_3RD_PARTY_DIR = include/3rdparty

FIXMATH_FLAGS = -DFIXMATH_NO_CACHE -DFIXMATH_NO_64BIT -DFIXMATH_NO_ROUNDING
FABOOH_FLAGS = $(FIXMATH_FLAGS)
FABOOH_FLAGS += -fwrapv -fomit-frame-pointer
FABOOH_FLAGS += -ffunction-sections -fdata-sections

#=============================================================================#
# board specific options, cpu and toolchain selection
#=============================================================================#
include $(FBD)$(BOARDDIR)/board.mk

#=============================================================================#
# toolchain configuration
#=============================================================================#

CXX = $(TOOLCHAIN)g++
CC = $(TOOLCHAIN)gcc
AS = $(TOOLCHAIN)gcc -x assembler-with-cpp
OBJCOPY = $(TOOLCHAIN)objcopy
OBJDUMP = $(TOOLCHAIN)objdump
SIZE = $(TOOLCHAIN)size

# output folder (absolute or relative path, leave empty for in-tree compilation)
OUT_DIR = $(BOARD)_release

# include directories (absolute or relative paths to additional folders with
# headers, current folder is always included)
INC_DIRS = include/$(CORE)/core $(BOARDDIR) $(_3RD_PARTY_DIR) 

# library directories (absolute or relative paths to additional folders with
# libraries)
LIB_DIRS = $(BOARDDIR)

# libraries (additional libraries for linking, e.g. "-lm -lsome_name" to link
# math library libm.a and libsome_name.a)
LIBS ?=

# additional directories with source files (absolute or relative paths to
# folders with source files, current folder is always included)
SRCS_DIRS = $(FBD)$(BOARDDIR)

# extension of C++ files
CXX_EXT = cpp

# wildcard for C++ source files (all files with CXX_EXT extension found in
# current folder and SRCS_DIRS folders will be compiled and linked)
CXX_SRCS = $(wildcard $(patsubst %, %/*.$(CXX_EXT), . $(SRCS_DIRS)))

# extension of C files
C_EXT = c

# wildcard for C source files (all files with C_EXT extension found in current
# folder and SRCS_DIRS folders will be compiled and linked)
C_SRCS = $(wildcard $(patsubst %, %/*.$(C_EXT), . $(SRCS_DIRS)))

# extension of ASM files
AS_EXT = S

# wildcard for ASM source files (all files with AS_EXT extension found in
# current folder and SRCS_DIRS folders will be compiled and linked)
AS_SRCS = $(wildcard $(patsubst %, %/*.$(AS_EXT), . $(SRCS_DIRS)))

# optimization flags ("-O0" - no optimization, "-O1" - optimize, "-O2" -
# optimize even more, "-Os" - optimize for size or "-O3" - optimize yet more) 
OPTIMIZATION ?= -Os

# warning options 
#CXX_WARNINGS = -Wall -Wextra
#C_WARNINGS = -Wall -Wstrict-prototypes -Wextra

CXX_WARNINGS = -Wall -Wextra
C_WARNINGS = $(CXX_WARNINGS) -Wstrict-prototypes 

# C++ language standard ("c++98", "gnu++98" - default, "c++0x", "gnu++0x")
CXX_STD = gnu++98

# C language standard ("c89" / "iso9899:1990", "iso9899:199409",
# "c99" / "iso9899:1999", "gnu89" - default, "gnu99")
C_STD = gnu99

#=============================================================================#
# set the VPATH according to SRCS_DIRS
#=============================================================================#

VPATH = $(SRCS_DIRS)

#=============================================================================#
# when using output folder, append trailing slash to its name
#=============================================================================#

ifeq ($(strip $(OUT_DIR)), )
	OUT_DIR_F =
else
	OUT_DIR_F = $(strip $(OUT_DIR))/
endif

#=============================================================================#
# various compilation flags
#=============================================================================#

# listing flags
LISTING_FLAGS = -Wa,-ahlms=$(OUT_DIR_F)$(notdir $(<:.c=.lst))

# flags for C++ compiler
#CXX_FLAGS = -std=$(CXX_STD) -g -ggdb3 -fno-rtti -fno-exceptions -fverbose-asm $(LISTING_FLAGS)
CXX_FLAGS = -std=$(CXX_STD) -g -ggdb3 -fno-rtti -fno-exceptions -fverbose-asm

# flags for C compiler
#C_FLAGS = -std=$(C_STD) -g -ggdb3 -fverbose-asm -Wa,-ahlms=$(OUT_DIR_F)$(notdir $(<:.$(C_EXT)=.lst))
C_FLAGS = -std=$(C_STD) -g -ggdb3 -fverbose-asm

# flags for assembler
#AS_FLAGS = -g -ggdb3 -Wa,-amhls=$(OUT_DIR_F)$(notdir $(<:.$(AS_EXT)=.lst))
AS_FLAGS = -g -ggdb3


#=============================================================================#
# do some formatting
#=============================================================================#

CXX_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(CXX_SRCS:.$(CXX_EXT)=.o)))
C_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(C_SRCS:.$(C_EXT)=.o)))
AS_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(AS_SRCS:.$(AS_EXT)=.o)))
OBJS = $(AS_OBJS) $(C_OBJS) $(CXX_OBJS) $(USER_OBJS)
DEPS = $(OBJS:.o=.d)
INC_DIRS_F = -I$(FBD) $(patsubst %, -I$(FBD)%, $(INC_DIRS))
LIB_DIRS_F = $(patsubst %, -L$(FBD)%, $(LIB_DIRS))

ELF = $(OUT_DIR_F)$(PROJECT).elf
HEX = $(OUT_DIR_F)$(PROJECT).hex
BIN = $(OUT_DIR_F)$(PROJECT).bin
LSS = $(OUT_DIR_F)$(PROJECT).lss
DMP = $(OUT_DIR_F)$(PROJECT).dmp


# format final flags for tools, request dependancies for C++, C and asm
CXX_FLAGS_F = $(CORE_FLAGS) $(OPTIMIZATION) $(CXX_WARNINGS) $(FABOOH_FLAGS) $(CXX_FLAGS) $(CXX_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
C_FLAGS_F = $(CORE_FLAGS) $(OPTIMIZATION) $(C_WARNINGS) $(FABOOH_FLAGS) $(C_FLAGS) $(C_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
AS_FLAGS_F = $(CORE_FLAGS) $(FABOOH_FLAGS) $(AS_FLAGS) $(AS_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
LD_FLAGS_F = $(CORE_FLAGS) $(FABOOH_LD_FLAGS) $(LD_FLAGS) $(LIB_DIRS_F)

#contents of output directory
GENERATED = $(wildcard $(patsubst %, $(OUT_DIR_F)*.%, bin d dmp elf hex lss lst map o))

#=============================================================================#
# make all
#=============================================================================#

all : make_output_dir $(ELF) $(LSS) $(DMP) $(HEX) $(BIN) print_size 

# make object files dependent on Makefile
$(OBJS) : Makefile

# make .elf file dependent on linker script
$(ELF) : $(LD_SCRIPT)

#-----------------------------------------------------------------------------#
# linking - objects -> elf
#-----------------------------------------------------------------------------#

$(ELF) : $(OBJS)
	@echo 'Linking target: $(ELF)'
	$(LD) $(LD_FLAGS_F) $(OBJS) $(LIBS) -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# compiling - C++ source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(CXX_EXT)
	@echo 'Compiling file: $<'
	$(CXX) -c $(CXX_FLAGS_F) $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# compiling - C source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(C_EXT)
	@echo 'Compiling C file: $<'
	$(CC) -c $(C_FLAGS_F) $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# assembling - ASM source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(AS_EXT)
	@echo 'Assembling file: $<'
	$(AS) -c $(AS_FLAGS_F) $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# memory images - elf -> hex, elf -> bin
#-----------------------------------------------------------------------------#

$(HEX) : $(ELF)
	@echo 'Creating IHEX image: $(HEX)'
	$(OBJCOPY) -O ihex $< $@
	@echo ' '

$(BIN) : $(ELF)
	@echo 'Creating binary image: $(BIN)'
	$(OBJCOPY) -O binary $< $@
	@echo ' '

#-----------------------------------------------------------------------------#
# memory dump - elf -> dmp
#-----------------------------------------------------------------------------#

$(DMP) : $(ELF)
	@echo 'Creating memory dump: $(DMP)'
	$(OBJDUMP) -x --syms $< > $@
	@echo ' '

#-----------------------------------------------------------------------------#
# extended listing - elf -> lss
#-----------------------------------------------------------------------------#

$(LSS) : $(ELF)
	@echo 'Creating extended listing: $(LSS)'
	$(OBJDUMP) -dSC $< > $@
	@echo ' '

#-----------------------------------------------------------------------------#
# print the size of the objects 
#-----------------------------------------------------------------------------#

print_size :
	@echo 'Size of modules:'
	$(SIZE) -B -t --common $(OBJS) $(USER_OBJS)
	@echo ' '
	$(SIZE) $(ELF)
	@echo ' '

#-----------------------------------------------------------------------------#
# install
#-----------------------------------------------------------------------------#

install : all
	$(BOOTLOADER) $(BL_ARGS)


#-----------------------------------------------------------------------------#
# create the desired output directory
#-----------------------------------------------------------------------------#

make_output_dir :
	$(shell mkdir $(OUT_DIR_F) 2>/dev/null)
	
boards :
	@echo 'valid BOARD targets are: $(BOARDS)'
	@echo 'use:'
	@echo '$$ make BOARD=xxxxxx clean all install'

#=============================================================================#
# make clean
#=============================================================================#

clean:
ifeq ($(strip $(OUT_DIR_F)), )
	@echo 'Removing all generated output files'
else
	@echo 'Removing all generated output files from output directory: $(OUT_DIR_F)'
endif
ifneq ($(strip $(GENERATED)), )
	$(RM) $(GENERATED)
	$(RM) -r $(OUT_DIR_F)
else
	@echo 'Nothing to remove...'
endif

#=============================================================================#
# global exports
#=============================================================================#

.PHONY: all clean dependents install boards

.SECONDARY:

# include dependancy files
-include $(DEPS)
