/* Prototypes for exported functions defined in msp430.c
   
   Copyright (C) 2000, 2001 Free Software Foundation, Inc.
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


extern void gas_output_limited_string (FILE * file, const char *str);
extern void gas_output_ascii (FILE * file, const char *str, size_t length);

int msp430_regno_ok_for_base_p (int reg, int strict);

int msp430_adjust_insn_length (rtx insn, int length);

#ifdef HAVE_MACHINE_MODES
extern int msp430_hard_regno_mode_ok (int regno, enum machine_mode mode);
#endif

extern int msp430_initial_elimination_offset (int, int);

#ifdef TREE_CODE
void msp430_asm_declare_function_name (FILE *, const char *, tree);
unsigned int msp430_section_type_flags (tree DECL, const char *NAME,
					int RELOC);

#ifdef RTX_CODE			/* inside TREE_CODE */
extern rtx msp430_function_arg (CUMULATIVE_ARGS * cum,
				enum machine_mode mode, tree type, int named);
extern void msp430_init_cumulative_args (CUMULATIVE_ARGS * cum,
					 tree fntype, rtx libname,
					 tree fndecl);
extern void msp430_init_cumulative_incoming_args (CUMULATIVE_ARGS * cum,
						  tree fntype, rtx libname);
extern rtx msp430_function_incoming_arg (CUMULATIVE_ARGS * cum,
					 enum machine_mode mode, tree type,
					 int named);



#endif /* RTX_CODE inside TREE_CODE */

#ifdef HAVE_MACHINE_MODES	/* inside TREE_CODE */
extern void msp430_function_arg_advance (CUMULATIVE_ARGS * cum,
					 enum machine_mode mode, tree type,
					 int named);
#endif /* HAVE_MACHINE_MODES inside TREE_CODE */
#endif /* TREE_CODE */

#ifdef RTX_CODE

int msp430_extract_multiword_operand (enum machine_mode innermode, rtx op,
				      rtx * parts);

const char *msp430_output_template (enum machine_mode mode,
				    int src_operand,
				    const char *init_op,
				    const char *next_op,
				    const char *prefix);
const char * msp430_output_reverse_template (enum machine_mode mode,
					     int src_operand,
					     const char *init_op,
					     const char *next_op,
					     const char *prefix);

const char *msp430_mov_noclobber (rtx operands[]);

enum reg_class msp430_regno_reg_class (int);

int msp430_extend_matches (const_rtx, const_rtx);
void msp430_expand_signextend (rtx operands[]);
void msp430_expand_zeroextend (rtx operands[]);

int msp430_expand_extract (rtx operands[], bool signed_p);

extern void msp430_notice_update_cc (rtx body, rtx insn);
extern void msp430_output_addr_vec_elt (FILE * stream, int value);

extern void msp430_print_operand (FILE * file, rtx x, int code);

extern int msp430_jump_dist (rtx x, rtx insn);

extern void msp430_expand_mul (rtx[], int);
extern void msp430_expand_ashl (rtx[]);
extern void msp430_expand_ashr (rtx[]);
extern void msp430_expand_lshr (rtx[]);

extern void msp430_expand_cbranch (rtx[]);
extern const char* msp430_output_branchcc (rtx, rtx[]);

extern void msp430_expand_prologue (void);
extern void msp430_expand_epilogue (void);
extern void msp430_set_current_function (tree);
extern int msp430_epilogue_uses (int regno);

bool msp430_inhibited_return_fallthru_p (rtx insn);

#endif /* RTX_CODE */

extern void msp430_pr_vector (struct cpp_reader *);
#ifdef TREE_CODE
extern GTY(()) tree msp430_vector_offset_tree;
#endif /* TREE_CODE */

extern rtx msp430_return_addr_rtx (int, rtx);
extern int msp430_cfa_frame_base_offset (const_tree decl);
