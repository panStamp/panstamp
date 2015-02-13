/* This work is partially financed by the European Commission under the
 * Framework 6 Information Society Technologies Project
 * "Wirelessly Accessible Sensor Populations (WASP)".
 */

/* Definitions of target machine for GNU compiler,
   for Texas Instruments MSP430 microcontrollers.
   Copyright (C) 2001-2009 Free Software Foundation, Inc.
   Contributed by Dmitry Diky <diwil@mail.ru>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* The GCC internals manual (http://gcc.gnu.org/onlinedocs/gccint, but
 * only if you're working on the trunk) defines the current standard
 * back-end expectations.  See Chapter 17 "Target Macros".
 *
 * Target macros and hooks should not be defined if the default
 * behavior is correct for the platform.  To make it clear that the
 * maintainer is aware of the macro and has verified it, it may be
 * defined to be its default in a comment with a prefix DEFAULT: to
 * make it clear this is a comment, not droppings from disabled code
 * (see, for example, PTR_DIFF_TYPE below).
 */

/** Update this on each release */
#define MSP430_MSPGCC_VERSION 20120406

/** Bit-markers for type of CPU present. */
typedef enum msp430_cpu_e
{
  MSP430_CPU_MSP430 = 0x0000,
  MSP430_CPU_MSP430X = 0x0002,
  MSP430_CPU_MSP430XV2 = 0x0003,
  MSP430_CPU = 0x0003
} msp430_cpu_e;

/** Bit-markers for type of hardware multiplier present. */
typedef enum msp430_mpy_e
{
  MSP430_MPY_NONE = 0x0000,
  MSP430_MPY_TYPE_16 = 0x0010,
  MSP430_MPY_TYPE_32 = 0x0020,
  MSP430_MPY_TYPE_ANY = 0x0030,
  MSP430_MPY_HAS_SE = 0x0001,
  MSP430_MPY_HAS_DW = 0x0002,
  MSP430_MPY_16 = MSP430_MPY_TYPE_16,
  MSP430_MPY_16SE = MSP430_MPY_16 | MSP430_MPY_HAS_SE,
  MSP430_MPY_32 = MSP430_MPY_TYPE_16 | MSP430_MPY_TYPE_32 | MSP430_MPY_HAS_SE,
  MSP430_MPY_32DW = MSP430_MPY_32 | MSP430_MPY_HAS_DW
} msp430_mpy_e;

extern msp430_cpu_e msp430_cpu;
extern msp430_mpy_e msp430_mpy;
extern int msp430_ivcnt;
extern int msp430_has_hwmul;

#define MSP430_HAS_HWMUL_INTERNAL (msp430_has_hwmul)

int msp430_current_function_noint_hwmul_function_p (void);
#define MSP430_NOINT_HWMUL (msp430_current_function_noint_hwmul_function_p())

/* 1 if the integral value can be produced by the MSP430 CG registers */
#define MSP430_CG_INT_P(VALUE)	\
  ((-1 == (VALUE))		\
   || (0 == (VALUE))		\
   || (1 == (VALUE))		\
   || (2 == (VALUE))		\
   || (4 == (VALUE))		\
   || (8 == (VALUE)))

/* Maximum number of positions a single CPUX multi-position shift
 * operation can take. */
#define MSP430_CPUX_MULTISHIFT_MAX 4

/* 1 if the integral value can be the shift count in a CPUX
 * multi-position shift operation. */
#define MSP430_CPUX_MULTISHIFT_COUNT_P(VALUE) \
  (1 <= (VALUE) && (VALUE) <= MSP430_CPUX_MULTISHIFT_MAX)

/* 16.19.5 Insn Lengths */
#define ADJUST_INSN_LENGTH(INSN, LENGTH) \
  (LENGTH = msp430_adjust_insn_length (INSN, LENGTH))

/* 17.1 Target Structure -- defined in msp430.c */

/* 17.2 Driver */
/* On startup read the spec file that translates known MCUs into
 * device-specific flags (note: this is an msp430 extension to the
 * upstream driver).  Also normalize any legacy mcu descriptions,
 * then ensure we have -mcpu and -mmpy configurations derived from
 * the mcu. */
#define DRIVER_SELF_SPECS \
  "%:include-noerr(msp430mcu.spec)" \
  " %(msp430_cpu)" \
  " %(msp430_mpy)" \
  " %(msp430_ivcnt)"
/* Accept -posix; invoke a special function to
 * rote-translate -mmcu options into preprocessor definition
 * options.  */
#define CPP_SPEC "%{posix:-D_POSIX_SOURCE} %:msp430_mcucpp(%{mmcu=*:%*})"
/* Provide device-specific -mivcnt, -mcpu, -mmpy options. */
#define CC1_SPEC "%{profile:-p} %{mcpu=*} %{mmpy=*} %{mivcnt=*}"
/* TODO: legacy value */
#define CC1PLUS_SPEC "-fno-rtti -fno-exceptions"
/* Pass MCU-related options to the assembler. */
#define ASM_SPEC "%{mcpu=*} %{mmcu=*}"
/* Do not add -lg when linking statically */
#define LIB_SPEC "%{!shared:%{!p:%{!pg:-lc}}%{p:-lc_p}%{pg:-lc_p}}"
/* Use the msp430 linker emulation */
#define LINK_SPEC "-m msp430"
/* Select the corresponding interrupt vector initializer based on
 * mivcnt. */
#define STARTFILE_SPEC				\
  "%{mivcnt=16:crt0ivtbl16%O%s} "		\
  "%{mivcnt=32:crt0ivtbl32%O%s} "		\
  "%{mivcnt=64:crt0ivtbl64%O%s}"
#define ENDFILE_SPEC \
  "%{mno-disable-watchdog|!mdisable-watchdog:-lcrt0; :-lcrt0dwdt}"
extern const char *msp430_mcucpp (int argc, const char **argv);
extern const char *msp430_mculdscriptpaths (int argc, const char **argv);
#define EXTRA_SPEC_FUNCTIONS				\
  { "msp430_mcucpp", msp430_mcucpp },			\
  { "msp430_mculdscriptpaths", msp430_mculdscriptpaths },
#define EXTRA_SPECS							\
  { "msp430_cpu", "%{!mcpu=*:%{mcpu=430}}" },				\
  { "msp430_mpy", "%{!mmpy=*:%{mmpy=none}}" },				\
  { "msp430_ivcnt", "%{!mivcnt=*:%{mivcnt=16}}" },
/* Override this.  The value is essentially the default from gcc.c,
 * but we need to be able to define a default linker script derived
 * from the -mmcu parameter, and it has to be at the end; it doesn't
 * work when placed after the emulation in LINK_SPEC.  */
#define LINK_COMMAND_SPEC "\
%{!fsyntax-only:%{!c:%{!M:%{!MM:%{!E:%{!S:\
    %(linker) \
    %{"PLUGIN_COND": \
    -plugin %(linker_plugin_file) \
    -plugin-opt=%(lto_wrapper) \
    -plugin-opt=-fresolution=%u.res \
    %{!nostdlib:%{!nodefaultlibs:%:pass-through-libs(%(link_gcc_c_sequence))}} \
    }"PLUGIN_COND_CLOSE" \
    %{flto|flto=*:%<fcompare-debug*} \
    %{flto} %{flto=*} %l " LINK_PIE_SPEC \
   "%X %{o*} %{e*} %{N} %{n} %{r}\
    %{s} %{t} %{u*} %{z} %{Z} %{!nostdlib:%{!nostartfiles:%S}}\
    %{static:} %{L*} %(mfwrap) %(link_libgcc) %o\
    %{fopenmp|ftree-parallelize-loops=*:%:include(libgomp.spec)%(link_gomp)}\
    %(mflib) " STACK_SPLIT_SPEC "\
    %{fprofile-arcs|fprofile-generate*|coverage:-lgcov}\
    %{!nostdlib:%{!nodefaultlibs:%(link_ssp) %(link_gcc_c_sequence)}}\
    %{!nostdlib:%{!nostartfiles:%E}} %{T*} \
    %{mmcu=*:%:msp430_mculdscriptpaths(%{mmcu=*:%*})} \
    }}}}}}"
#define MULTILIB_DEFAULTS { "mcpu=430", "mmpy=none" }

/* 17.3 Run-time Target */
#define MSP430_BUILTIN_DEFINE(_m) do {				\
  snprintf(defstr, sizeof(defstr), "__" #_m "__=%d", _m);	\
  builtin_define(defstr);					\
} while (0)

#define TARGET_CPU_CPP_BUILTINS()					\
  do {									\
    char defstr[64];							\
    builtin_define_std ("MSP430");					\
    if(msp430_has_hwmul)						\
      builtin_define ("MSP430_HAS_HWMUL");				\
    else								\
      builtin_define ("MSP430_NO_HWMUL");				\
    if (MSP430_CPU_MSP430X <= msp430_cpu)				\
      {									\
	snprintf(defstr, sizeof (defstr), "__MSP430X__=%d", msp430_cpu); \
	builtin_define (defstr);					\
      }									\
    snprintf (defstr, sizeof (defstr), "__MSPGCC__=%d", MSP430_MSPGCC_VERSION); \
    builtin_define (defstr);						\
    MSP430_BUILTIN_DEFINE(MSP430_CPU_MSP430);				\
    MSP430_BUILTIN_DEFINE(MSP430_CPU_MSP430X);				\
    MSP430_BUILTIN_DEFINE(MSP430_CPU_MSP430XV2);			\
    MSP430_BUILTIN_DEFINE(MSP430_MPY_NONE);				\
    MSP430_BUILTIN_DEFINE(MSP430_MPY_TYPE_16);				\
    MSP430_BUILTIN_DEFINE(MSP430_MPY_TYPE_32);				\
    MSP430_BUILTIN_DEFINE(MSP430_MPY_TYPE_ANY);				\
    MSP430_BUILTIN_DEFINE(MSP430_MPY_HAS_SE);				\
    MSP430_BUILTIN_DEFINE(MSP430_MPY_HAS_DW);				\
    MSP430_BUILTIN_DEFINE(MSP430_MPY_16);				\
    MSP430_BUILTIN_DEFINE(MSP430_MPY_16SE);				\
    MSP430_BUILTIN_DEFINE(MSP430_MPY_32);				\
    MSP430_BUILTIN_DEFINE(MSP430_MPY_32DW);				\
    snprintf (defstr, sizeof (defstr), "__MSP430_CPU__=%d", msp430_cpu); \
    builtin_define (defstr);						\
    if (MSP430_MPY_NONE != msp430_mpy)					\
      {									\
	snprintf (defstr, sizeof (defstr), "__MSP430_MPY__=%d", msp430_mpy); \
	builtin_define (defstr);					\
	if (msp430_mpy & MSP430_MPY_TYPE_16)				\
	  {								\
	    snprintf (defstr, sizeof (defstr), "__MSP430_MPY16__=%d", msp430_mpy); \
	    builtin_define (defstr);					\
	  }								\
	if (msp430_mpy & MSP430_MPY_TYPE_32)				\
	  {								\
	    snprintf (defstr, sizeof (defstr), "__MSP430_MPY32__=%d", msp430_mpy); \
	    builtin_define (defstr);					\
	  }								\
      }									\
    snprintf (defstr, sizeof (defstr), "__MSP430_IVCNT__=%d", msp430_ivcnt); \
    builtin_define (defstr);						\
    builtin_define ("__interrupt=__attribute__((__interrupt__))");	\
  } while (0)
/* TODO: TARGET_HANDLE_OPTION ? */
#define TARGET_VERSION fprintf (stderr, " (TI MSP430)");

/* 17.4 Per-Function Data -- not used*/

/* 17.5: Storage Layout */
#define BITS_BIG_ENDIAN 0
#define BYTES_BIG_ENDIAN 0
#define WORDS_BIG_ENDIAN 0
#define UNITS_PER_WORD 2
#define POINTER_SIZE BITS_PER_WORD
/* TODO: POINTERS_EXTEND_UNSIGNED for A20 support */
#define PARM_BOUNDARY BITS_PER_WORD
#define STACK_BOUNDARY PARM_BOUNDARY
#define FUNCTION_BOUNDARY BITS_PER_WORD
#define BIGGEST_ALIGNMENT BITS_PER_WORD
#define MINIMUM_ATOMIC_ALIGNMENT BITS_PER_UNIT
#define EMPTY_FIELD_BOUNDARY BITS_PER_WORD
#define STRUCTURE_SIZE_BOUNDARY BITS_PER_WORD
#define STRICT_ALIGNMENT 1
/* DEFAULT: #define MAX_FIXED_MODE_SIZE GET_MODE_BITSIZE (DImode) */

/* 17.6 Type Layout */
#define SHORT_TYPE_SIZE 16
#define LONG_TYPE_SIZE 32
#define LONG_LONG_TYPE_SIZE 64
#define FLOAT_TYPE_SIZE 32
#define DOUBLE_TYPE_SIZE FLOAT_TYPE_SIZE
#define LONG_DOUBLE_TYPE_SIZE FLOAT_TYPE_SIZE
#define DEFAULT_SIGNED_CHAR 1
#define SIZE_TYPE "unsigned int"
/* DEFAULT: #define PTRDIFF_TYPE "long int */

/* 17.7 Registers */
#define MSP430_CG1_REGNUM 2
#define MSP430_CG2_REGNUM 3
#define MSP430_MIN_GENERAL_REGNUM 4
#define MSP430_MAX_GENERAL_REGNUM 15
#define MSP430_MAX_HARD_REGNUM 15
#define MSP430_RETURN_REGISTER_BASE 15
#define MSP430_ARG_REGISTER_BASE 15
#define MSP430_ARG_REGISTER_COUNT 4
/* 1 if REG is a valid register number (not a fixed pseudo-reg) */
#define MSP430_HARD_REGISTER_NUM_P(REGNO) \
  ((unsigned int)(REGNO) <= MSP430_MAX_HARD_REGNUM)
/* 1 if REG is a general register */
#define MSP430_GENERAL_REGISTER_NUM_P(REGNO) \
  (MSP430_MIN_GENERAL_REGNUM <= (REGNO) \
   && (REGNO) <= MSP430_MAX_GENERAL_REGNUM)

/* 17.7.1 Basic Characteristics of Registers */
#define FIRST_PSEUDO_REGISTER 18
#define FIXED_REGISTERS {							\
    1,		/* r0 : Program Counter (PC) */					\
    1,		/* r1 : Stack Pointer (SP) */					\
    1,		/* r2 : Status Register (SR); Constant Generator (CG1) */	\
    1,		/* r3 : Constant Generator (CG2) */				\
    0, 0, 0, 0, /* r4-r7 */							\
    0, 0, 0, 0, /* r8-r11 */							\
    0, 0, 0, 0, /* r12-r15 */							\
    1,		/* r16 (PSEUDO) : Argument Pointer */				\
    1,		/* r17 (PSEUDO) : Frame Pointer */				\
    }
#define CALL_USED_REGISTERS {				\
    1, 1, 1, 1,	/* r0-r3: fixed registers */		\
    0, 0, 0, 0,	/* r4-r7 : callee saved */		\
    0, 0, 0, 0,	/* r8-r11 : callee saved */		\
    1, 1, 1, 1,	/* r12-r15 : parameter registers */	\
    1, 1,	/* r16-r17 : fixed pseudo register */	\
    }
#define PC_REGNUM 0

/* 17.7.2 Order of Allocation of Registers */
/* TODO: SF 3191557 */
#define REG_ALLOC_ORDER { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 }

/* 17.7.3 How Values Fit in Registers */
#define HARD_REGNO_NREGS(REGNO, MODE)					\
  ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)
#define HARD_REGNO_MODE_OK(REGNO, MODE)					\
  MSP430_HARD_REGISTER_NUM_P((REGNO) + HARD_REGNO_NREGS (REGNO, MODE) - 1)
#define MODES_TIEABLE_P(MODE1, MODE2)			\
  (GET_MODE_SIZE (MODE1) == GET_MODE_SIZE (MODE2)	\
   || (GET_MODE_SIZE (MODE1) <= UNITS_PER_WORD		\
       && GET_MODE_SIZE (MODE2) <= UNITS_PER_WORD))

/* 17.8 Register Classes */
int msp430_general_or_stack_reg (int regno);
int msp430_indirect_register_operand (rtx x);
int msp430_indexed_register_operand (rtx x);

enum reg_class
{
  NO_REGS,
  PC_REG,			/* r0 (PC) */
  SP_REG,			/* r1 (SP) */
  SR_REG,			/* r2 (SR) */
  CG1_REG,			/* r2 (CG1) */
  CG2_REG,			/* r3 (CG2) */
  CG_REGS,			/* r2,r3 */
  FP_REG,			/* r4 (HARD_FRAME_POINTER) */
  GENERAL_REGS,			/* r4-r15 */
  GENERAL_SP_REGS,		/* r1,r4-r15 */
  GENERAL_SR_REGS,		/* r2,r4-r15 */
  POINTER_REGS,			/* r1,r4-r15 */
  SOFT_ARGP_REG,		/* r16 (ARG_POINTER) */
  SOFT_FP_REG,			/* r17 (FRAME_POINTER) */
  FIXED_PSEUDO_REGS,		/* r16- */
  FIXED_PSEUDO_GENERAL_REGS,	/* r4-r15,r16- */
  FIXED_PSEUDO_GENERAL_SP_REGS,	/* r1,r4-r15,r16- */
  ALL_REGS,
  LIM_REG_CLASSES
};
#define N_REG_CLASSES (int) LIM_REG_CLASSES
#define REG_CLASS_NAMES {		\
		"NO_REGS",		\
		"PC_REG",		\
		"SP_REG",		\
		"SR_REG",		\
		"CG1_REG",		\
		"CG2_REG",		\
		"CG_REGS",		\
		"FP_REG",		\
		"GENERAL_REGS",		\
		"GENERAL_SP_REGS",	\
		"GENERAL_SR_REGS",	\
		"POINTER_REGS",		\
		"SOFT_ARGP_REG",	\
		"SOFT_FP_REG",		\
		"FIXED_PSEUDO_REGS",	\
		"FIXED_PSEUDO_GENERAL_REGS",	\
		"FIXED_PSEUDO_GENERAL_SP_REGS",	\
		"ALL_REGS" 		\
}
#define REG_CLASS_CONTENTS {			\
  {0x00000000ul},	/* NO_REGS */		\
  {0x00000001ul},	/* PC_REG */		\
  {0x00000002ul},	/* SP_REG */		\
  {0x00000004ul},	/* SR_REG */		\
  {0x00000004ul},	/* CG1_REG */		\
  {0x00000008ul},	/* CG2_REG */		\
  {0x0000000cul},	/* CG_REGS */		\
  {0x00000010ul},	/* FP_REG */		\
  {0x0000fff0ul},	/* GENERAL_REGS */      \
  {0x0000fff2ul},	/* GENERAL_SP_REGS */   \
  {0x0000fff4ul},	/* GENERAL_SR_REGS */   \
  {0x0000fff2ul},	/* POINTER_REGS */      \
  {0x00010000ul},	/* SOFT_ARGP_REG */     \
  {0x00020000ul},	/* SOFT_FP_REG */	\
  {0x00030000ul},	/* FIXED_PSEUDO_REGS */ \
  {0x0003fff0ul},	/* FIXED_PSEUDO_GENERAL_REGS */	\
  {0x0003fff2ul},	/* FIXED_PSEUDO_GENERAL_SP_REGS */ \
  {0x0003fffful}	/* ALL_REGS */		\
}
#define REGNO_REG_CLASS(REGNO) msp430_regno_reg_class (REGNO)
#define BASE_REG_CLASS POINTER_REGS
#define INDEX_REG_CLASS NO_REGS
#define REGNO_OK_FOR_BASE_STRICT_P(NUM) msp430_regno_ok_for_base_p(NUM, 1)
#define REGNO_OK_FOR_BASE_NONSTRICT_P(NUM) msp430_regno_ok_for_base_p(NUM, 0)
#ifdef REG_OK_STRICT
#define REGNO_OK_FOR_BASE_P(NUM) REGNO_OK_FOR_BASE_STRICT_P(NUM)
#else
#define REGNO_OK_FOR_BASE_P(NUM) REGNO_OK_FOR_BASE_NONSTRICT_P(NUM)
#endif
#define REGNO_OK_FOR_INDEX_P(NUM) 0
#define PREFERRED_RELOAD_CLASS(X, CLASS) CLASS
#define CLASS_MAX_NREGS(CLASS, MODE) HARD_REGNO_NREGS(_unreferenced, MODE)

/* 17.9 Old Constraints */

/* 17.10 Stack and Calling */
/* 17.10.1 Frame Layout */
#define STACK_GROWS_DOWNWARD
#define STARTING_FRAME_OFFSET 0
#define FIRST_PARM_OFFSET(FUNDECL) 0
#define INCOMING_RETURN_ADDR_RTX gen_rtx_MEM (Pmode, stack_pointer_rtx)
#define INCOMING_FRAME_SP_OFFSET GET_MODE_SIZE (Pmode)
#define CFA_FRAME_BASE_OFFSET(FUNDECL) msp430_cfa_frame_base_offset (FUNDECL)
#define RETURN_ADDR_RTX(COUNT, FRAMEADDR) msp430_return_addr_rtx (COUNT, FRAMEADDR)
/* 17.10.2 Exception Handling */
/* 17.10.3 Stack Checking */
/* 17.10.4 Frame Registers */
#define STACK_POINTER_REGNUM 1
#define STATUS_REGISTER_REGNUM 2
#define HARD_FRAME_POINTER_REGNUM 4
#define STATIC_CHAIN_REGNUM 6
#define ARG_POINTER_REGNUM 16
#define FRAME_POINTER_REGNUM 17
/* 17.10.5 Elimination */
#define ELIMINABLE_REGS {				\
    {ARG_POINTER_REGNUM, STACK_POINTER_REGNUM},		\
    {ARG_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM},	\
    {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},	\
    {FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM},	\
}
#define INITIAL_ELIMINATION_OFFSET(FROM_REG, TO_REG, OFFSET_VAR) \
  OFFSET_VAR = msp430_initial_elimination_offset (FROM_REG, TO_REG)
/* 17.10.6 Stack Arguments */
/* NB Default ACCUMULATE_OUTGOING_ARGS is 0, so default PUSH_ARGS is 1 */
#define PUSH_ROUNDING(NPUSHED) (((NPUSHED) + 1) & ~1)
/* 17.10.7 Register Arguments */
struct msp430_cumulative_args
{
  int nregs;			/* # registers available for passing */
  int regno;			/* next available register number */
};
#define CUMULATIVE_ARGS struct msp430_cumulative_args
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) \
  (msp430_function_arg (&(CUM), MODE, TYPE, NAMED))
#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, FNDECL, N_NAMED_ARGS ) \
  msp430_init_cumulative_args (&(CUM), FNTYPE, LIBNAME, FNDECL)
#define INIT_CUMULATIVE_INCOMING_ARGS(CUM, FNTYPE, LIBNAME) \
  msp430_init_cumulative_incoming_args (&(CUM), FNTYPE, LIBNAME)
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED) \
  msp430_function_arg_advance (&CUM, MODE, TYPE, NAMED)
#define FUNCTION_ARG_REGNO_P(R) \
  (R >= (MSP430_ARG_REGISTER_BASE - MSP430_ARG_REGISTER_COUNT + 1) \
   && R <= MSP430_ARG_REGISTER_BASE)
/* 17.10.8 Scalar Return */
/* TODO: Replace with target function */
#define FUNCTION_VALUE_REGNO_P(REGNO) ((REGNO) == MSP430_RETURN_REGISTER_BASE)
/* 17.10.9 Aggregate Return */
#define DEFAULT_PCC_STRUCT_RETURN 0
/* 17.10.10 Caller Saves */
/* 17.10.11 Function Entry */
#define EPILOGUE_USES(REGNO) msp430_epilogue_uses(REGNO)
/* 17.10.12 Profiling */
#define FUNCTION_PROFILER(FILE, LABELNO)  \
  fprintf (FILE, "/* profiler %d */", LABELNO)
/* 17.10.13 Tail Calls */
/* 17.10.14 Stack Smashing Protection */

/* 17.11 Varargs (no macros relevant) */

/* 17.12 Trampolines */
#define TRAMPOLINE_SIZE 8
#define TRAMPOLINE_ALIGNMENT BITS_PER_WORD

/* 17.13 Library Calls */
#define TARGET_LIB_INT_CMP_BIASED true

/* 17.14 Addressing Modes */
#define HAVE_POST_INCREMENT 1
#define CONSTANT_ADDRESS_P(X) (CONSTANT_P (X) && GET_CODE (X) != CONST_DOUBLE)
#define MAX_REGS_PER_ADDRESS 1
#define LEGITIMATE_CONSTANT_P(X) 1

/* 17.15 Anchored Addresses */

/* 17.16 Condition Code */
#define NOTICE_UPDATE_CC(EXP, INSN) msp430_notice_update_cc (EXP, INSN)

/* Extend conditions.h to explicitly note cases where the carry flag
   is invalid; these mostly are rotate-through-carry, and
   multi-instruction operations on 32- or 64-bit values.  (Note that
   the standard CC_NO_OVERFLOW is being used to refer to SR.V being
   reset.) */
#define CC_CLOBBER_C 0x1000

/* Extend conditions.h to explicitly note cases where the zero flag is
   invalid; these mostly are multi-instruction operations on 32- or
   64-bit values. */
#define CC_CLOBBER_Z 0x2000

/* Extend conditions.h to explicitly note cases where the overflow
   flag is invalid. */
#define CC_CLOBBER_V 0x4000

/* Extend conditions.h to note when the comparison was actually
   output. */
#define CC_EXPLICIT_COMPARE 0x8000

/* 17.17 Costs */
#define REGISTER_MOVE_COST(MODE, FROM, TO) ((MODE)==QImode ? 1 :	\
					    (MODE)==HImode ? 1 :	\
					    (MODE)==SImode ? 2 :	\
					    (MODE)==SFmode ? 2 : 4)
/* TODO: SF 3192592 */
#define MEMORY_MOVE_COST(MODE, CLASS, IN) ((MODE)==QImode ? 2 :		\
					   (MODE)==HImode ? 2 :		\
					   (MODE)==SImode ? 4 :		\
					   (MODE)==SFmode ? 4 : 8)
/* TODO: SF 3192592 */
#define BRANCH_COST(SPEED_P, PREDICTABLE_P) 0
/* TODO: SF 3192592 */
#define SLOW_BYTE_ACCESS 0
#define NO_FUNCTION_CSE

/* 17.18 Scheduling */

/* 17.19 Sections */
/* See also 17.21.6 for CTORS/DTORS */
#define TEXT_SECTION_ASM_OP "\t.text"
#define DATA_SECTION_ASM_OP "\t.data"
#define BSS_SECTION_ASM_OP "\t.section\t.bss"
#define READONLY_DATA_SECTION_ASM_OP "\t.section\t.rodata"

/* 17.20 PIC */
/* Not supported on this platform at this time */

/* 17.21 Assembler Format */
/* 17.21.1 File Framework */
#define ASM_COMMENT_START " ; "
#define ASM_APP_ON "#APP\n"
#define ASM_APP_OFF "#NOAPP\n"
/* Define this hook here so TARGET_HAVE_NAMED_SECTIONS is defaulted to
 * true in c-common.c */
#define TARGET_ASM_NAMED_SECTION default_elf_asm_named_section
/* 17.21.2 Data Output */
#define ASM_OUTPUT_ASCII(FILE, P, SIZE) gas_output_ascii (FILE, P, SIZE)
/* TODO: this is probably a misinterpretation of "logical line separator" */
#define IS_ASM_LOGICAL_LINE_SEPARATOR(C, STR) ((C) == '\n')
/* 17.21.3 Uninitialized Data */
#define ASM_OUTPUT_COMMON(STREAM, NAME, SIZE, ROUNDED)		\
  do {								\
    const char *p = NAME;					\
    if(*p == '*' || *p == '@' ) p++;				\
    if(*p >= '0' && *p <= '9' ) break;				\
    fputs ("\t.comm ", (STREAM));				\
    assemble_name ((STREAM), (NAME));				\
    fprintf ((STREAM), ",%lu%s", (unsigned long)(SIZE), (SIZE)>1?",2\n":"\n"); \
  } while (0)
#define ASM_OUTPUT_BSS(FILE, DECL, NAME, SIZE, ROUNDED)		\
  do {								\
    const char *p = (NAME);					\
    if(*p == '*' || *p == '@' ) p++;				\
    if(*p >= '0' && *p <= '9' ) break;				\
    asm_output_bss ((FILE), (DECL), (NAME), (SIZE), (ROUNDED)); \
  } while(0)
#define ASM_OUTPUT_LOCAL(STREAM, NAME, SIZE, ROUNDED)		\
  do {								\
    const char *p = NAME;					\
    if(*p == '*' || *p == '@' ) p++;				\
    if(*p >= '0' && *p <= '9' ) break;				\
    fputs ("\t.local ", (STREAM));				\
    assemble_name ((STREAM), (NAME));				\
    fputs ("\n",(STREAM));					\
    fputs ("\t.comm ", (STREAM));				\
    assemble_name ((STREAM), (NAME));				\
    fprintf ((STREAM), ",%lu%s", (unsigned long)(SIZE),(SIZE)>1?",2\n":"\n"); \
  } while (0)
/* 17.21.4 Label Output */
/* TODO: what's this workaround for?  came in at mspgcc4 */
#define ASM_OUTPUT_LABEL(STREAM, NAME)					\
  {									\
    int __msp430_data_name_workaround = (((NAME)[0] == '*') && ((NAME)[1] == '0')); \
    if (__msp430_data_name_workaround)					\
      {									\
	static int repeatable_pseudolabels_resolver = 0;		\
	fprintf (STREAM, "__");						\
	assemble_name (STREAM, NAME);					\
	fprintf (STREAM, "_%d", ++repeatable_pseudolabels_resolver);	\
      }									\
    else								\
      assemble_name (STREAM, NAME);					\
    fprintf (STREAM, ":\n");						\
  }
#define SIZE_ASM_OP "\t.size\t"
#define TYPE_ASM_OP "\t.type\t"
#define TYPE_OPERAND_FMT "@%s"
#define ASM_DECLARE_FUNCTION_NAME(FILE, NAME, DECL)	\
  msp430_asm_declare_function_name (FILE, NAME, DECL)
#define ASM_DECLARE_FUNCTION_SIZE(FILE, FNAME, DECL)		\
  do {								\
    if (!flag_inhibit_size_directive)				\
      {								\
        char label[256];					\
	static int labelno;					\
	labelno++;						\
	ASM_GENERATE_INTERNAL_LABEL (label, "Lfe", labelno);	\
	ASM_OUTPUT_LABEL (FILE, label);				\
	fprintf (FILE, "%s", SIZE_ASM_OP);			\
	assemble_name (FILE, (FNAME));				\
        fprintf (FILE, ",");					\
	assemble_name (FILE, label);				\
        fprintf (FILE, "-");					\
	assemble_name (FILE, (FNAME));				\
	fprintf (FILE,"\n;; End of function \n\n");		\
      }								\
  } while (0)
#define ASM_DECLARE_OBJECT_NAME(FILE, NAME, DECL)			\
  do {									\
    int __msp430_data_name_workaround = (((NAME)[0] == '*') && ((NAME)[1] == '0')); \
    if (__msp430_data_name_workaround)					\
      fprintf (FILE, "#");						\
    fprintf (FILE, "%s", "\t.type\t");					\
    assemble_name (FILE, NAME);						\
    putc (',', FILE);							\
    fprintf (FILE, TYPE_OPERAND_FMT, "object");				\
    putc ('\n', FILE);							\
    size_directive_output = 0;						\
    if (!flag_inhibit_size_directive && DECL_SIZE (DECL))		\
      {									\
	size_directive_output = 1;					\
	if (__msp430_data_name_workaround)				\
	  fprintf (FILE, "#");						\
	fprintf (FILE, "%s", SIZE_ASM_OP);				\
	assemble_name (FILE, NAME);					\
	fprintf (FILE, ",%lu\n",  int_size_in_bytes (TREE_TYPE (DECL))); \
      }									\
    ASM_OUTPUT_LABEL(FILE, NAME);					\
  } while (0)
#define ASM_FINISH_DECLARE_OBJECT(FILE, DECL, TOP_LEVEL, AT_END)	\
  do {									\
    const char *name = XSTR (XEXP (DECL_RTL (DECL), 0), 0);		\
    if (!flag_inhibit_size_directive && DECL_SIZE (DECL)		\
	&& ! AT_END && TOP_LEVEL					\
	&& DECL_INITIAL (DECL) == error_mark_node			\
	&& !size_directive_output)					\
      {									\
	size_directive_output = 1;					\
	fprintf (FILE, "%s", SIZE_ASM_OP);				\
	assemble_name (FILE, name);					\
	fprintf (FILE, ",%lu\n",  int_size_in_bytes (TREE_TYPE (DECL))); \
      }									\
  } while (0)
#define ASM_WEAKEN_LABEL(FILE, NAME)		\
  do						\
    {						\
      fputs ("\t.weak\t", (FILE));		\
      assemble_name ((FILE), (NAME));		\
      fputc ('\n', (FILE));			\
    }						\
  while (0)
#define ASM_GENERATE_INTERNAL_LABEL(STRING, PREFIX, NUM)	\
  sprintf (STRING, "*.%s%lu", PREFIX, (unsigned long)NUM)
#define SUPPORTS_WEAK 1
/* Here we must catch r0 - r15 used as variable names */
#define ASM_OUTPUT_LABELREF(FILE,NAME) 			\
  do{							\
    const char *p = NAME;				\
    while(*p == '_') p++;				\
    if(*p == 'r' || *p == 'R')				\
      {							\
	int val;					\
	char *endptr;					\
	p++;						\
	val = strtol (p, &endptr, 10);			\
	if(val >= 0 && val <= 15 &&			\
	   *endptr == 0 )				\
	  {						\
	    asm_fprintf ((FILE), "_%U%s", (NAME));	\
	  }						\
	else						\
	  asm_fprintf ((FILE), "%U%s", (NAME));		\
      }							\
    else						\
      asm_fprintf ((FILE), "%U%s", (NAME));		\
  } while(0)
#define SET_ASM_OP "\t.set\t"
/* 17.21.5 Initialization */
/* 17.21.6 Macros for Initialization */
#define HAS_INIT_SECTION 1
/* The code that walks the [cd]tors list is not referenced in
 * _reset_vector_ or _stop_progExec__; doing so here ensures that code
 * is only linked in if it's actually needed. */
#define CTORS_SECTION_ASM_OP "\t.global\t__do_global_ctors\n\t.section\t.ctors"
#define DTORS_SECTION_ASM_OP "\t.global\t__do_global_dtors\n\t.section\t.dtors"
/* 17.21.7 Instruction Output */
#define REGISTER_NAMES				\
  {						\
    "r0", "r1", "r2", "r3",			\
    "r4", "r5", "r6", "r7",			\
    "r8", "r9", "r10", "r11",			\
    "r12", "r13", "r14", "r15",			\
    "argp", "sfp"				\
  }
#define PRINT_OPERAND(STREAM, X, CODE) msp430_print_operand (STREAM, X, CODE)
#define ASM_OUTPUT_REG_PUSH(STREAM, REGNO)		\
{							\
  gcc_assert (MSP430_HARD_REGISTER_NUM_P(REGNO));	\
  fprintf (STREAM, "\tpush\tr%d", REGNO);		\
}
#define ASM_OUTPUT_REG_POP(STREAM, REGNO)		\
{							\
  gcc_assert (MSP430_HARD_REGISTER_NUM_P(REGNO));	\
  fprintf (STREAM, "\tpop\tr%d", REGNO);		\
}
/* 17.21.8 Dispatch Tables */
#define ASM_OUTPUT_ADDR_VEC_ELT(STREAM, VALUE)		\
  msp430_output_addr_vec_elt(STREAM, VALUE)
/* 17.21.9 Exception Region Output */
/* 17.21.10 Alignment Output */
#define ASM_OUTPUT_SKIP(STREAM, N)		\
  fprintf (STREAM, "\t.skip %lu,0\n", (unsigned long)N)
#define ASM_OUTPUT_ALIGN(STREAM, POWER) \
  fprintf (STREAM, "\t.p2align %d,0\n", POWER)

/* 17.22 Debugging Info */
/* 17.22.1 All Debuggers */
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG
/* 17.22.2 DBX Options -- not supported */
/* 17.22.3 DBX Hooks -- not supported */
/* 17.22.4 File Names and DBX -- not supported */
/* 17.22.5 SDB and DWARF */
#define DWARF2_DEBUGGING_INFO 1
/* 17.22.6 VMS Debug -- not supported */

/* 17.23 Floating Point */
/* MSP430 doesn't have floating point, and its emulation uses IEEE.
 * Presumably, none of this section is relevant. */

/* 17.24 Mode Switching */
/* For things like configuring an FPU to single- or double-precision
 * mode.  Not currently relevant. */

/* 17.25 Target Attributes */
/* These are all hooks */

/* 17.26 Emulated TLS */
/* Thread Local Storage on a microcontroller?  Please. */

/* 17.27 MIPS Coprocessors -- not relevant */

/* 17.28 PCH Target */
/* Pre-compiled headers might be useful, but we're not using them
 * now. */

/* 17.29 C++ ABI */
/* These are all hooks */

/* 17.30 Named Address Spaces */
/* This might be relevant when dealing with near versus far memory.
 * It is specifically for use by embedded processors. */

/* 17.31 Misc */
#define CASE_VECTOR_MODE HImode
#define MOVE_MAX 2
#define SHIFT_COUNT_TRUNCATED 1
#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) 1
#define Pmode HImode
#define FUNCTION_MODE HImode
#define REGISTER_TARGET_PRAGMAS() do {					\
    c_register_pragma_with_expansion (0, "vector", msp430_pr_vector);	\
  } while (0)
#define DOLLARS_IN_IDENTIFIERS 0
#define NO_DOLLAR_IN_LABEL 1

/* Undocumented but legitimate */
#define OBJECT_FORMAT_ELF

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

enum msp430_frame_flags_e
{
  MSP430_FF_prologue_push_sr = 0x0001,
  MSP430_FF_use_reti = 0x0002,
  MSP430_FF_prologue_eint = 0x0004,
  MSP430_FF_prologue_dint = 0x0008,
  MSP430_FF_epilogue_eint = 0x0010,
  MSP430_FF_epilogue_dint = 0x0020,
  MSP430_FF_epilogue_exit_lpm = 0x0040,
  MSP430_FF_epilogue_pop_sr = 0x0080,
  MSP430_FF_use_frame_saver = 0x0100,
  MSP430_FF_preserve_registers = 0x0200,
  MSP430_FF_allocate_frame = 0x0400,
  MSP430_FF_inhibit_return = 0x0800,
  MSP430_FF_treat_as_main = 0x1000,
  MSP430_FF_ready_for_return = 0x2000
};

struct GTY(()) machine_function
{
  int initialized;
  tree signal;
  tree interrupt;
  tree naked;
  tree task;
  tree wakeup;
  tree critical;
  tree reentrant;
  tree saveprologue;
  tree noint_hwmul;
  tree hosted;

  /* Aligned frame size as recorded in expand_prologue */
  int frame_size;

  /* Bit-mask indicating the registers saved in expand_prologue */
  unsigned int saved_regs_mask;

  /* Number of registers saved in expand_prologue */
  int saved_regs_count;

  /* Bit-mask from msp430_frame_flags_e */
  unsigned int frame_flags;

  /* Vector offset for interrupt: non-negative multiple of two.
   * Negative indicates an unbound interrupt.  Field valid only if
   * interrupt is not null. */
  int vector_offset;

  /* Non-zero if we need the frame pointer (e.g., for a SR_IRQ
   * builtin) */
  int frame_pointer_required;

  /* An internal label to be emitted at the end of the epilog, in
   * functions that might inhibit a return but still have a return in
   * the middle of the code.  NULL unless frame_flags has
   * MSP430_FF_inhibit_return set. */
  const char *inhibited_return_label;
};
